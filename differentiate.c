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
double differentiate_get_B(double x, double y, double z) {
	vector *xyz = vinit(3, x, y, z);
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
double B_var_x(double x, void *yz) {
	return differentiate_get_B(x, ((double*)yz)[0], ((double*)yz)[1]);
}
double B_var_y(double y, void *xz) {
	return differentiate_get_B(((double*)xz)[0], y, ((double*)xz)[1]);
}
double B_var_z(double z, void *xy) {
	return differentiate_get_B(((double*)xy)[0], ((double*)xy)[1], z);
}

/**
 * Calculate bhat
 */
vector *differentiate_Bhat(double x, double y, double z) {
	vector *xyz = vinit(3, x, y, z);
	vector *res = magnetic_field_get(xyz);

	double r = sqrt(
		res->val[0]*res->val[0]+
		res->val[1]*res->val[1]+
		res->val[2]*res->val[2]
	);

	vfree(xyz);
	res->val[0] /= r;
	res->val[1] /= r;
	res->val[2] /= r;

	return res;
}

/* Calculate the various Bhat components and
 * allow GSL to vary x, y, z one at a time
 */
double bhaty_var_x(double x, void *yz) {
	vector *res = differentiate_Bhat(x, ((double*)yz)[0], ((double*)yz)[1]);
	double by = res->val[0];
	vfree(res);
	return by;
}
double bhatz_var_x(double x, void *yz) {
	vector *res = differentiate_Bhat(x, ((double*)yz)[0], ((double*)yz)[1]);
	double bz = res->val[0];
	vfree(res);
	return bz;
}
double bhatx_var_y(double y, void *xz) {
	vector *res = differentiate_Bhat(((double*)xz)[0], y, ((double*)xz)[1]);
	double bx = res->val[0];
	vfree(res);
	return bx;
}
double bhatz_var_y(double y, void *xz) {
	vector *res = differentiate_Bhat(((double*)xz)[0], y, ((double*)xz)[1]);
	double bz = res->val[0];
	vfree(res);
	return bz;
}
double bhatx_var_z(double z, void *xy) {
	vector *res = differentiate_Bhat(((double*)xy)[0], ((double*)xy)[1], z);
	double bx = res->val[0];
	vfree(res);
	return bx;
}
double bhaty_var_z(double z, void *xy) {
	vector *res = differentiate_Bhat(((double*)xy)[0], ((double*)xy)[1], z);
	double by = res->val[0];
	vfree(res);
	return by;
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

vector *differentiate_rotBhat(vector *xyz) {
	gsl_function
		Fdbx_dy, Fdbx_dz,
		Fdby_dx, Fdby_dz,
		Fdbz_dx, Fdbz_dy;
	double abserr;
	double xy[2] = {xyz->val[0], xyz->val[1]};
	double xz[2] = {xyz->val[0], xyz->val[2]};
	double yz[2] = {xyz->val[1], xyz->val[2]};
	vector *res = vnew(3);

	Fdbx_dy.function = &bhatx_var_y;
	Fdbx_dy.params = xz;
	Fdbx_dz.function = &bhatx_var_z;
	Fdbx_dz.params = xy;
	Fdby_dx.function = &bhaty_var_x;
	Fdby_dx.params = yz;
	Fdby_dz.function = &bhaty_var_z;
	Fdby_dz.params = xy;
	Fdbz_dx.function = &bhatz_var_x;
	Fdbz_dx.params = yz;
	Fdbz_dy.function = &bhatz_var_y;
	Fdbz_dy.params = xz;

	double dbx_dy, dbx_dz,
		   dby_dx, dby_dz,
		   dbz_dx, dbz_dy;

	gsl_deriv_central(&Fdbx_dy, xyz->val[0], 1e-8, &dbx_dy, &abserr);
	gsl_deriv_central(&Fdbx_dz, xyz->val[0], 1e-8, &dbx_dz, &abserr);
	gsl_deriv_central(&Fdby_dx, xyz->val[1], 1e-8, &dby_dx, &abserr);
	gsl_deriv_central(&Fdby_dz, xyz->val[1], 1e-8, &dby_dz, &abserr);
	gsl_deriv_central(&Fdbz_dx, xyz->val[2], 1e-8, &dbz_dx, &abserr);
	gsl_deriv_central(&Fdbz_dy, xyz->val[2], 1e-8, &dbz_dy, &abserr);

	res->val[0] = dbz_dy - dby_dz;
	res->val[1] = dbx_dz - dbz_dx;
	res->val[2] = dby_dx - dbx_dy;

	return res;
}
