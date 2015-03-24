#ifndef _EQUATION_GCM_H
#define _EQUATION_GCM_H

#include "IO_data.h"

ode_solution *equation_GCM_init(initial_data*, vector*);
vector *equation_GCM(double, vector*);

#endif/*_EQUATION_GCM_H*/
