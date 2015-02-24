/* Domain manager */

#include <stdlib.h>
#include "domain.h"

/**
 * Load a file containing data points giving
 * the domain of the problem.
 *
 * filename: Name of file to load domain from
 *
 * RETURNS the domain object loaded
 */
domain *domain_load(char *filename) {
	return NULL;
}

/**
 * Check if a point lies within, outside or
 * on the border of the domain.
 *
 * d: Domain for the problem
 * r: Radial coordinate
 * z: Z-coordinate
 *
 * RETURNS one of DOMAIN_WITHIN, DOMAIN_ONDOMAIN or
 * DOMAIN_OUTSIDE depending on wether the given point
 * is inside, on the border of or outside the given
 * domain
 */
int domain_check(domain *d, double *r, double* z) { 
		double det;/* Determinant */
	
	/* Variables used in method */
	double x00=r[0];
	double x10=r[1];
	double y00=z[0];
	double y10=z[1];
	double x01;
	double y01;
	double x11;
	double y11;
	double s;
	double t;
	/* Used in loop */  
	int i;
	
	/* Check if matrix is zero */
	if (x00-x10==0 && y00-y01==0)
		return DOMAIN_OUTSIDE;

	for (i=0;i<d->n;i++){
		x01=d->r[i];
		x11=d->r[i+1];
		y01=d->z[i];
		y11=d->z[i+1];
		
		/* Calculates the determinant */
		det=x11*y01-x01*y11;
		
		/* Calculates s and t */
		
		s=1/det*((x00-x10)*y01-(y00-y10)*x01);
		t=1/det*(-(-x00+x10)*y11+(y00-y10)*x11);
		
		/* If s and t are between 0 and 1 => intersection */
		if (s>=0 && s<=1 && t>=0 && t<=1)
			return DOMAIN_OUTSIDE;
		
	}
		
	return DOMAIN_WITHIN;
}

/**
 * Function for testing the module
 */
void domain_test(void) {}

