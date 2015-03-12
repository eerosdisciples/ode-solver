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
  double mass, charge;	/* Particle mass and charge */
} particle;

particle* particle_new();
particle* particle_init(double, double, double, double, double, double);
particle* particle_add(particle*, particle*);

double particle_get_position_x(particle*);
double particle_get_position_y(particle*);
double particle_get_position_z(particle*);

double particle_get_velocity_x(particle*);
double particle_get_velocity_y(particle*);
double particle_get_velocity_z(particle*);

#endif/*_PARTICLE_H*/
