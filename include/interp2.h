#ifndef _INTERP2_H
#define _INTERP2_H

#include "magnetic_field.h"
#include "vector.h"

vector* interp2_interpolate(vector*);
/* Must be used before interpolation! */
void interp2_init_interpolation(magnetic_field*);

double **interp2_jacobian(vector*);
double diff_BrDr(double);

#endif/*_INTERP2_H*/
