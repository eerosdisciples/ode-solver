#include <stdlib.h>
#include "vector.h"
#include "equation_particle.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "interp2.h"
#include "IO_data.h"
#include <math.h>

/* Global variable containing particle initial values defined in main */
initial_data* initial;

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

  double m=initial->mass; // particle mass
  double e=initial->charge; // particle charge

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

