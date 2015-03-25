/* ODE Solver */
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "ctsv.h"
#include "equations.h"
#include "rkf45.h"
#include "vector.h"

/* 1e-7 seems pretty much the best.
 */
#define EPS0 1e-7                /* error tolerance */
#define SAFETY_FACTOR 0.9	/* Safety factor beta */
#define NUMBER_OF_TESTPOINTS 5000 /* for ode_test */

/* Stores Cash Carp coefficients A */
/*double A[6][6]={
  {0,0,0,0,0,0},
  {1.0/5,0,0,0,0,0},
  {3.0/40,9.0/40,0,0,0,0},
  {3.0/10,-9.0/10,6.0/5,0,0,0},
  {-11.0/54,5.0/2,-70.0/27,35.0/27,0,0},
  {1631.0/55296,175.0/512,575.0/13828,44275.0/110592,253.0/4096,0},
  };*/

/* Stores Cash Carp coefficients. Contains
   b in first row and bhat in second */
/*  double B[2][6]={
    {37.0/378,0,250.0/621,125.0/594,512.0/1771,0},
    {2825.0/27648,0,18575.0/48384,13525.0/55296,277.0/14336,1.0/4}
    };*/
/* Stores Cash Carp coefficients , used if explicit time dependence */
double c[]={1.0/5,3.0/10,1,7.0/8};

/* Stores Fehlberg coefficients A */
double A[6][6] = {
  {0,0,0,0,0,0},
  {1.0/4.0,0,0,0,0,0},
  {3.0/32.0,9/32.0,0,0,0,0},
  {1932.0/2197.0, -7200.0/2197.0, 7296.0/2197.0, 0, 0, 0},
  {439.0/216.0, -8.0, 3680.0/513.0, -845.0/4104.0, 0, 0},
  {-8.0/27.0, 2.0, -3544.0/2565.0, 1859.0/4104.0, -11.0/40.0, 0}
};
/* Stores Fehlberg coefficients. Contains
   b in first row and bhat in second */
double B[2][6] = {
  {25.0/216.0, 0, 1408.0/2565.0, 2197.0/4104.0, -1.0/5.0, 0},
  {16.0/135.0, 0, 6656.0/12825.0,28561.0/56430.0,-9.0/50.0,2.0/55.0}
};
/**
 * Solve an Initial Value Problem (IVP ODE)
 *
 * equation: Pointer to function giving the equation,
 * that is f in "z' = f(t,z)" where t is time
 * (scalar) and z is the unknown (vector)
 *
 * solver_object: pointer to a defined type ode_solution consisting 
 * of Z, step, flag.
 * Z contains the solution values from the previous iteration.
 * step is the optimal step size. flag indicates REDO_STEP (1) or OK_STEP (0).
 *
 * T: time
 *
 * RETURNS a solution to the equation as a pointer to an ode_solution consisting
 * of Z, step, flag.
 * Now Z contains the new solution points, new step size and new flag value.
 *
 * Called from main
 */
ode_solution* ode_solve(vector *(equation)(double, vector*),ode_solution *solver_object, double T){

  /* Choose order of iteration */
  unsigned int order1=4;
  unsigned int order2=5;
  double eps0=EPS0; // Tolerance parameter
  double beta=SAFETY_FACTOR; // Safety parameter
  /* Stores flag indicating whether the the iteration should be redone */
  int flag = solver_object->flag;
  /* current solution points */
  vector* Z = solver_object->Z;
  /* current step size */
  double h=solver_object->step;
  
  /* To store optimal steplenght*/
  double hopt;

  unsigned int i; // loop variable

  /* Calculate next point */
  vector* K = ode_step(equation, solver_object, T, order2);

  /* Calculate sum to be used in next point for Z_next and Zhat */
  /* Help variables */
  vector *sum1, *sum2;
  sum1=vnew(Z->n);
  sum2=vnew(Z->n);

  /* Initialize sum */
  for (i = 0; i < sum1->n; i++) {
    sum1->val[i] = 0;
    sum2->val[i] = 0;
  }

  /* Sum k1 to k5 (4th order method has "5 stages" (e.g. 5 k's) */
  for (i=0; i<=order1; i++){
    vector* ms = vmuls(h*B[0][i], K+i);
    vaddf(sum1,ms);
    vfree(ms);
  }

  /* Calculate next point */
  vector* Z_next= vadd(Z,sum1);
  vfree(sum1);

  /* Sum k1 to k6 (5th order method has "6 stages" (e.g. 6 k's) */
  for (i=0; i <= order2; i++){
    vector *ms = vmuls(h*B[1][i], K+i);
    vaddf(sum2,ms);
    vfree(ms);
  }

  vector* Zhat= vadd(Z,sum2);
  vfree(sum2);

  /* calculate relative error epsilon for each component,
   * save the largest error value */
  double eps, epst;

  /* Calculate the maximum error made */
  eps = fabs((Zhat->val[0] - Z_next->val[0]) /  Z_next->val[0]);
  for (i = 1; i < Z_next->n; i++) {
    epst = fabs((Zhat->val[i] - Z_next->val[i]) / Z_next->val[i]);
    if (epst > eps) {
      eps = epst;
    }
  }
  hopt = h; /* Optimal step size */

  /* Choose optimal step */
  if (eps >= eps0) {
    hopt=beta*h*pow(eps0/eps,0.20);
    flag=REDO_STEP;
  } else {
    hopt=h*pow(eps0/eps,0.25);
    flag=OK_STEP;
  }

  /* Save and return calculated values */
  solver_object->step = hopt;
  solver_object->flag = flag;
  vector* Zp1 = solver_object->Z+1;
  Zp1->val = Z_next->val;
  Zp1->n = Z_next->n;

  /* NOTE: We do not want to do a
   * vfree on Z, since `val' is now
   * used in Zp1!. Zhat however is
   * completely unused from now on */
  free(Z_next);
  vfree(Zhat);

  /* Free k's */
  for (i = 0; i <= order2; i++) {
    free(K[i].val);
  }
  free(K);

  /* Return the solver object */
  return solver_object;
}
/**
 * Calculates new solution points
 *
 * equation: Pointer to function giving the equation,
 * that is f in "z' = f(t,z)" where t is time
 * (scalar) and z is the unknown (vector)
 *
 * solver_object: pointer to a defined type ode_solution consisting 
 * of Z, step, flag.
 * Z contains the solution values from the previous iteration.
 * step is the optimal step size. flag indicates REDO_STEP (1) or OK_STEP (0).
 *
 * T: time
 *
 * order: order of RK-algorithm
 *
 * RETURNS array with K values
 *
 * Called from ode_solve.
 */

