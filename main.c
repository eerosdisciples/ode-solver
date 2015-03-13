/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "arguments.h"
#include "ctsv.h"
#include "domain.h"
#include "equation.h"
#include "interp2.h"
#include "magnetic_field.h"
#include "ode.h"
#include "readfile.h"
#include "solution_data.h"

#define NUMBER_OF_POINTS 1000 // number of data points to start with
/* Reference point to check if initial position is inside domain */
#define REFERENCE_POINT_X 6 
#define REFERENCE_POINT_Y 0 

int main(int argc, char *argv[]) {
  /* Variable declarations */
  arguments *args;
  domain *dom;
  magnetic_field *B;
  vector *solution;
  ode_solution *solver_object;
  particle *part;
  args = parse_args(argc, argv);
  double vx,vy,vz;
  unsigned int i;
  
  unsigned int points;
  points = NUMBER_OF_POINTS;

  /* Load domain */
  dom = domain_load(args->domain_file);
  /* Load magnetic field */
  B = magnetic_field_load(args->magfield_file);
  /* Initialize particle */
  part = malloc(sizeof(particle));
  part->mass = args->particle_mass;
  part->charge = args->particle_charge;

  /* Initialization of interpolation and equation */
  interp2_init_interpolation(B);
  equation_init(part);

  /* Allocate memory for solution vector */
  solution = malloc(sizeof(vector)*points);
  /* solver_object of type ode_solution contains solution points, optimal
   * step size, and flag indicating ok step. */
  solver_object = malloc(sizeof(ode_solution));
  solver_object->step = 1e-10; /* Initial step size */

  /* initial velocity from input file pi. TODO: Load input data to particle object, pass particle to solver function. Edit the particle type first. */
  vx = args->v0[0];
  vy = args->v0[1];
  vz = args->v0[2];

  /* Set initial values */
  solution->n = 6;
  solution->val = malloc(sizeof(double)*6);
  solution->val[0] = args->r0[0];
  solution->val[1] = args->r0[1];
  solution->val[2] = args->r0[2];
  solution->val[3] = vx;
  solution->val[4] = vy;
  solution->val[5] = vz;

  /* For storing time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0] = args->tstart;
  /* For storing energy */
  double *E = malloc(sizeof(double)*(points+1));
  E[0] = part->mass/2*(vx*vx + vy*vy + vz*vz);

  /* For checking domain */
  double x,y,z,r;

  x = solution->val[0];
  y = solution->val[1];
  z = solution->val[2];
  r = sqrt(x*x + y*y);
  
  double R[2] = {6,r};
  double Z[2] = {0,z};

  if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
    printf("Particle starts outside reactor!\n");
    exit(EXIT_FAILURE);
  }

  i = 0;
  while (t[i] < args->tend) {
    /* Check if we have enough points */
    if (i+2 >= points) {
      unsigned int np = (unsigned int)ceil(points * args->tend/t[i]);
      solution = realloc(solution, np*(sizeof(vector)));
      t = realloc(t, np*sizeof(double));
      E = realloc(E, np*sizeof(double));

      points = np;
    }

    solver_object->Z = solution+i;
    do {
      t[i+1] = t[i] + solver_object->step;
      ode_solve(equation_particle, solver_object, t[i]);
    } while (solver_object->flag == REDO_STEP);

    x = solution[i].val[0];
    y = solution[i].val[1];
    z = solution[i].val[2];
    vx= solution[i].val[0];
    vy= solution[i].val[1];
    vz= solution[i].val[2];
    r = sqrt(x*x + y*y);
    E[i+1] = part->mass/2 * (vx*vx + vy*vy + vz*vz);

    /* Move on to next iteration */
    i++;

    R[0] = R[1]; Z[0] = Z[1];
    R[1] = r;    Z[1] = z;
    if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
      printf("Particle collided with reactor wall!\n");
      break;
    }
  }

  printf("Number of points: %d\n", i);

  /* Output data */
  solution_data output;
  output.T=t;
  output.E=E;
  output.v=solution;
  output.labels=malloc(sizeof(char *)*6);
  output.labels[0]="x";
  output.labels[1]="y";
  output.labels[2]="z";
  output.labels[3]="vx";
  output.labels[4]="vy";
  output.labels[5]="vz";
  output.points=i;
  output.nvars=6;
  ctsv_write("particle.csv",',',&output);

  return 0;
}
