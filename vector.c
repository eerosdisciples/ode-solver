/* Vector operations */

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

/*
 * Create a new n-dimensional vector
 *
 * n: Number of dimensions
 * RETURNS: A new n-dimensional vector
 */
vector *vnew(unsigned int n) {
	/* Declare vector */
	vector *v;
	v = malloc(sizeof(vector));

	/* Set size of vector */
	v->n = n;

	/* Allocate memory for matrix */
	v->val = malloc(sizeof(double)*n);

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
vector *vinit(unsigned int n, ...) {
	/* Create a variable argument list */
	va_list valist;
	/* Allocate new vector */
	vector *v = vnew(n);

	/* Initialize VA list */
	va_start(valist, n);

	/* Copy arguments to vector in order */
	unsigned int i;
	for (i = 0; i < n; i++) {
		double val = va_arg(valist, double);
		v->val[i] = val;
	}

	/* Clean memory */
	va_end(valist);

	return v;
}
/*
 * Free the memory occupied by a vector
 *
 * vec: Vector to free
 */
void vfree(vector *vec) {
	free(vec->val);
	free(vec);
}
/* 
 * Add two vectors
 *
 * a: First vector to add
 * b: Second vector to add
 * RETURNS: A new vector with
 * elements equal to a+b
 */ 
vector *vadd(vector *a, vector *b) {
	/* Make sure a and b are of the same size */
	if (a->n != b->n) {
		printf("ERROR: Given vectors are of different size!\n");
		exit(EXIT_FAILURE);
	}
	
	/* Declare sum matrix */
	vector *v = vnew(a->n);

	/* Sum over all elements */
	unsigned int i;
	for (i = 0; i < a->n; i++) {
		v->val[i] = a->val[i] + b->val[i];
	}

	return v;
}

/* 
 * Add two vectors, but DO NOT allocate
 * space for a new vector. Store result in `a'.
 *
 * a: First vector to add
 * b: Second vector to add
 * RETURNS: A new vector with
 * elements equal to a+b
 */ 
vector *vaddf(vector *a, vector *b) {
	/* Make sure a and b are of the same size */
	if (a->n != b->n) {
		printf("ERROR: Given vectors are of different size!\n");
		exit(EXIT_FAILURE);
	}
	
	/* Sum over all elements */
	unsigned int i;
	for (i = 0; i < a->n; i++) {
		a->val[i] += b->val[i];
	}

	return a;
}

/*
 * Multiply each element of a vector
 * by a scalar.
 *
 * scalar: Scalar to multiply with
 * a: Vector to multiply with
 */
vector *vmuls(double scalar, vector *a) {
	vector *v = vnew(a->n);

	unsigned int i;
	for (i = 0; i < a->n; i++) {
		v->val[i] = scalar * a->val[i];
	}

	return v;
}

/*
 * Multiply each element of a vector
 * by a scalar, but DO NOT allocate
 * new memory for resulting vector.
 *
 * scalar: Scalar to multiply with
 * a: Vector to multiply with
 */
vector *vmulsf(double scalar, vector *a) {
	unsigned int i;
	for (i = 0; i < a->n; i++) {
		a->val[i] = scalar * a->val[i];
	}

	return a;
}

/*
 * Caclulates scalar product between vectors.
 * u: Vector
 * v: Vector 
 * Must be the same size
 */

/*
 * Caclulates scalar product between vectors.
 * u: Vector
 * v: Vector 
 * Must be the same size
 */
double vdot(vector *v, vector *u) {
	/* Check if same size */
	if (v->n!=u->n)	{
		printf("ERROR: Vectors not same size");
		return 0;	
	}
	double scalar=0;

	unsigned int i;
	for (i = 0; i < v->n; i++) {
		scalar = scalar + v->val[i]*u->val[i];
	}

	return scalar;
}

/**
 * Clone a vector
 *
 * v: Vector to clone
 */
vector *vclone(vector *v) {
	vector *p = vnew(v->n);
	unsigned int i;
	for (i = 0; i < v->n; i++) {
		p->val[i] = v->val[i];
	}

	return p;
}
