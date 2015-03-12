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

int main(int argc, char *argv[]) {
  /* Variable declarations */
  arguments *args;
  domain *dom;
  magnetic_field *mf;
  vector *solution;
  ode_solution *solvobj;
  particle *part;
  args = parse_args(argc, argv);
  unsigned int points = args->points, i;

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
  solution = malloc(sizeof(vector)*args->points);
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

  for (i = 0; i < points-1; i++) {
    solvobj->Z = solution+i;
    do {
      t[i+1] = t[i] + solvobj->step;
      ode_solve(equation_particle, solvobj, t[i]);
    } while (solvobj->flag == REDO_STEP);
  }

  printf("Ended at t = %e\n", t[i]);

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
  output.points=points;
  output.nvars=6;
  ctsv_write("particle.csv",',',&output);

  return 0;
}
