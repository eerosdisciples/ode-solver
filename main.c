/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ode.h"
#include <math.h>
#include "arguments.h"
#include "domain.h"
#include "magnetic_field.h"
#include "ode.h"
#include "readfile.h"
#include "equation.h"
#include "ctsv.h"
#define SAFETY_FACTOR 0.9	/* Safety factor beta */
#define NUMBER_OF_TESTPOINTS 100
#define REFERENCE_POINT_X 6 //4.79839
#define REFERENCE_POINT_Y 0 //1.78125
vector *dummy_eq(double t, vector *z) {
  return NULL;
}
int main(int argc, char *argv[]) {
  /* Testing equation function */
  /* Initialize interpolator */
  equation_init();
  /* Create domain varible */
  domain *d = domain_load("iter.wall_2d");
  /* Initiate vector to store calculated points */
  vector* coordinates;
  unsigned int points = NUMBER_OF_TESTPOINTS;
  coordinates=malloc(sizeof(vector)*(points+1));
  /* Set initial point */
  coordinates->val = malloc(sizeof(double)*6);
  coordinates->n = 6;
  /* Initial conditions*/
  coordinates->val[0] = 8.0287;
  coordinates->val[1] = -0.0105;
  coordinates->val[2] = 0.2538 ;
  coordinates->val[3] = -9.5487e6;
  coordinates->val[4] = -7.7664e6;
  coordinates->val[5] = -4.1652e6;
  /* To store time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0] = 0;
  /* Choose starting steplenght */
  double h=1e-5; // Måste vara litet om vi ska kunna använda interpolation
  /* Save everything in type 'ode_solution' */
  ode_solution *param;
  param = malloc(sizeof(ode_solution));
  param->step=h;
  /* First value of flag is 'OK'*/
  param->flag=0;
  /* Iterate and save!*/
  /* Iteration variable */
  unsigned int i=0;
  /* Help variables to use in domain_check */
  double x;
  double z;
  double xnew;
  double znew;
  /* Check if starting point inside or outside */
  x=6;
  z=0;
  xnew=coordinates->val[0];
  znew=coordinates->val[1];
  double X[2]={x,xnew};
  double Y[2]={z,znew};
  if (domain_check(d,X,Y)==1){ // Check if particle inside contour
    printf("Particle outside contour!!\n");
    return 0;
  }
  /* Do the next iteration */
  for (i = 0; i < points; i++) {
    t[i+1]=t[i]+param->step;
    /* Iterate once */
    param->Z = coordinates+i; // sparar föregående param
    x=param->Z->val[0];
    z=param->Z->val[1];
    //printf("To begin with %f %f",x,z);
    ode_solve(equation_particle, param, t[i]); // beräknar ny param
    //printf("To continue with %f %f",x,z);
    xnew=param->Z->val[0];
    znew=param->Z->val[2];
    double X[2]={x,xnew};
    double Y[2]={z,znew};
    /* Check if iteration needs to be re-done */
    /* flag=0 -> ok -> continue */	
    if (param->flag!=0) {i=i-1;} // Redo step with new calculated h in param
    else if (domain_check(d,X,Y)==1){ // Check if particle inside contour
      printf("Particle outside contour!!\n");
      return 0;
    }
  }
  ctsv_input output;
  output.t=t;
  output.v=coordinates;
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
  /* Solve */
  arguments *args;
  domain *dom;
  magnetic_field *mf;
  vector *solution;
  ode_solution *solvobj;
  args = parse_args(argc, argv);
  return 0;
  /* Load domain */
  dom = domain_load(args->domain_file);
  /* Load magnetic field */
  mf = magnetic_field_load(args->magfield_file);
  /* Solve */
  solution = malloc(sizeof(vector)*args->points);
  solvobj = malloc(sizeof(ode_solution));
  solvobj->step = 0.1; /* Initial step size */
  double *s = malloc(sizeof(double)*args->points);
  t[0] = args->tstart;
  for (i = 0; i < args->points-1; i++) {
    solvobj->Z = solution+i;
    do {
      s[i+1] = s[i] + solvobj->step;
      ode_solve(dummy_eq, solvobj, s[i]);
    } while (!solvobj->flag);
  }
  /* Output data */
  return 0;
}
