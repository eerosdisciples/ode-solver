#ifndef _EQUATION_PARTICLE_H
#define _EQUATION_PARTICLE_H

#include "particle.h"

/* Save input particle in local variable in equation */
void equation_init(particle*);

/* Equation for the charged particle motion. Lorentz force, magnetic field only. */
vector * equation_particle(double, vector* );

#endif/*_EQUATION_PARTICLE_H*/
