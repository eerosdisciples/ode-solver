#ifndef _EQUATION_PARTICLE_H
#define _EQUATION_PARTICLE_H

#include "IO_data.h"
#include "vector.h"

/* Save input particle in local variable in equation */
void equation_particle_init(initial_data*);

/* Equation for the charged particle motion. Lorentz force, magnetic field only. */
vector * equation_particle(double, vector* );

#endif/*_EQUATION_PARTICLE_H*/
