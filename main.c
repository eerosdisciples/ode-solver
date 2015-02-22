/* Test program */

#include <stdio.h>
#include "magnetic_field.h"
#include "vector.h"
#include "euler.h"

void printv(vector v) {
	printf("[%f %f %f]\n", v.val[0], v.val[1], v.val[2]);
}
int main(int argc, char *argv[]) {


		double *Br=load_magnetic_field(argv[1],1);
		double *Bphi=load_magnetic_field(argv[1],2);
		double *Bz=load_magnetic_field(argv[1],3);
	
		vector z0=vinit(3,1.0,1.0,1.0);
		int frac= 10;
		int tmax= 10;
	     eulermethod(frac,tmax,z0, Br,Bphi,Bz);
		

	return 0;
}

