#ifndef _EQUATION_H
#define _EQUATION_H

#include "particle.h"

/* Save input particle in local variable in equation */
void equation_init(particle*);

/* Equation for the charged particle motion. Lorentz force, magnetic field only. */
vector * equation_particle(double, vector* );

/* Test equation */
vector * equation_predator_prey(double, vector*);

#endif/*_VECTOR_H*/