vector * ode_step(vector *(equation)(double, vector*),ode_solution *solver_object, double T, unsigned int order){
  
  vector* Z;
  Z=solver_object->Z;
  double h=solver_object->step;

  /* loop variables*/
  unsigned int i;
  unsigned int j;

  /* Array to store K-values 
     Allocate memory for each vector*/
  vector *K;
  K = malloc(sizeof(vector)*(order+1));
  for (i = 0; i <= order; i++) {
    K[i].n = Z->n;
  }

  /* Help variables, need only one if only one iteration is done; CHANGE IF NEEDED */
  vector *sum;

  /*vector *sum2; sum2=vinit(order-1);*/
  /* Cacluate first K */
  vector* vec = equation(T, Z);
  K[0].val = vec->val;

  free(vec);
  sum = vnew(Z->n);
  /* Calculate each K up to order. Start from K2 (i=1).
   * Note that a 4th order method contains 5 k's and
   * a 5th order method contains 6 k's */
  for (i=1; i <= order; i++) {
    /* Initialize sum */
    for (j = 0; j < Z->n; j++)
      sum->val[j] = 0;

    /*Calculate sum to use in argument */
    for (j=0; j < i; j++) {
      vector *ms = vmuls(h*A[i][j], K+j);
      vaddf(sum, ms);
      vfree(ms);
    }

    /* Calculate K */
    vaddf(sum, Z);
    vec=equation(T+c[i]*h,sum);
    K[i].val = vec->val;

    free(vec);
  }

  vfree(sum);

  return K;
}
/**
   Test function for this module 
*/
void ode_test(void) {
  /* Iteration variable */
  unsigned int i=0;

  /* Predator prey model
   */
  /* Initiate vector to store calculated points */
  vector* coordinates;
  unsigned int points = NUMBER_OF_TESTPOINTS;
  coordinates=malloc(sizeof(vector)*(points+1));

  /* Set initial point */
  coordinates->val = malloc(sizeof(double)*2);
  coordinates->n = 2;

  /* Initial condition:nbr of predators and prey */
  coordinates->val[0] = 34.91;
  coordinates->val[1] = 3.857;

  /* To store time */
  double *t = malloc(sizeof(double)*(points+1));
  t[0] = 0;
  /* To store dummy ''Energy'' */
  double *E = malloc(sizeof(double)*(points+1));
  for (i = 0; i < points; i++) {
    E[i] = 0;
  }

  /* Choose starting steplenght */
  double h=0.3;

  /* Save everything in type 'ode_solution' */
  ode_solution *param;
  param = malloc(sizeof(ode_solution));
  param->step=h;

  /* First value of flag is 'OK'*/
  param->flag=0;

  /* Iterate and save!*/
  /* Choose size of intervall */

  for (i = 0; i < points; i++) {
    t[i+1]=t[i]+param->step;

    /* Iterate once */
    param->Z = coordinates+i;
    //    ode_solve(equation_predator_prey, param, t[i]);

    /* Check if iteration needs to be re-done */
    /* flag=0 -> ok -> continue */	
    if (param->flag!=0) i=i-1; // Redo step with new calculated h in param
  }

  solution_data output;
  output.T=t;
  output.E=E;
  output.v=coordinates;
  output.labels=malloc(sizeof(char *)*2);
  output.labels[0]="x";
  output.labels[1]="y";
  output.points=points;
  output.nvars=2;

  ctsv_write("Output.csv",',',&output, NULL);
}
