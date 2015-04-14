/* Equation of Guiding-Center motion */

#include <math.h>
#include <stdlib.h>
#include "diff.h"
#include "equation_GCM.h"
#include "global.h"
#include "interp2.h"
#include "IO_data.h"
#include "magnetic_field.h"
#include "quantities.h"
#include "readfile.h"
#include "rkf45.h"
#include "vector.h"


/* Indices to the quantity array. Initialized
 * in `equation_GCM_init'. */
int GCM_QUANTITY_MU, GCM_QUANTITY_ENERGY, GCM_QUANTITY_XI;
initial_data *INITIAL;

/**
 * Function to initialize data for the GCM
 * Input: (initial_data object. men njä
 * Output: ode_solution pointer
 *
 * kommentera mera
 */
ode_solution* equation_GCM_init(vector *solution, initial_data *initial) {
  INITIAL = initial;
  double m=initial->mass; // particle mass
  double e = initial->charge; // particle charge

  /* Define the additional quantities we will
   * calculate during simulation */
  GCM_QUANTITY_ENERGY = quantities_define("Energy");
  GCM_QUANTITY_XI = quantities_define("Xi");
 
  /* initial particle position */
  double x = initial->x0,
    y = initial->y0,
    z = initial->z0;
  /* Create initial particle position vector, r */
  vector *r=vinit(3,x,y,z);
  
  /* initial particle velocity */
  double vx = initial->vx0,
    vy = initial->vy0,
    vz = initial->vz0;
  /* Create initial velocity vector  */
  vector *v=vinit(3,vx,vy,vz);		   		   

  /* Get magnetic field B for initial position */
  vector *B = magnetic_field_get(r);
		   
  double Bx=B->val[0],   
    By=B->val[1],
    Bz=B->val[2];	   
  /* Absolute value of B */
  double B_abs=sqrt(Bx*Bx + By*By + Bz*Bz);
		   
  /* Calculate bhat, unit vector in B-field direction */
  vector *bhat=vmulsf(1/B_abs,B);
  double bx=bhat->val[0],
    by=bhat->val[1],
    bz=bhat->val[2];
		   
  /* Calculate absolute value of parallel velocity */
  double vpar_abs=vdot(bhat,v);
			   
  /* negative parallel velocity vector, to calculate
   * perpendicular velocity vector */
  vector *bhat_vpar = vmuls(-vpar_abs,bhat);
  /*  perpendicular velocity */
  vector *vperp= vadd(v,bhat_vpar);
  double vperp_x=vperp->val[0], vperp_y=vperp->val[1], vperp_z=vperp->val[2];
	
  /* Allocate and calculate Larmor vector rho */
  vector *rho=vnew(3);
  rho->val[0]=by*vperp_z-bz*vperp_y;
  rho->val[1]=bz*vperp_x-bx*vperp_z;
  rho->val[2]=bx*vperp_y-by*vperp_x;
  double Omega = e*B_abs/m; //angular velocity Omega (no 'c' in SI-units) 
  vmulsf(-1/Omega, rho);

  /* Calculate guiding center position vector X */
  vector *X=vaddf(r,rho);

  /* Calculate magnetic moment mu */
  double mu=m*vdot(vperp,vperp)/(2*B_abs);
  
  /* store initial values to solution vector */
  solution->val[0] = vpar_abs; 
  solution->val[1] = X->val[0]; // Guiding center x-position
  solution->val[2] = X->val[1]; // Guiding center y-position
  solution->val[3] = X->val[2]; // Guiding center z-position 
  solution->val[4] = mu;        // Magnetic moment mu
			  
  /* solver_object of type ode_solution to be returned,
   * contains solution points, optimal
   * step size, and flag indicating ok step. */
  ode_solution *solver_object;
  solver_object = malloc(sizeof(ode_solution));
			 
  /* Save initial data in solver object */
  solver_object->step = 1e-8; /* Initial step size */
  solver_object->flag=0;
  solver_object->Z=solution;

  /* Calculate energy */
  double v2 = (vx*vx + vy*vy + vz*vz);
  double E = (m/2*v2)*ENERGY;
  quantities_report(GCM_QUANTITY_ENERGY, E);

  /* Calculate Xi */
  double xi = vpar_abs / sqrt(v2);
  quantities_report(GCM_QUANTITY_XI, xi);

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
  initial_data *initial;
  initial = INITIAL;
  double m = initial->mass;
  double e = initial->charge;
	
  vector *value = vnew(5);
  vector *xyz = vinit(3, Z->val[1], Z->val[2], Z->val[3]);
  double udot, Xdot1, Xdot2, Xdot3, mu;

  mu = Z->val[4];

  /***************************
   * Calculate udot          *
   ***************************/
  diff_data *dd = diff(xyz);

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

  /* Report mu */
  quantities_report(GCM_QUANTITY_MU, mu);
  /* Calculate energy */
  double v2 = Z->val[0]*Z->val[0] + 2*dd->Babs*mu/m;
  double E = (m/2*v2)*ENERGY;
  quantities_report(GCM_QUANTITY_ENERGY, E);
  /* Calculate Xi */
  double xi = Z->val[0] / sqrt(v2);
  quantities_report(GCM_QUANTITY_XI, xi);

  vfree(xyz);
  vfree(bhat);
  vfree(dd->curlB);
  vfree(dd->gradB);
  vfree(dd->B);
  free(dd);

  return value;
}
