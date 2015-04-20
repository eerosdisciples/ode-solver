#include <stdio.h>
#include "IO_data.h"
#include "rkf45.h"
#include "equations.h"
#include <stdlib.h>
#include "problem_functions.h"
#include "arguments.h"

/**
 * Output labels for GCM.
 * RETURNS output with set labels
 */
solution_data* output_GCM(solution_data* output) {
  output->labels = malloc(sizeof(char*)*5);
  output->labels[0] = "u";
  output->labels[1] = "X";
  output->labels[2] = "Y";
  output->labels[3] = "Z";
  output->labels[4] = "mu";
  output->nvars = 5;

  return output;
}

/**
 * Output labels for regular particle motion.
 * RETURNS output with set labels
 */
solution_data* output_no_GCM(solution_data* output) {
  output->labels=malloc(sizeof(char *)*6);
  output->labels[0]="x";
  output->labels[1]="y";
  output->labels[2]="z";
  output->labels[3]="vx";
  output->labels[4]="vy";
  output->labels[5]="vz";
  output->nvars=6;

  return output;
}

/**
 * choose to solve GCM
 */
ode_solution* init_GCM(vector* solution, initial_data* initial) {
  solution->n = 5;
  solution->val = malloc(sizeof(double)*5);
  /* Get initial values for guiding center from 
   * particle initial values, store in solver_object */
  ode_solution *solver_object = equation_GCM_init(solution, initial);

  return solver_object;
}

/**
 * Choose to solve non GCM
 */
ode_solution* init_no_GCM(vector* solution, initial_data* initial) { 
  solution->n = 6;
  solution->val = malloc(sizeof(double)*6);

  solution->val[0] = initial->x0; 
  solution->val[1] = initial->y0;
  solution->val[2] = initial->z0; 
  solution->val[3] = initial->vx0;
  solution->val[4] = initial->vy0;
  solution->val[5] = initial->vz0;

  equation_particle_init(initial);

  ode_solution *solver_object = malloc(sizeof(ode_solution));
  solver_object->step = 1e-10;

  return solver_object;
}
/**
 * Select which problem to solve 
 */
problem* use_problem(arguments *args) {
  problem *prob = malloc(sizeof(problem));
  
  if (args->problem == PROBLEM_GC) {
    prob->init = init_GCM;
    prob->output = output_GCM;
    prob->equation = equation_GCM;
  }
  else {
    prob->init = init_no_GCM;
    prob->output = output_no_GCM;
    prob->equation = equation_particle;
  }
  return prob;
}
