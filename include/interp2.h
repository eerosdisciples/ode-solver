#ifndef _INTERP2_H
#define _INTERP2_H

#include "magnetic_field.h"
#include "vector.h"

vector* interp2_interpolate(magnetic_field*, vector*);
double* interp2_create_grid(unsigned int, double, double);
void init(magnetic_field*, double*, double*);

#endif/*_INTERP2_H*/
