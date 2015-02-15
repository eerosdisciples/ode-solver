/* Vector operations */

#include <stdarg.h>
#include <stdlib.h>
#include "vector.h"

/*
 * Create a new n-dimensional vector
 *
 * n: Number of dimensions
 * RETURNS: A new n-dimensional vector
 */
vector vnew(unsigned int n) {
	/* Declare vector */
	vector v;

	/* Set size of vector */
	v.n = n;

	/* Allocate memory for matrix */
	v.val = malloc(sizeof(double)*n);

	return v;
}
/*
 * Initialize vector with values from a given set of double's
 * 
 * n: Number of vector dimensions
 * VARIABLE ARGUMENTS: Vector elements. All must be double's!!
 * RETURNS: A new matrix containing the same elements
 * as v
 */
vector vinit(unsigned int n, ...) {
	/* Create a variable argument list */
	va_list valist;
	/* Allocate new vector */
	vector v = vnew(n);

	/* Initialize VA list */
	va_start(valist, n);

	/* Copy arguments to vector in order */
	unsigned int i;
	for (i = 0; i < n; i++) {
		double val = va_arg(valist, double);
		v.val[i] = val;
	}

	/* Clean memory */
	va_end(valist);

	return v;
}
/* 
 * Add two vectors
 *
 * a: First vector to add
 * b: Second vector to add
 * RETURNS: A new vector with
 * elements equal to a+b
 */ 
vector vadd(vector a, vector b) {
	/* Make sure a and b are of the same size */
	if (a.n != b.n)
		return (vector){NULL,0};
	
	/* Declare sum matrix */
	vector v = vnew(a.n);

	/* Sum over all elements */
	unsigned int i;
	for (i = 0; i < a.n; i++) {
		v.val[i] = a.val[i] + b.val[i];
	}

	return v;
}

/*
 * Multiply each element of a vector
 * by a scalar.
 *
 * scalar: Scalar to multiply with
 * a: Vector to multiply with
 */
vector vmuls(double scalar, vector a) {
	vector v = vnew(a.n);

	unsigned int i;
	for (i = 0; i < a.n; i++) {
		v.val[i] = scalar * a.val[i];
	}

	return v;
}
