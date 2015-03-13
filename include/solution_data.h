#ifndef _SOLUTION_DATA_H
#define _SOLUTION_DATA_H

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



#endif/*_SOLUTION_DATA_H*/