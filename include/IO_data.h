#ifndef _IO_DATA_H
#define _IO_DATA_H

#include "vector.h"

/**
 * Structure defining the type solution_data, containing results from a 
 * run of the integrator.
 */
typedef struct {
  double* T; 				// Time points
  double* E;  			// Energy 
  vector *v;			// List of coordinates to output 
  char **labels;			//List of labels 
  unsigned int points;	// Number of points 
  unsigned int nvars;		// Number of coordinate-variables (or columns) 
} solution_data;


/**
 * Structure defining the type initial_data, containing initial parameters
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



#endif/*_IO_DATA_H*/
