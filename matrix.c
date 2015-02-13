/* Matrix operations */

#include <stdlib.h>
#include "include/matrix.h"

/*
 * Create a new m-by-n matrix
 *
 * m: Number of rows
 * n: Number of columns
 * RETURNS: A new m-by-n matrix
 */
matrix mnew(unsigned int m, unsigned int n) {
	/* Declare matrix */
	matrix nm;

	/* Set size of matrix */
	nm.m = m, nm.n = n;

	/* Allocate memory for matrix */
	nm.val = malloc(sizeof(double*)*m);
	unsigned int i;
	for (i = 0; i < m; i++) {
		nm.val[i] = malloc(sizeof(double)*n);
	}

	return nm;
}
/* 
 * Add two matrices
 *
 * a: First matrix to add
 * b: Second matrix to add
 * RETURNS: A new matrix with
 * elements equal to a+b
 */ 
matrix madd(matrix a, matrix b) {
	/* Make sure a and b are of the same size */
	if (a.m != b.m || a.n != b.n)
		return (matrix){NULL,0,0};
	
	/* Declare sum matrix */
	matrix nm = mnew(a.m, a.n);

	/* Sum over all elements */
	unsigned int i, j;
	for (i = 0; i < a.m; i++) {
		for (j = 0; j < a.n; j++) {
			nm.val[i][j] = a.val[i][j] + b.val[i][j];
		}
	}

	return nm;
}

/*
 * Multiply each element of a matrix
 * by a scalar.
 *
 * scalar: Scalar to multiply with
 * a: Matrix to multiply with
 */
matrix mmuls(double scalar, matrix a) {}
