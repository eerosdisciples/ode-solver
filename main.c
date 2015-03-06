/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ode.h"


#include "arguments.h"
#include "domain.h"
#include "magnetic_field.h"
#include "ode.h"
#include "readfile.h"

vector *dummy_eq(double t, vector *z) {
	return NULL;
}

int main(int argc, char *argv[]) {

	/* Solve */
	arguments *args;
	domain *dom;
	magnetic_field *mf;
	vector *solution;
	ode_solution *solvobj;
	unsigned int i;

	args = parse_args(argc, argv);
	return 0;

	/* Load domain */
	dom = domain_load(args->domain_file);
	/* Load magnetic field */
	mf  = magnetic_field_load(args->magfield_file);

	/* Solve */
	solution = malloc(sizeof(vector)*args->points);
	solvobj  = malloc(sizeof(ode_solution));
	solvobj->step = 0.1;	/* Initial step size */
	double *t = malloc(sizeof(double)*args->points);
	t[0] = args->tstart;

	for (i = 0; i < args->points-1; i++) {
		solvobj->Z = solution+i;
		do {
			t[i+1] = t[i] + solvobj->step;
			ode_solve(dummy_eq, solvobj, t[i]);
		} while (!solvobj->flag);
	}

	/* Output data */
	ode_test();
	return 0;
}
