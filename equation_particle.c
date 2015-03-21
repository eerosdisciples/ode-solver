#include <stdlib.h>
#include "vector.h"
#include "equation_particle.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "interp2.h"
#include "particle.h"
#include <math.h>

particle* equation_partobj;

/**
Initializes equation for particle object:
* saves particle in local variable. 
*/
void equation_init(particle *p) {
	equation_partobj = p;
}
/* Equation functions */

/**
 * Equation for the charged particle motion
 * Lorentz force, only magnetic field.
 * 
 * T: time, not used here but needed for ode.c
 * Z: Pointer to vector containing particle position
 * coordinates in the first three values, and particle
 * velocity in the next three.
 *
 * RETURNS: vector of values of function f 
 *
 * Used as the first argument to ode_solve in ode.c
 */
vector * equation_particle(double T, vector* Z){
  /*TODO Change from vector to particle + arbitrary parametrs m,e */
  /* Define needed parameters */
  double amu_to_kg=1.66053886e-27;
  double ev=1.60217657e-19;
  double m=equation_partobj->mass*amu_to_kg;
  double e=equation_partobj->charge*ev;

  /* Save xyz coordinates of particle*/
  vector *xyz=vnew(3);
  xyz->val[0]=Z->val[0];
  xyz->val[1]=Z->val[1];
  xyz->val[2]=Z->val[2];

  /* Save  particle velocity*/
  double v1=Z->val[3],
    v2=Z->val[4],
    v3=Z->val[5];

  /* Get magnetic field in point of particle */
  vector *B = magnetic_field_get(xyz);

  /* Extract x, y and z values of magnetic field */
  double B1=B->val[0],
    B2=B->val[1],
    B3=B->val[2];
  /* Calculate each function (f) value */
  double f1=v1,
    f2=v2,
    f3=v3,
    f4=(e/m)*(v2*B3-v3*B2),
    f5=(e/m)*(v3*B1-v1*B3),
    f6=(e/m)*(v1*B2-v2*B1);
   
  vfree(xyz);
  vfree(B);

  /* Save in vector and return */
  vector* value;
  value=vnew(6);
  value->val[0]=f1;
  value->val[1]=f2;
  value->val[2]=f3;
  value->val[3]=f4;
  value->val[4]=f5;
  value->val[5]=f6;

  return value;
}

