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


typedef struct {
	double* T; 		/* Time points */
	double* E;  		/* Energy */
	vector *v;		/* List of coordinates to output */
	char **labels;			/* List of labels */
	unsigned int points;	/* Number of points */
	unsigned int nvars;		/* Number of coordinate-variables (or columns) */

} solution_data;



/* Solve an initial value problem */
/*vector *ode_solve(
	vector *(equation)(double, vector*),
	double, vector*, unsigned int
);*/

vector * ode_step(vector *(equation)(double, vector*),ode_solution*, double ,unsigned int);
ode_solution* ode_solve( vector *(equation)(double, vector*),ode_solution *parameters,double);

void ode_test(void);

#endif/*_ODE_H*/
