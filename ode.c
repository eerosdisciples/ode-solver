/* ODE Solver */

#include <stdlib.h>
#include "ode.h"
#include "vector.h"
#include <math.h>

/**
 * Solve an Initial Value Problem (IVP ODE)
 *
 * equation: Pointer to function giving the equation,
 *   that is f in "z' = f(t,z)" where `t' is time
 *   (scalar) and `z' is the unknown (vector)
 * t0: Initial time
 * z0: Initial value
 * n:  Number of points to calculate
 * T:time
 * Z: vector containing values from previous iteration
 * RETURNS a solution to the equation as a defined type ode_solution consisting of Z,step, flag
 */


ode_solution* ode_solve( vector *(equation)(double, vector*),ode_solution *parameters,double T){
	/* TODO: find a better way to calculate absolute value
	Chech if everythinf is correct */
	
	
	/* Choose order of iteration */
	int order1=4;
	int order2=5;
	
	/* Take out h */
	double h=parameters->step;

	 double eps0=0.7;           /* Tolerans parameter */
	 double beta=0.7;           /* Safety parameter */
	 int flag; // Variable to store value indicating whether the the iteration should be re-done
	 
	 /* To store optimal steplenght*/
	 double hopt;
	 /* Calculate next point */
	 
	 vector* Z_next=ode_step(equation, parameters,T,order1);
	 vector* Zhat=ode_step(equation, parameters,T,order2);
	 
	 /* Calculate epsilon. Absolute value of function */
	 
	 double eps=pow(vdot(vadd(vmuls(-1,Z_next),Zhat),vadd(vmuls(-1,Z_next),Zhat)),1.0/2);/* = absolute value of vector () 
		 Check that each argument is ok
		 find a faster solution, without calculating k in each turn*/
	 
	 /* Choose optimal step */
	 if (eps>=eps0){
		 hopt=beta*h*pow(eps0/eps,1/5);
		 flag=REDO_STEP;
	 }
	 else  { hopt=beta*h*pow(eps0/eps,1/4);
		 flag=OK_STEP;
	 }
	 
	 /* Save and return calculated values */
	 ode_solution* solution;
	 solution->Z=Z_next;
	 solution->step=hopt;
	 solution->flag=flag;
	 
	 return solution;// SHOULD RETURN Z_NEXT, HOPT, FLAG
}
/**
 * Calculates the next point depending on the order
 *
 * Same arguments as in ode_solve
 * Z: vector containing values from previous iteration
 * RETURNS array with K values
 */

vector * ode_step(vector *(equation)(double, vector*),ode_solution *parameters, double T,int order){

	/*TODO 	WHICH COEFFICIENTS TO USE; BHAT OR B*/
 double A[5][6]; /* Stores Cash Carp coefficients */
 double b[5]; /* Stores Cash Carp coefficients */
 double bhat[5]; /* Stores Cash Carp coefficients */
 double alpha[6];/* Stores Cash Carp coefficients, if explicit time dependence */
 vector* Z=parameters->Z;
 double h=parameters->step;
 /* Variables used in loop*/
 int i;
 int j;
 
 /* Array to store K-values CHANGE IF PARTICLE TYPE */
 vector* K[order];
 
 /* Help variables, need only one if only one iteration is done; CHANGE IF NEEDED */
 vector *sum;
 //vector sum2;
 
 /*  Calculate each K up to order*/
 
 for(i=0;i<order;i++){
	 
	 /*Cacalculate sum to use in argument */ 
	 for (j=0;j<order-1;j++){
		 sum = vmuls(h*A[i][j],K[j]);
	 }
	 
	 /* Calculate K */
	 	 K[i]=equation(T+alpha[i]*h,vadd(Z,sum));
	
 }
 
 /* Calculate sum do be used in next point */
 for (i=0;i<order;i++){
	 sum=vmuls(h*b[i],K[i]);
 }
 
 /* Calculate next point */
 
	return vadd(Z,sum);
}

/* Test function for this module */
void ode_test(void) {

/* Predator prey model
 */


}