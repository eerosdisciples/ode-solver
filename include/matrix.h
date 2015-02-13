#ifndef _MATRIX_H
#define _MATRIX_H

/* Define the matrix type */
typedef struct {
	double **val;		/* The actual matrix */
	unsigned int m;		/* Number of rows */
	unsigned int n;		/* Number of columns */
} matrix;

#endif/*_MATRIX_H*/
