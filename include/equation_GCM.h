#ifndef _EQUATION_GCM_H
#define _EQUATION_GCM_H

#include "IO_data.h"
#include "rkf45.h"

ode_solution *equation_GCM_init(vector*, initial_data*);
vector *equation_GCM(double, vector*);

#endif/*_EQUATION_GCM_H*/
