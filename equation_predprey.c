#include <stdlib.h>
#include "vector.h"
#includme "equation_predprey.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "interp2.h"
#include <math.h>
#include "IO_data.h"
#include "rkf45.h"

/**
 * Function for the Predator-Prey model
 * x: number of prey
 * y: number of predator
 * 
 * alpha:growth
 * beta:rate of predation upon the prey
 * delta: growth of the predator population
 * gm : loss rate of the predators due to either natural death or emigration.
 *
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
