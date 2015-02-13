/* Matrix operations */

#include "include/matrix.h"

/*
 * Create a new m-by-n matrix
 *
 * m: Number of rows
 * n: Number of columns
 * RETURNS: A new m-by-n matrix
 */
matrix mnew(unsigned int m, unsigned int n) {
	matrix nm;

	nm.m = m, nm.n = n;
	nm.data = malloc(sizeof(double*)*m);
	int i;

	for (i = 0; i < m; i++) {
		nm.data[i] = malloc(sizeof(double)*n);
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
matrix madd(matrix a, matrix b) {}

/*
 * Multiply each element of a matrix
 * by a scalar.
 *
 * scalar: Scalar to multiply with
 * a: Matrix to multiply with
 */
matrix mmuls(double scalar, matrix a) {}
