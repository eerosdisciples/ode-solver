#ifndef _IO_DATA_H
#define _IO_DATA_H

#include "quantities.h"
#include "vector.h"

/**
 * Structure defining the type solution_data, containing results from a 
 * run of the integrator.
 */
typedef struct {
  double* T; 				// Time points
  quantity *quantities;		// List of interesting quantities
  vector *v;			// List of coordinates to output 
  char **labels;			//List of labels 
  unsigned int points;	// Number of points 
  unsigned int nvars;		// Number of coordinate-variables (or columns) 
  unsigned int no_quantities;	// Number of quantities
} solution_data;


/**
 * Structure defining the type initial_data, containing initial values
 * and particle constants.
 */
typedef struct {
  double t0;    // starting time
  double tmax;  // ending time
  /* initial position */
  double x0;			
  double y0;
  double z0;
  /* initial velocity */
  double vx0;          
  double vy0;
  double vz0;
  double mass;   // particle mass
  double charge; // particle charge 
} initial_data;

/**
 EXTERNAL VARIABLE CONTAINING PARTICLE INITIAL VALUES 
**/
extern initial_data* initial;

#endif/*_IO_DATA_H*/
