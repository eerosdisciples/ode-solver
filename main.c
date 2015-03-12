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

#define NUMBER_OF_POINTS 1000
#define REFERENCE_POINT_X 6 //4.79839
#define REFERENCE_POINT_Y 0 //1.78125

int main(int argc, char *argv[]) {
  /* Variable declarations */
  arguments *args;
  domain *dom;
  magnetic_field *mf;
  vector *solution;
  ode_solution *solvobj;
  particle *part;
  args = parse_args(argc, argv);
  unsigned int points, i;

  points = NUMBER_OF_POINTS;

  /* Load domain */
  dom = domain_load(args->domain_file);
  /* Load magnetic field */
  mf = magnetic_field_load(args->magfield_file);
  /* Initialize particle */
  part = malloc(sizeof(particle));
  part->mass = args->particle_mass;
  part->charge = args->particle_charge;

  /* Initialization */
  interp2_init_interpolation(mf);
  equation_init(part);

  /* Solve */
  solution = malloc(sizeof(vector)*points);
  solvobj = malloc(sizeof(ode_solution));
  solvobj->step = 1e-10; /* Initial step size */

  /* Set initial values */
  solution->n = 6;
  solution->val = malloc(sizeof(double)*6);
  solution->val[0] = args->r0[0];
  solution->val[1] = args->r0[1];
  solution->val[2] = args->r0[2];
  solution->val[3] = args->v0[0];
  solution->val[4] = args->v0[1];
  solution->val[5] = args->v0[2];

  /* For storing time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0] = args->tstart;

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
		unsigned int np = 2 * points;
		solution = realloc(solution, np*(sizeof(vector)));
		t = realloc(t, np*sizeof(double));

		points = np;
	}

    solvobj->Z = solution+i;
    do {
      t[i+1] = t[i] + solvobj->step;
      ode_solve(equation_particle, solvobj, t[i]);
    } while (solvobj->flag == REDO_STEP);

	x = solution[i].val[0];
	y = solution[i].val[1];
	z = solution[i].val[2];
	r = sqrt(x*x + y*y);

	R[0] = R[1]; Z[0] = Z[1];
	R[1] = r;    Z[1] = z;
	if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
      printf("Particle collided with reactor wall!\n");
	  break;
	}

	i++;
  }

  printf("Number of points: %d\n", i);

  /* Output data */
  ctsv_input output;
  output.t=t;
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
