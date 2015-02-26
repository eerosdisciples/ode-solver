#ifndef _PARTICLE_H
#define _PARTICLE_H

/* The particle type is a six dimensional vector where the first three entries 
are position in cartesian coordinates and the three last are velocity in
cartesian coordinates. This represents a point particle travelling in
three spatial dimensions.*/

/* particle = (x,y,z,v_x,v_y,v_z) */

/* Define the particle type */
typedef struct {
  double val[6];		/* The particle vector */
} particle;

particle new(unsigned int);
particle particleInit(unsigned int, ...);
particle particleAdd(particle, particle);
particle muls(double, particle);

#endif/*_PARTICLE_H*/