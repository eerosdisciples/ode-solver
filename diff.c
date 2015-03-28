/* Differentiation of the B-field */

#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>
#include <math.h>
#include <stdlib.h>
#include "diff.h"
#include "interp2.h"
#include "magnetic_field.h"
#include "vector.h"

double default_x, default_y, default_z;

vector *differentiate_get_B(double x, double y, double z) {
//	vector *xyz = vinit(3, x, y, z);
	vector xyz;
	xyz.val = (double[]){x,y,z};
	xyz.n = 3;
	vector *res = magnetic_field_get(&xyz);

//	vfree(xyz);
	return res;
}
/**
 * Get the n:th component of the B-field
 * with the m:th variable as THE variable.
 */
double Bn_var_m(double var, void *params) {
	int n = ((int*)params)[0];
	int m = ((int*)params)[1];
	vector *v;
	double r;
	
	     if (m == 0) v = differentiate_get_B(var, default_y, default_z);
	else if (m == 1) v = differentiate_get_B(default_x, var, default_z);
	else if (m == 2) v = differentiate_get_B(default_x, default_y, var);
	
	r = v->val[n];
	vfree(v);
	return r;
}

double dodiff(vector *xyz, int n, int m) {
	default_x = xyz->val[0];
	default_y = xyz->val[1];
	default_z = xyz->val[2];

	gsl_function F;
	F.function = Bn_var_m;
	F.params = (int[]){n,m};

	double r = 0.;
	double abserr = 0.;

	gsl_deriv_central(&F, xyz->val[m], 1e-8, &r, &abserr);
	return r;
}

diff_data * diff(vector *xyz) {
	/* Calculate Jacobian */
	double
		dBx_dx=dodiff(xyz,0,0),  dBx_dy=dodiff(xyz,0,1),  dBx_dz=dodiff(xyz,0,2),
		dBy_dx=dodiff(xyz,1,0),  dBy_dy=dodiff(xyz,1,1),  dBy_dz=dodiff(xyz,1,2),
		dBz_dx=dodiff(xyz,2,0),  dBz_dy=dodiff(xyz,2,1),  dBz_dz=dodiff(xyz,2,2);
	vector *B = magnetic_field_get(xyz);
	/* Quicker access to the components of B */
	double Bx = B->val[0], By = B->val[1], Bz = B->val[2];
	/* Absolute value of B, ||B|| */
	double Babs = sqrt(Bx*Bx + By*By + Bz*Bz);

	printf("Correct Jacobian:\n");
	printf(" / %10f    %10f    %10f  \\\n", dBx_dx, dBx_dy, dBx_dz);
	printf("|  %10f    %10f    %10f   |\n", dBy_dx, dBy_dy, dBy_dz);
	printf(" \\ %10f    %10f    %10f  /\n", dBz_dx, dBz_dy, dBz_dz);
	
	/* Calculate components of gradient (grad ||B||) */
	double
		gradBx = (Bx*dBx_dx + By*dBy_dx + Bz*dBz_dx) / Babs,
		gradBy = (Bx*dBx_dy + By*dBy_dy + Bz*dBz_dy) / Babs,
		gradBz = (Bx*dBx_dz + By*dBy_dz + Bz*dBz_dz) / Babs;
	/* Create gradient vector */
	vector *gradB = vinit(3, gradBx, gradBy, gradBz);

	/* Calculate curl bhat */
	double
		curlBx = (Babs*dBz_dy - Bz*gradBy - Babs*dBy_dz + By*gradBz) / (Babs*Babs),
		curlBy = (Babs*dBx_dz - Bx*gradBz - Babs*dBz_dx + Bz*gradBx) / (Babs*Babs),
		curlBz = (Babs*dBy_dx - By*gradBx - Babs*dBx_dy + Bx*gradBy) / (Babs*Babs);

	vector *curlB = vinit(3, curlBx, curlBy, curlBz);

	diff_data *dd = malloc(sizeof(diff_data));
	dd->B = B;
	dd->gradB = gradB;
	dd->curlB = curlB;
	dd->Babs = Babs;

	return dd;
}

void diff_test(void) {
	magnetic_field *mf = magnetic_field_load("iter2d.bkg");
	interp2_init_interpolation(mf);

	vector *xyz = vinit(3, 8, 0, 0.2);
	diff_data *dd = diff(xyz);

	//printf("gradB = %e, %e, %e\n", dd->gradB->val[0], dd->gradB->val[1], dd->gradB->val[2]);
	//printf("curlB = %e, %e, %e\n", dd->curlB->val[0], dd->curlB->val[1], dd->curlB->val[2]);
	//printf("------------------\n");

	/* Calculate gradient from jacobian */
	vector *B = magnetic_field_get(xyz);
	double **J = interp2_jacobian(xyz);
	//printf("dBx/dx = %e\n", J[0][0]);
	printf("Current Jacobian:\n");
	printf(" / %10f    %10f    %10f  \\\n", J[0][0], J[0][1], J[0][2]);
	printf("|  %10f    %10f    %10f   |\n",  J[1][0], J[1][1], J[1][2]);
	printf(" \\ %10f    %10f    %10f  /\n", J[2][0], J[2][1], J[2][2]);

	double Bx = B->val[0], By = B->val[1], Bz = B->val[2];
	double Babs = sqrt(Bx*Bx + By*By + Bz*Bz);
	double
		gradBx = (Bx*J[0][0] + By*J[0][1] + Bz*J[0][2]) / Babs,
		gradBy = (Bx*J[1][0] + By*J[1][1] + Bz*J[1][2]) / Babs,
		gradBz = (Bx*J[2][0] + By*J[2][1] + Bz*J[2][2]) / Babs;

	//printf("gradB = %e, %e, %e\n", gradBx, gradBy, gradBz);
}

