/* Domain manager */

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
int domain_check(domain *d, double r, double z) {
	return DOMAIN_WITHIN;
}

/**
 * Function for testing the module
 */
void domain_test(void) {}

