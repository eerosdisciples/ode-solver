/* Get bhat at given position */

#include <math.h>
#include <stdio.h>
#include "arguments.h"
#include "interp2.h"
#include "input.h"
#include "magnetic_field.h"
#include "vector.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		printf("ERROR: Expected position as argument! (x,y,z  -- no spaces)\n");
		return -1;
	}

	double *r = atodp3(argv[1]);
	vector *v = vinit(3, r[0], r[1], r[2]);

	magnetic_field *mf = magnetic_field_load("iter2d.bkg");
	interp2_init_interpolation(mf);

	vector *B = magnetic_field_get(v);
	double Babs = sqrt(vdot(B, B));

	printf("bhat=[%e, %e, %e];\n",B->val[0]/Babs,B->val[1]/Babs,B->val[2]/Babs);

	return 0;
}
