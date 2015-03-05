#ifndef _VECTOR_H
#define _VECTOR_H

/* Define the vector type */
typedef struct {
	double *val;		/* The actual vector */
	unsigned int n;		/* Number of dimensions */
} vector;

vector *vnew(unsigned int);
vector *vinit(unsigned int, ...);
void vfree(vector*);
vector *vadd(vector*, vector*);
vector *vmuls(double, vector*);
double vdot(vector *v, vector *u); 


#endif/*_VECTOR_H*/
