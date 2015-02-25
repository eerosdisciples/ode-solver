/* Particle operations */

/* UNFINISHED MOD OF vector.c */

#include <stdarg.h>
#include <stdlib.h>
#include "particle.h"


/*
 * Create a new particle
 * RETURNS: a generic particle
 */
vector newParticle() {
	/* Declare six dimensional particle vector */
	particle p[6];

	particle p;
}
/*
 * Initialize a new particle with specified position and velocity
 * RETURNS: The new particle
 */

particle initParticle(double x, double y, double z, double v_x, double v_y, double v_z ) { 
  /* Declare particle */
  particle p[6];

  p[0] = x;
  p[0] = y;
  p[0] = z;
  p[0] = v_x;
  p[0] = v_y;
  p[0] = v_z;

  return p;
}

}
/* 
 * Add two particles
 *
 * a: First particle to add
 * b: Second particle to add
 * RETURNS: A new particle with
 * elements equal to a+b
 */ 
particle addParticle(particle a, particle b) {
	
	/* Declare new particle */
	particle p= newParticle();

	/* Sum over all elements */
	unsigned int i;
	for (i = 0; i < 6; i++) {
		p.val[i] = a.val[i] + b.val[i];
	}

	return p;
}
