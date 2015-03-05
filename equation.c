#include <stdlib.h>
#include "vector.h"
#include "equation.h"



/* Equation functions */

/*
 * Function for the Predator-Prey model
 *NOTE: NO expleicit time dependence
 * n: alpha,beta,delta, gm arbitrary parameters
 * RETURNS: vector of values of function in point 'arguments'
 */

vector * equation_predator_prey(double T,vector* Z){
	/* Define parameters */
	
	double alpha=0.3;
	double beta=1;
	double gm=2;
	double delta=4;
	
	/* Save values */
	double x=Z->val[0];
	double y=Z->val[1];
	
	printf("In predator prey x and y is %f %f,\n\n",x,y);
	
	
	/* To store output */
	vector* value;
	
	/* Calculate values */
	
	double fx=alpha*x-beta*x*y;
	double fy=delta*x*y-gm*y;
	
	value->val[0]=fx;
	value->val[1]=fy;
	printf("In predator prey %f %f,\n\n",fx,fy);
	return value;
}