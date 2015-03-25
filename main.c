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

/* number of data points to start with */
#define NUMBER_OF_POINTS 1000 
/* Reference point to check if initial position is inside domain */
#define REFERENCE_POINT_R 6 
#define REFERENCE_POINT_Z 0
/* conversion from atomic mass units to kg */
#define AMU_TO_KG 1.66053886e-27
/* elementary charge in Coloumbs */
#define CHARGE 1.60217657e-19
/* conversion from joule to eV */
#define ENERGY 6.24150934e18

/* global variable containing particle initial values defined in main.
 declared in IO_data.h */
initial_data *initial;

/**
 * Calculates saves the values of the particle motion with given parameters
 *
 * RETURNS a solution_data type named output containing all calculated 
 * data points.
 *
 * Called from main
 */

solution_data* main_solve(domain *dom, arguments *args){
  unsigned int i; // loop variable
  double x,y,z,r; // coordinates, cartesian and cylindrical
  double vx,vy,vz;// velocity, cartesian
  vector *solution;  
  ode_solution *solver_object;
  
  unsigned int points;
  points = NUMBER_OF_POINTS;
	
  /* Allocate memory for solution vector 
   *
   * solution vector will contain position in cartesian coordinates
   * and velocity: (x,y,z,vx,vy,vz)
   *
   */
  solution = malloc(sizeof(vector)*points);
  
  /* Choose problem to solve */
  if (args->problem == PROBLEM_GC) {
	  solution->n = 5;
	  solution->val = malloc(sizeof(double)*6);		
	  /* Get initial values for guiding center from 
          * particle initial values, store in solver_object */
	  solver_object = equation_GCM_init(solution);
  } else {
	  solution->n = 6;
	  solution->val = malloc(sizeof(double)*6);

	  solution->val[0] = initial->x0; 
	  solution->val[1] = initial->y0;
	  solution->val[2] = initial->z0; 
	  solution->val[3] = initial->vx0;
	  solution->val[4] = initial->vy0;
	  solution->val[5] = initial->vz0;

	  solver_object = malloc(sizeof(ode_solution));
	  solver_object->step = 1e-10; 
  }

  /* speeds are needed separately for calculations */
  vx = initial->vx0;
  vy = initial->vy0;
  vz = initial->vz0;
  /* for domain check of initial position */
  x = initial->x0;
  y = initial->y0;
  z = initial->z0;
  r = sqrt(x*x + y*y);

  /* For storing time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0]=initial->t0;
  /* For storing energy */
  double *E = malloc(sizeof(double)*(points+1));
  E[0] = initial->mass/2*(vx*vx + vy*vy + vz*vz)*ENERGY;

  /* domain check of initial position */
  double R[2] = {REFERENCE_POINT_R,r};
  double Z[2] = {REFERENCE_POINT_Z,z};

  if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
    printf("Particle starts outside reactor!\n");
    exit(EXIT_FAILURE);
  }
  
  /* Main loop. Loop until the final time has been reached */
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
	
	/* Our solution vector 'solution' has been pre-allocated. We
	 * simply point our pointer object solution vector 'Z' to the
	 * pre-allocated array. Since 'ode_solve' only gives us one
	 * step at a time this allows to fill the solution array gradually
	 */
    solver_object->Z = solution+i;
    do {
      t[i+1] = t[i] + solver_object->step;
	  if (args->problem == PROBLEM_GC)
        ode_solve(equation_GCM, solver_object, t[i]);
	  else
		ode_solve(equation_particle, solver_object, t[i]);
    } while (solver_object->flag == REDO_STEP);

	if (args->problem == PROBLEM_GC) {
		double u=solution[i].val[0];
		x = solution[i].val[1];
		y = solution[i].val[2];
		z = solution[i].val[3];
		r = sqrt(x*x + y*y);

		/* This is wrong at the moment, but I'm
		 * not quite sure what this should be... */
		E[i+1] = (initial->mass/2 * u*u) * ENERGY;
	} else {
		/* get new position and velocity */
		x = solution[i].val[0];
		y = solution[i].val[1];
		z = solution[i].val[2];
		vx= solution[i].val[3];
		vy= solution[i].val[4];
		vz= solution[i].val[5];
		r = sqrt(x*x + y*y);
		
		/* store new energy */
		E[i+1] = initial->mass/2 * (vx*vx + vy*vy + vz*vz)*ENERGY;
	}

    /* Move on to next iteration */
    i++;
    /* check if new position is inside domain */
    R[0] = R[1]; Z[0] = Z[1];
    R[1] = r;    Z[1] = z;
    if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
      printf("Particle collided with reactor wall!\n");
	  printf("  r = %e\n  z = %e\n", r, z);
      break;
    }
  }

  printf("Number of points: %d\n", i);
  printf("t = %e\n", t[i]);

  /* Output data */
  solution_data *output;
  output = malloc(sizeof(solution_data));
  output->T=t;
  output->E=E;
  output->v=solution;
  output->points=i;

  if (args->problem == PROBLEM_GC) {
	  output->labels = malloc(sizeof(char*)*5);
	  output->labels[0] = "u";
	  output->labels[1] = "X";
	  output->labels[2] = "Y";
	  output->labels[3] = "Z";
	  output->labels[4] = "mu";
	  output->nvars = 5;
  } else {
	  output->labels=malloc(sizeof(char *)*6);
	  output->labels[0]="x";
	  output->labels[1]="y";
	  output->labels[2]="z";
	  output->labels[3]="vx";
	  output->labels[4]="vy";
	  output->labels[5]="vz";
	  output->nvars=6;
  }
    
  return output;
}

/**
 * main function: stores initial values in global variable initial of
 * type initial_data. Also stores domain, magnetic field 
 * and initializes interpolation of the magnetic field.
 *
 * calls main_solve.
 * uses ctsv_write to write solution data to file.
 */
int main(int argc, char *argv[]) {
  /* Variable declarations */
  arguments *args;
  domain *dom;
  magnetic_field *B;
  
  args = parse_args(argc, argv);
 
  /* Load domain */
  dom = domain_load(args->domain_file);
  /* Load magnetic field */
  B = magnetic_field_load(args->magfield_file);

  /* Initialization of interpolation */
  interp2_init_interpolation(B);

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

  initial->mass = args->particle_mass*AMU_TO_KG;
  initial->charge = args->particle_charge*CHARGE;

  /* solve */
  solution_data *output = main_solve(dom, args);
  /* write solution data to file specified in input file */
  ctsv_write(args->output_file,',',output, args);

  return 0;
}

