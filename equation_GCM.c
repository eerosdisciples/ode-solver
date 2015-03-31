/* Equation of Guiding-Center motion */

#include <math.h>
#include <stdlib.h>
#include "diff.h"
#include "equation_GCM.h"
#include "interp2.h"
#include "IO_data.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "rkf45.h"
#include "vector.h"

/* We won't be needing this, trust me! */
//#define c 299792458 //  speed of light in m/s

/* Global variable containing particle initial values defined in main */
extern initial_data *initial;

/**
 * Function to initialize data for the GCM
 * Input: (initial_data object. men njä
 * Output: ode_solution pointer
 *
 * kommentera mera
 */
ode_solution* equation_GCM_init(vector *solution) {
	double m=initial->mass; // particle mass
	double e = initial->charge; // particle charge

	/* initial particle position */
	double x = initial->x0,
	  y = initial->y0,
	  z = initial->z0;

	/* initial particle velocity */
	double vx = initial->vx0,
	  vy = initial->vy0,
	  vz = initial->vz0;
		   		   
	/* Create particle position vector, r */
	vector *r=vinit(3,x,y,z);
	/* Create velocity vector  */
	vector *v=vinit(3,vx,vy,vz);

	/* Get magnetic field */
	vector *B = magnetic_field_get(r);
		   
	double Bx=B->val[0],   
		   By=B->val[1],
		   Bz=B->val[2];
		   
	/* Absolute value of B */
	double B_abs=sqrt(Bx*Bx + By*By + Bz*Bz);
		   
	/* Calculate bhat */
	vector *bhat=vmulsf(1/B_abs,B);
	double bx=bhat->val[0],
		   by=bhat->val[1],
		   bz=bhat->val[2];
		   
	/* Calculate absolute value of parallell velocity */
	double vpar_abs=vdot(bhat,v);
			   
	/*  perpendicular velocity */
	vector *bhat_vpar = vmuls(-vpar_abs,bhat);
    vector *vperp= vadd(v,bhat_vpar);
    double vperp_x=vperp->val[0], vperp_y=vperp->val[1], vperp_z=vperp->val[2];
        
	/* Calculate mu */
	double mu=m*vdot(vperp,vperp)/(2*B_abs);

    /* angular velocity Omega */
    /* And NO! There should NOT be a 'c' here!! */
	double Omega = e*B_abs/m;
	
	/* Allocate and calculate Larmour vector rho */
	vector *rho=vnew(3);
	rho->val[0]=by*vperp_z-bz*vperp_y;
	rho->val[1]=bz*vperp_x-bx*vperp_z;
	rho->val[2]=bx*vperp_y-by*vperp_x;
	vmulsf(-1/Omega, rho);

	/* Calculate guiding center position vector X */
	vector *X=vaddf(r,rho);
			  
	/* solver_object of type ode_solution contains solution points, optimal
	 * step size, and flag indicating ok step. */
	ode_solution *solver_object;
	solver_object = malloc(sizeof(ode_solution));
			 
	/* Save data in solver object */
	solver_object->step = 1e-8; /* Initial step size */
	solver_object->flag=0;
	solver_object->Z=solution;
	
	solution->val[0] = vdot(bhat, v);
	solution->val[1] = X->val[0];
	solution->val[2] = X->val[1];
	solution->val[3] = X->val[2];
	solution->val[4] = mu;

	/* Free up some memory */
	vfree(bhat_vpar);
	vfree(r);
	vfree(v);
	vfree(vperp);
	vfree(bhat);
	vfree(rho);
 
	return solver_object;
}

/**
 * COMMENT THIS FUNCTION!!
 * what does Z contain?? 
 */
vector *equation_GCM(double T, vector *Z) {
	double m = initial->mass;
	double e = initial->charge;
	
	vector *value = vnew(5);
	vector *xyz = vinit(3, Z->val[1], Z->val[2], Z->val[3]);
	double udot, Xdot1, Xdot2, Xdot3, mu;

	mu = Z->val[4];

	/***************************
	 * Calculate udot          *
	 ***************************/
	diff_data *dd = ndiff(xyz);

	/* Calculate bhat */
	vector *bhat = vmuls(1/dd->Babs, dd->B);

	/* Calculate B* (effective B-field) */
	vector *B_eff = vaddf(dd->B, vmulsf(m/e*Z->val[0], dd->curlB));

	/* Get value of B* parallel to b^ */
	double Beff_par = vdot(B_eff, bhat);

	/* == > */
	udot = -mu/(m*Beff_par) * vdot(dd->gradB, B_eff);

	/***************************
	 * Calculate Xdot1         *
	 ***************************/
	Xdot1 = 1/Beff_par * (-mu/e*(dd->gradB->val[1]*bhat->val[2] - dd->gradB->val[2]*bhat->val[1]) + Z->val[0] * B_eff->val[0]);
	/***************************
	 * Calculate Xdot2         *
	 ***************************/
	Xdot2 = 1/Beff_par * (-mu/e*(dd->gradB->val[2]*bhat->val[0] - dd->gradB->val[0]*bhat->val[2]) + Z->val[0] * B_eff->val[1]);
	/***************************
	 * Calculate Xdot3         *
	 ***************************/
	Xdot3 = 1/Beff_par * (-mu/e*(dd->gradB->val[0]*bhat->val[1] - dd->gradB->val[1]*bhat->val[0]) + Z->val[0] * B_eff->val[2]);

	value->val[0] = udot;
	value->val[1] = Xdot1;
	value->val[2] = Xdot2;
	value->val[3] = Xdot3;
	value->val[4] = 0;

	vfree(xyz);
	vfree(bhat);
	vfree(dd->curlB);
	vfree(dd->gradB);
	vfree(dd->B);
	free(dd);

	return value;
}
