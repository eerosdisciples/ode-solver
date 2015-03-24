#include <stdlib.h>
#include "vector.h"
#include "equation_predprey.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "interp2.h"
#include <math.h>
#include "IO_data.h"
#include "rkf45.h"


/* Function to initialize data for the GCM
 * Input: (initial_data object
 * Output: ode_solution pointer
 *
 *
 *
 *
 *
*/

ode_solution* predator_init(initial_data *initial){
	/* Calculate mu */
	double m=initial->mass;
	double vx = initial->vx0,
	       vy = initial->vy0,
	       vz = initial->vz0;
		   
	double x = initial->x0,
		   y = initial->y0,
		   z = initial->z0;
		   		   
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
		    	  by=bhat->val[0],
		          bz=bhat->val[0];
		   
		   
		   /* Calculate scalar product */
		 //  double bhat_bhat=vdot(bhat,bhat);
		   
		 //  double mu= m/2(*B_abs)
			   
			   /* Calculate absolute value paralell velocity */
			   
			double vpar_abs=vdot(bhat,v);
			   
			   
			   /* Calculate absolute value of perpendicular velocity */
   	    //  double vperp_abs= sqrt(vdot(v,v)-vpar*vpar);
					  
					  /*  */
		         vector * w=vadd(v,vmuls(-vpar_abs,bhat));
				 double wx=w->val[0],
				  	   	  wy=w->val[1],
				          wz=w->val[2];
			   
						  			   
			   /* Calculate mu */
	          double mu=m/(2*B_abs)*(vdot(w,w));
			 
	
	
			  /*Allocate and  calculate rho */
			  vector *rho=vnew(3);
			  rho->val[0]=by*wz-bz*wy;
			  rho->val[1]=bz*wx-bx*wz;
			  rho->val[2]=bx*wy-by*wx;
			
			/* Calculate position vector X */
			  vector *X=vadd(vmuls(-1,xyz),rho);
			  
		     /* solver_object of type ode_solution contains solution points, optimal
		      * step size, and flag indicating ok step. */
			  ode_solution *solver_object;
		     solver_object = malloc(sizeof(ode_solution));
			 
			 
			 /* Save data in solver object */
		     solver_object->step = 1e-10; /* Initial step size */
			 solver_object->flag=0;
			 solver_object->Z=X;
			 
			return solver_object;
	
}





/**
 * Function for the Predator-Prey model
 * x: number of prey
 * y: number of predator
 * 
 * alpha:growth
 * beta:rate of predation upon the prey
 * delta: growth of the predator population
 * gm : loss rate of the predators due to either natural death or emigration.
 *
 * RETURNS: vector of values of function in point 'arguments'
 */
vector * equation_predator_prey(double T,vector* Z){
  /* Define parameters */
  /*From source: http://www-rohan.sdsu.edu/~jmahaffy/courses/f09/math636/lectures/lotka/qualde2.html */
  double alpha=0.4807;
  double beta=0.02482;
  double gm=0.9272;
  double delta=0.02756;
  /* Save values */
  double x=Z->val[0];
  double y=Z->val[1];
  /* To store output */
  vector* value;
  value=vnew(2);
  /* Calculate values */
  double fx=alpha*x-beta*x*y;
  double fy=delta*x*y-gm*y;
  value->val[0]=fx;
  value->val[1]=fy;
  return value;
}
