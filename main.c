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
#include "IO_data.h"
#include "global.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "rkf45.h"
#include "quantities.h"
#include "problem_functions.h"

/* Reference point to check if initial position is inside domain */
#define REFERENCE_POINT_R 6 
#define REFERENCE_POINT_Z 0

unsigned int current_index=-1;

/**
 * Calculates saves the values of the particle motion with given parameters
 *
 * RETURNS a solution_data type named output containing all calculated 
 * data points.
 *
 * Called from main
 */

solution_data* main_solve(domain *dom, arguments *args, initial_data *initial){
  double x,y,z,r; // coordinates, cartesian and cylindrical
  vector *solution;  
  ode_solution *solver_object;
  
  unsigned int points;
  points = NUMBER_OF_SIMULATION_POINTS;

  /* domain check of initial position */
  x = initial->x0;
  y = initial->y0;
  z = initial->z0;
  r = sqrt(x*x + y*y);
  double R[2] = {REFERENCE_POINT_R,r};
  double Z[2] = {REFERENCE_POINT_Z,z};

  if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
    printf("Particle starts outside reactor!\n");
    exit(EXIT_FAILURE);
  }
  
  /* Allocate memory for solution vector 
   *
   * solution vector will contain position in cartesian coordinates
   * and velocity: (x,y,z,vx,vy,vz)
   */
  solution = malloc(sizeof(vector)*points);

  /* Set which problem to use from now on, GCM or regular particle motion */
  problem *prob =  use_problem(args);

  /* Store initial values in solver_object */ 
  solver_object = prob->init(solution, initial);

  /* For storing time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0]=initial->t0;

  /* Main loop. Loop until the final time has been reached */
  current_index = 0;
  while (t[current_index] < initial->tmax) {
    /* Check if we have enough points */
    if (current_index+2 >= points) {
      unsigned int np = (unsigned int)ceil(points * initial->tmax/t[current_index]);
      solution = realloc(solution, np*(sizeof(vector)));
      t = realloc(t, np*sizeof(double));
      quantities_expand(np);

      points = np;
    }
	
    /* Our solution vector 'solution' has been pre-allocated. We
     * simply point our pointer object solution vector 'Z' to the
     * pre-allocated array. Since 'ode_solve' only gives us one
     * step at a time this allows to fill the solution array gradually
     */
    solver_object->Z = solution+current_index;
    do {
      t[current_index+1] = t[current_index] + solver_object->step;
        ode_solve(prob->equation, solver_object, t[current_index]);
    } while (solver_object->flag == REDO_STEP);

    /* Move on to next iteration */
    current_index++;
    /* check if new position is inside domain */
	x=solver_object->Z->val[1];
	y=solver_object->Z->val[2];
	z=solver_object->Z->val[3];
    R[0] = R[1]; Z[0] = Z[1];
    R[1] = sqrt(x*x+y*y);    Z[1] = z;
    if (domain_check(dom, R, Z) == DOMAIN_OUTSIDE) {
      printf("Particle collided with reactor wall!\n");
      printf("  r = %e\n  z = %e\n", R[1], Z[1]);
      break;
    }
  }

  printf("Number of points: %d\n", current_index);
  printf("t = %e\n", t[current_index]);

  /* Output data */
  solution_data *output;
  output = malloc(sizeof(solution_data));
  output->T=t;
  output->quantities = quantities_get();
  output->no_quantities = quantities_get_no();
  output->v=solution;
  output->points=current_index;

  /* Set labels for output data, according to problem */
  output = prob->output(output);   
    
  return output;
}

/**
 * Store initial particle data in an "initial_data" data type.
 * RETURNS initial values
 */
initial_data* set_initial_values(arguments *args) {

  initial_data *initial;
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

  return initial;
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
  arguments *args;
  initial_data *initial;
  domain *dom;
  magnetic_field *B;
 
  
  args = parse_args(argc, argv); 
  initial = set_initial_values(args);
    
  /* Load domain */
  dom = domain_load(args->domain_file);
  /* Load magnetic field */
  B = magnetic_field_load(args->magfield_file);

  /* Initialization of interpolation */
  interp2_init_interpolation(B);

  /* solve */
  solution_data *output = main_solve(dom, args, initial);
  /* write solution data to file specified in input file */
  ctsv_write(args->output_file,',',output, args);

  return 0;
}
