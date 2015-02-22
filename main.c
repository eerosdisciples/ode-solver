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

	/* Define matrix with arbitrary cartesian coords */
/*	vector carcoord=vinit(3,1.0,2.0,3.0);
	vector ccord=findcoords(carcoord);
		printf("r is %f, phi is %f,z is %f\n",ccord.val[0],ccord.val[1],ccord.val[2]);
	
	
		double *B_phi=load_magnetic_field(argv[1]);
		printf("first value in Bphi is %f\n",B_phi[1]);//	double Br[]=
	
	
			vector B=transform(carcoord ,B_phi,B_phi,B_phi);

			printf("Bx is %f, By is %f,Bz is %f\n",B.val[0],B.val[1],B.val[2]);
	*/