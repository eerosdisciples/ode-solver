/* Equation of Guiding-Center motion */

#include <math.h>
#include <stdlib.h>
#include "differentiate.h"
#include "equation_predprey.h"
#include "interp2.h"
#include "IO_data.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "rkf45.h"
#include "vector.h"

/* Function to initialize data for the GCM
 * Input: (initial_data object
 * Output: ode_solution pointer
 */

ode_solution* equation_GCM_init(initial_data *initial, vector *solution){
	/* Calculate mu */
	double m=initial->mass;
	double vx = initial->vx0,
	       vy = initial->vy0,
	       vz = initial->vz0;
		   
	double x = initial->x0,
		   y = initial->y0,
		   z = initial->z0;
	double e = initial->charge;
		   		   
	/* Create x-vector */
	vector *xyz=vinit(3,x,y,z);
	/* Create velocity vector  */
	vector *v=vinit(3,vx,vy,vz);

	/* Get magnetic field */
	vector *B = magnetic_field_get(xyz);
		   
	double Bx=B->val[0],   
		   By=B->val[1],
		   Bz=B->val[2];
		   
	/* Absolute value of B */
	double B_abs=sqrt(Bx*Bx + By*By + Bz*Bz);
		   
	/* Calculate bhat */
	vector *bhat=vmuls(1/B_abs,B);
	double bx=bhat->val[0],
		   by=bhat->val[1],
		   bz=bhat->val[2];
		   
	/* Calculate absolute value paralell velocity */
	double vpar_abs=vdot(bhat,v);
			   
	/* Calculate absolute value of perpendicular velocity */
	//double vperp_abs= sqrt(vdot(v,v)-vpar*vpar);
					  
	/* XXX Memory leak here!  */
	vector * w=vadd(v,vmuls(-vpar_abs,bhat));
	double wx=w->val[0], wy=w->val[1], wz=w->val[2];
	/* Calculate mu */
	double mu=m/(2*B_abs)*(wx*wx + wy*wy + wz*wz);
//	mu = 9.9896e-14;

	double omega = e*B_abs/m;
	
	/* Allocate and  calculate rho */
	vector *rho=vnew(3);
	rho->val[0]=by*wz-bz*wy;
	rho->val[1]=bz*wx-bx*wz;
	rho->val[2]=bx*wy-by*wx;
	vmulsf(-1/omega, rho);

	/* Calculate position vector X */
	vector *X=vadd(xyz,rho);
	//vector *X = xyz;
			  
	/* solver_object of type ode_solution contains solution points, optimal
	 * step size, and flag indicating ok step. */
	ode_solution *solver_object;
	solver_object = malloc(sizeof(ode_solution));
			 
	/* Save data in solver object */
	solver_object->step = 1e-8; /* Initial step size */
	solver_object->flag=0;
	solver_object->Z=solution;
	
	solution->val[0] = vdot(bhat, v);
//	solution->val[0] = 6.4970e6;
	solution->val[1] = X->val[0];
	solution->val[2] = X->val[1];
	solution->val[3] = X->val[2];
	solution->val[4] = mu;
 
	return solver_object;
}

vector *equation_GCM(double T, vector *Z) {
	vector *value = vnew(5);
	vector *xyz = vinit(3, Z->val[1], Z->val[2], Z->val[3]);
	double udot, Xdot1, Xdot2, Xdot3, mu,
		   m = initial->mass,
		   e = initial->charge;

	mu = Z->val[4];

	/***************************
	 * Calculate udot          *
	 ***************************/
	/* Calculate B and b^ */
	vector *B = magnetic_field_get(xyz);
	vector *bhat = vmuls(1/sqrt(vdot(B, B)), B);

	/* Calculate grad B and rot b^ */
	vector *rotBhat = differentiate_rotBhat(xyz);
	vector *gradB = differentiate_gradB(xyz);

	/* Calculate B* (effective B-field) */
	//vector *B_eff = vaddf(B, vmulsf(m/e*Z->val[0], rotBhat));
	vector *B_eff = B;

	/* Get value of B* parallel to b^ */
	double Beff_par = vdot(B_eff, bhat);

	/* == > */
	udot = -mu/(m*Beff_par) * vdot(gradB, B_eff);

	/***************************
	 * Calculate Xdot1         *
	 ***************************/
	Xdot1 = 1/Beff_par * (-mu/e*(gradB->val[1]*bhat->val[2] - gradB->val[2]*bhat->val[1]) + Z->val[0] * B_eff->val[0]);
	/***************************
	 * Calculate Xdot2         *
	 ***************************/
	Xdot2 = 1/Beff_par * (-mu/e*(gradB->val[2]*bhat->val[0] - gradB->val[0]*bhat->val[2]) + Z->val[0] * B_eff->val[1]);
	/***************************
	 * Calculate Xdot3         *
	 ***************************/
	Xdot3 = 1/Beff_par * (-mu/e*(gradB->val[0]*bhat->val[1] - gradB->val[1]*bhat->val[0]) + Z->val[0] * B_eff->val[2]);

	value->val[0] = udot;
	value->val[1] = Xdot1;
	value->val[2] = Xdot2;
	value->val[3] = Xdot3;
	value->val[4] = mu;

	vfree(xyz);
	vfree(bhat);
	vfree(rotBhat);
	vfree(gradB);
	vfree(B_eff);

	return value;
}
