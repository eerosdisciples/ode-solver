#ifndef _ODE_H
#define _ODE_H

#include "vector.h"

typedef struct {
	double *t;		/* Time points */
	vector *z;		/* Solution points, z = z(t) */
	unsigned int n;	/* Number of points in solution */
} ode_solution;

/* Solve an initial value problem */
vector *ode_solve(
	vector *(equation)(double, vector*),
	double, vector*, unsigned int
);

void ode_test(void);

#endif/*_ODE_H*/
