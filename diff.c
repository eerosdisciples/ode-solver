/* Differentiation of the B-field */

#include <gsl/gsl_math.h>
#include <gsl/gsl_deriv.h>
#include <math.h>
#include <stdlib.h>
#include "diff.h"
#include "interp2.h"
#include "magnetic_field.h"
#include "vector.h"

diff_data * diff(vector *xyz) {
	/* Calculate Jacobian */
	double **J = interp2_jacobian(xyz);
	double
		dBx_dx=J[0][0],  dBx_dy=J[0][1],  dBx_dz=J[0][2],
		dBy_dx=J[1][0],  dBy_dy=J[1][1],  dBy_dz=J[1][2],
		dBz_dx=J[2][0],  dBz_dy=J[2][1],  dBz_dz=J[2][2];

	vector *B = magnetic_field_get(xyz);
	/* Quicker access to the components of B */
	double Bx = B->val[0], By = B->val[1], Bz = B->val[2];
	/* Absolute value of B, ||B|| */
	double Babs = sqrt(Bx*Bx + By*By + Bz*Bz);

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

	vector *xyz = vinit(3, 8.0, 0.0, 0.2);

	//printf("gradB = %e, %e, %e\n", dd->gradB->val[0], dd->gradB->val[1], dd->gradB->val[2]);
	//printf("curlB = %e, %e, %e\n", dd->curlB->val[0], dd->curlB->val[1], dd->curlB->val[2]);
	//printf("------------------\n");

	/* Calculate gradient from jacobian */
	double **J = interp2_jacobian(xyz);
	//printf("dBx/dx = %e\n", J[0][0]);
	printf("Current Jacobian:\n");
	printf(" / %10f    %10f    %10f  \\\n", J[0][0], J[0][1], J[0][2]);
	printf("|  %10f    %10f    %10f   |\n",  J[1][0], J[1][1], J[1][2]);
	printf(" \\ %10f    %10f    %10f  /\n", J[2][0], J[2][1], J[2][2]);
}

