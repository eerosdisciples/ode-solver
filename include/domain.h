#ifndef _DOMAIN_H
#define _DOMAIN_H

/* Return values of `domain_check' */
#define DOMAIN_WITHIN 0
#define DOMAIN_OUTSIDE 1

/**
 * The `domain' data type contains the
 * coordinates for the points that make
 * up the walls of the reactor.
 */
typedef struct {
	double *r, *z;		/* Coordinates */
	int n;				/* Number of points */
} domain;

/* Load the domain coordinates from file */
domain *domain_load(char*); // * 
/* Check if the given points lies within,
 * outside or on the border of the domain
 */
int domain_check(domain*, double* , double* );
/* Check if the given point lies within,
 * outside or on the border of the domain
 */
int domain_pointin(domain*, double, double);
/* Function for testing the module */
void domain_test(void);

#endif/*_DOMAIN_H*/
