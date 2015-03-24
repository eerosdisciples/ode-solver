/* Differentiation of the B-field */

#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>
#include <math.h>
#include "magnetic_field.h"

/**
 * Calculate ||B|| and allow GSL to vary
 * the field in the x direction.
 *
 * x: Position in the X direction
 * yz: Double array with
 *    [0] = Position in Y direction
 *    [1] = Position in Z direction
 */
double B_var_x(double x, void *yz) {
	vector *xyz = vinit(3, x, ((double*)yz)[1], ((double*)yz)[2]);
	vector *res = magnetic_field_get(xyz);

	double r = sqrt(
		res->val[0]*res->val[0]+
		res->val[1]*res->val[1]+
		res->val[2]*res->val[2]
	);

	vfree(xyz);
	vfree(res);
	return r;
}
/**
 * Calculate ||B|| and allow GSL to vary
 * the field in the y direction.
 *
 * y: Position in the Y direction
 * xz: Double array with
 *    [0] = Position in X direction
 *    [1] = Position in Z direction
 */
double B_var_y(double y, void *xz) {
	vector *xyz = vinit(3, ((double*)xz)[0], y, ((double*)xz)[2]);
	vector *res = magnetic_field_get(xyz);

	double r = sqrt(
		res->val[0]*res->val[0]+
		res->val[1]*res->val[1]+
		res->val[2]*res->val[2]
	);

	vfree(xyz);
	vfree(res);
	return r;
}
/**
 * Calculate ||B|| and allow GSL to vary
 * the field in the z direction.
 *
 * z: Position in the Z direction
 * xy: Double array with
 *    [0] = Position in X direction
 *    [1] = Position in Y direction
 */
double B_var_z(double z, void *xy) {
	vector *xyz = vinit(3, ((double*)xy)[0], ((double*)xy)[1], z);
	vector *res = magnetic_field_get(xyz);

	double r = sqrt(
		res->val[0]*res->val[0]+
		res->val[1]*res->val[1]+
		res->val[2]*res->val[2]
	);

	vfree(xyz);
	vfree(res);
	return r;
}

/**
 * Calculate the gradient of the norm
 * of the B-field
 *
 * xyz: Position in which to evaluate grad B
 */
vector *differentiate_gradB(vector *xyz) {
	gsl_function Fx, Fy, Fz;
	double abserr;
	vector *res = vnew(3);

	/* Initialize arguments to GSL */
	Fx.function = &B_var_x;
	Fy.function = &B_var_y;
	Fz.function = &B_var_z;
	/* Set the 'default' x-y-z parameters */
	Fx.params = (double[]){xyz->val[1],xyz->val[2]};
	Fy.params = (double[]){xyz->val[0],xyz->val[2]};
	Fz.params = (double[]){xyz->val[0],xyz->val[1]};

	/* Carry out the differentiations */
	gsl_deriv_central(&Fx, xyz->val[0], 1e-8, res->val  , &abserr);
	gsl_deriv_central(&Fy, xyz->val[1], 1e-8, res->val+1, &abserr);
	gsl_deriv_central(&Fz, xyz->val[2], 1e-8, res->val+2, &abserr);

	return res;
}
