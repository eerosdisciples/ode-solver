#ifndef _VECTOR_H
#define _VECTOR_H

/* Define the vector type */
typedef struct {
	double *val;		/* The actual vector */
	unsigned int n;		/* Number of dimensions */
} vector;

vector vnew(unsigned int);
vector vinit(unsigned int, ...);
vector vadd(vector, vector);
vector vmuls(double, vector);

#endif/*_VECTOR_H*/
