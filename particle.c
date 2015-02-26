/* Particle operations */

#include <stdarg.h>
#include <stdlib.h>
#include "particle.h"

/*
 * Create a new particle
 * RETURNS: pointer to a generic particle
 */
particle* particle_new() {
  /* Declare pointer to particle and allocate memory */
  particle *p;
  p = (particle*)malloc(sizeof(particle));

  return p;
}
/*
 * Initialize a new particle with specified position and velocity
 * RETURNS: Pointer to the new particle
 */

particle* particle_init(double x, double y, double z, double v_x, double v_y, double v_z ) { 
  /* Declare pointer to particle */
  particle *p = particle_new();

  p->val[0] = x;
  p->val[1] = y;
  p->val[2] = z;
  p->val[3] = v_x;
  p->val[4] = v_y;
  p->val[5] = v_z;

  return p;
}

}
/* 
 * Add two particles
 *
 * a: Pointer to first particle to add
 * b: Pointer to second particle to add
 * RETURNS: Pointer to a new particle with
 * elements equal to a+b
 */ 
particle* particle_add(particle *a, particle *b) {
	
	/* Declare pointer to new particle */
	particle *p = particle_new();

	/* Sum over all elements */
	unsigned int i;
	for (i = 0; i < 6; i++) {
		p->val[i] = a->val[i] + b->val[i];
	}

	return p;
}

/*
 * Get value of particle's x-coordinate position
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's x-coordinate position
 */

double particle_get_position_x(particle *a) {
  return a->val[0];
}

/*
 * Get value of particle's y-coordinate position
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's y-coordinate position
 */

double particle_get_position_y(particle *a) {
  return a->val[1];
}

/*
 * Get value of particle's z-coordinate position
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's z-coordinate position
 */

double particle_get_position_z(particle *a) {
  return a->val[2];
}

/*
 * Get value of particle's x-coordinate velocity
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's x-coordinate velocity
 */

double particle_get_velocity_x(particle *a) {
  return a->val[3];
}

/*
 * Get value of particle's y-coordinate velocity
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's y-coordinate velocity
 */

double particle_get_velocity_y(particle *a) {
  return a->val[4];
}

/*
 * Get value of particle's z-coordinate velocity
 *
 * a: Pointer to particle
 *
 * RETURNS: particle's z-coordinate velocity
 */

double particle_get_velocity_z(particle *a) {
  return a->val[5];
}
