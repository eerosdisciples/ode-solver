#ifndef _ODE_H
#define _ODE_H

#define REDO_STEP 1
#define OK_STEP 0

#include "vector.h"

typedef struct {
	vector * Z;    /* Solution points, z = z(t) */
	double step;	/* Optimal step */
	int flag;	   /* If iteration should be redone */
} ode_solution;

//	unsigned int n;	/* Number of points in solution */

/* Solve an initial value problem */
/*vector *ode_solve(
	vector *(equation)(double, vector*),
	double, vector*, unsigned int
);*/

vector * ode_step(vector *(equation)(double, vector*),ode_solution*, double T,unsigned int,int);
ode_solution* ode_solve( vector *(equation)(double, vector*),ode_solution *parameters,double T);

void ode_test(void);

#endif/*_ODE_H*/
