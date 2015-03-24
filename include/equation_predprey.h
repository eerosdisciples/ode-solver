#ifndef _EQUATION_PREDPREY_H
#define _EQUATION_PREDPREY_H

#include "IO_data.h"
#include "vector.h"
#include "rkf45.h"


/* Test equation */
vector * equation_predator_prey(double, vector*);

ode_solution* predator_init(initial_data *);


#endif/*_EQUATION_PREDPREY_H*/
