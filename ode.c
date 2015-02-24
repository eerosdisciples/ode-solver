/* ODE Solver */

#include <stdlib.h>
#include "ode.h"
#include "vector.h"

/**
 * Solve an Initial Value Problem (IVP ODE)
 *
 * equation: Pointer to function giving the equation,
 *   that is f in "z' = f(t,z)" where `t' is time
 *   (scalar) and `z' is the unknown (vector)
 * t0: Initial time
 * z0: Initial value
 * n:  Number of points to calculate
 *
 * RETURNS a solution to the equation
 */
vector *ode_solve(
	vector *(equation)(double, vector*),
	double t0, vector *z0, unsigned int n
) {
	return NULL;
}

/* Test function for this module */
void ode_test(void) {}

