#include <stdlib.h>
#include "vector.h"
#include "equation.h"
#include "magnetic_field.h"
#include "readfile.h"
#include "interp2.h"
#include <math.h>


/*Initializes interpolator
*/
void equation_init(void){
	// TODO Change for arbitrary parameters
    magnetic_field *B = magnetic_field_load("iter2d.bkg");
	 interp2_init_interpolation(B);	
}


/* Equation functions */



/*
 * Function for the Charged particle motion 
 *x
 * RETURNS: vector of values of function in point 'arguments'
 */

vector * equation_particle(double T, vector* Z){
	/*TODO Change from vector to particle + arbitrary parametrs m,e */
	/* Define needed parameters */
	double amu_to_kg=1.66053886*pow(10,-27);
	double ev=1.60217657*pow(10,-19);	
	double m=4*amu_to_kg;
	double e=2*ev;
	
		/* Save xyz coordinates */
	vector *xyz=vinit(3);
	xyz->val[0]=Z->val[0];
	xyz->val[1]=Z->val[1];
	xyz->val[2]=Z->val[2];
	
					   		   /* Save last 3 values of Z */
      	   double z4=Z->val[3],
		   	 	  z5=Z->val[4],
		          z6=Z->val[5];
				  printf(" Velocity is %f %f %f\n", z4,z5,z6);
				  printf(" Location is %f %f %f\n", xyz->val[0],xyz->val[1],xyz->val[2]);
				  
				    
				  vector *B = magnetic_field_get(xyz);
	
	/* Get value of field in each direction in point "coordinates" */
	
				/* Save each value of B */
				double B1=B->val[0],
					   B2=B->val[1],
					   B3=B->val[2];
					    printf(" Magnetic field is is %f %f %f\n", B1,B2,B3);
					   
		   /* Calculate each function value */
	 double f1=z4,
	 		f2=z5,
			f3=z6,
			f4=(e/m)*(z5*B3-z6*B2),
			f5=(e/m)*(z6*B1-z4*B3),
			f6=(e/m)*(z4*B2-z5*B3);
			printf("The difference is %f %f %f\n",f4/z4,f5/z5,f6/z6);			
		    printf(" Next speed is %f %f %f\n", f4,f5,f6);
			
			/* Save in vector and return */

			vector* value;
			value=vnew(6);
			
			value->val[0]=f1;
			value->val[1]=f2;
			value->val[2]=f3;
			value->val[3]=f4;
			value->val[4]=f5;
			value->val[5]=f6;
			
			return value; 
			
}
	
/*
 * Function for the Predator-Prey model
 *x:nbr of prey
 *y:nbr of predator
 * parameters:
 *       alpha:growth
 *		 beta:rate of predation upon the prey
 *      delta: growth of the predator population
 *		 gm  : loss rate of the predators due to either natural death or emigration.
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
