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
	/*From source: http://www-rohan.sdsu.edu/~jmahaffy/courses/f09/math636/lectures/lotka/qualde2.html */
	double alpha=0.4807;
	double beta=0.02482;
	double gm=0.9272;
	double delta=0.02756;
	
	/* Save values */
	double x=Z->val[0];
	double y=Z->val[1];
	
	/* To store output */
	vector* value;
	value=vnew(2);
	/* Calculate values */
	
	double fx=alpha*x-beta*x*y;
	double fy=delta*x*y-gm*y;
	
	value->val[0]=fx;
	value->val[1]=fy;
	return value;
}
