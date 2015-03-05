#include <stdlib.h>
#include "vector.h"
#include "equation.h"



/* Equation functions */

/*
 * Function for the Predator-Prey model
 *x:nbr of prey
 *y:nbr of predator
 * parameters:
 *       alpha:growth
 *		 beta:rate of predation upon the prey
 *      delta: growth of the predator population
 *		 gm  : loss rate of the predators due to either natural death or emigration.
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
	
	
	
	/* To store output */
	vector* value;
	
	/* Calculate values */
	
	double fx=alpha*x-beta*x*y;
	double fy=delta*x*y-gm*y;
	
	value->val[0]=fx;
	value->val[1]=fy;
	return value;
}