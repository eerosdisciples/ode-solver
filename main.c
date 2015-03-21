/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "arguments.h"
#include "ctsv.h"
#include "domain.h"
#include "equations.h"
#include "interp2.h"
#include "magnetic_field.h"
#include "rkf45.h"
#include "readfile.h"
#include "IO_data.h"

#define NUMBER_OF_POINTS 1000 // number of data points to start with
/* Reference point to check if initial position is inside domain */
#define REFERENCE_POINT_R 6 
#define REFERENCE_POINT_Z 0 

/**
 * Calculates saves the values of the particle motion with given parameters
 * Uses ctsv_write
 * solution: Vector with particle parameters and initial values to function giving the equation,
 */

solution_data* main_solve(domain *dom,initial_data *initial, particle *part){
    unsigned int i;
    double vx,vy,vz;
    vector *solution;
    ode_solution *solver_object;
  
    unsigned int points;
    points = NUMBER_OF_POINTS;
	
    /* Allocate memory for solution vector */
    solution = malloc(sizeof(vector)*points);
    /* solver_object of type ode_solution contains solution points, optimal
     * step size, and flag indicating ok step. */
    solver_object = malloc(sizeof(ode_solution));
    solver_object->step = 1e-10; /* Initial step size */
		
    solution->n = 6;
    solution->val = malloc(sizeof(double)*6);
    solution->val[0] = initial->x0; 
    solution->val[1] = initial->y0;
    solution->val[2] = initial->z0; 
    solution->val[3] = initial->vx0;
    solution->val[4] = initial->vy0;
    solution->val[5] = initial->vz0;
	
	/* initial velocity from input file pi.
	 * TODO: Load input data to particle object,
	 * pass particle to solver function.
	 * Edit the particle type first.
	 */
	vx = initial->vx0;
	vy = initial->vy0;
	vz = initial->vz0;

    /* For storing time */
    double *t = malloc(sizeof(double)*(points+1));
	t[0]=initial->t0;
    /* For storing energy */
    double *E = malloc(sizeof(double)*(points+1));
    E[0] = part->mass/2*(vx*vx + vy*vy + vz*vz);

    /* For checking domain */
    double x,y,z,r;

    x = solution->val[0];
    y = solution->val[1];
    z = solution->val[2];
    r = sqrt(x*x + y*y);
  
    double R[2] = {REFERENCE_POINT_R,r};
    double Z[2] = {REFERENCE_POINT_Z,z};

    if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
      printf("Particle starts outside reactor!\n");
      exit(EXIT_FAILURE);
    }

    i = 0;
    while (t[i] < initial->tmax) {
      /* Check if we have enough points */
      if (i+2 >= points) {
        unsigned int np = (unsigned int)ceil(points * initial->tmax/t[i]);
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
      vx= solution[i].val[3];
      vy= solution[i].val[4];
      vz= solution[i].val[5];
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
    solution_data *output;
    output = malloc(sizeof(solution_data));
    output->T=t;
    output->E=E;
    output->v=solution;
    output->labels=malloc(sizeof(char *)*6);
    output->labels[0]="x";
    output->labels[1]="y";
    output->labels[2]="z";
    output->labels[3]="vx";
    output->labels[4]="vy";
    output->labels[5]="vz";
    output->points=i;
    output->nvars=6;
    
    return output;
}


int main(int argc, char *argv[]) {
  /* Variable declarations */
  arguments *args;
  domain *dom;
  magnetic_field *B;
  particle *part;
  args = parse_args(argc, argv);
  initial_data *initial;
 
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

  /* Set initial values */
  initial = malloc(sizeof(initial_data)); 
  initial->x0 = args->r0[0];
  initial->y0 = args->r0[1];
  initial->z0 = args->r0[2];
  
  initial->vx0 = args->v0[0];
  initial->vy0 = args->v0[1];
  initial->vz0 = args->v0[2];
	  
  initial->t0 = args->tstart;
  initial->tmax= args->tend;
  
  solution_data *output = main_solve(dom,initial,part);
  
  ctsv_write(args->output_file,',',output, args);

  return 0;
}
