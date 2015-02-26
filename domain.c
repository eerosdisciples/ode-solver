/* Domain manager */

#include <errno.h>
#include <stdlib.h>
#include "domain.h"
#include "readfile.h"

extern int errno;

/**
 * Read the numeric data from a domain file
 *
 * f: The C file object to read through
 * d: Domain object to load data into
 * n: Number of points to read
 */
void domain_read_data(FILE *f, domain *d, unsigned int n) {
	unsigned int i;
	char *p;
	d->r = malloc(sizeof(double)*n);
	d->z = malloc(sizeof(double)*n);

	for (i = 0; i < n; i++) {
		/* Read R value */
		p = readfile_word(f);
		sscanf(p, "%lf", d->r+i);
		p = readfile_word(f);
		sscanf(p, "%lf", d->z+i);
		readfile_word(f);
	}
}

/**
 * Load a file containing data points giving
 * the domain of the problem.
 *
 * filename: Name of file to load domain from
 *
 * RETURNS the domain object loaded
 */
domain *domain_load(char *filename) {
	FILE *f;
	domain *d=NULL;

	f = fopen(filename, "r");
	/* File error */
	if (!f) {
		perror("ERROR");
		fprintf(stderr, "Unable to open file: %s\n", filename);
		exit(EXIT_FAILURE);
	}

	d = malloc(sizeof(domain));
	/* Memory error */
	if (d == NULL) {
		fprintf(stderr, "ERROR: Memory error!\n");
		exit(EXIT_FAILURE);
	}
	
	/* Skip first 2 lines */
	readfile_skip_lines(2, f);
	/* Get number of points */
	char *np = readfile_word(f);
	d->n = strtol(np, (char**)NULL, 10);

	/* Skip 2 lines */
	readfile_skip_lines(2, f);

	/* Read the actual data */
	domain_read_data(f, d, d->n);

	/* DONE */
	
	return d;
}


/**
 * Check if a point lies within, outside or
 * on the border of the domain.
 *
 * d: Domain for the problem
 * r: next Radial coordinate
 * z: next Z-coordinate
 *
 * RETURNS one of DOMAIN_WITHIN, DOMAIN_ONDOMAIN or
 * DOMAIN_OUTSIDE depending on wether the given point
 * is inside or outside the given
 * domain
 */
int domain_check(domain *d, double *r, double *z) { 
	//printf("JUST checking r is %f and z is %f",r ,z);
	/* Determinant */
	double det;
	
	/* Parametrization*/
	
	/* A point inside the conotur */
	double x00=r[0];
	double y00=z[0];
	
	double x01=r[1]-x00;
	double y01=z[1]-y00;
	
	double x10;
	double y10;
	double x11;
	double y11;
	double s;
	double t;
	/* Used in loop */  
	int i;
	
	/* Variable for counting each intersection */
	int count=0;

	for (i=0;i<d->n;i++){
		x10=d->r[i];
		x11=d->r[i+1]-x10;
		y10=d->z[i];
		y11=d->z[i+1]-y10;
				
	/* Check if matrix is zero */
		if (x00-x10==0 && y00-y10==0)
			return DOMAIN_OUTSIDE;
				
		/* Calculates the determinant */
		det=x11*y01-x01*y11;
				
		/* Check if determinant is zero */
		if (det==0)
		return DOMAIN_WITHIN;

		/* Calculates s and t */
		s=(1/det)*((x00-x10)*y01-(y00-y10)*x01);
		t=(1/det)*(-(-(x00-x10)*y11+(y00-y10)*x11));
				
		/* If s and t are between 0 and 1 => intersection */
		if (s>=0 && s<=1 && t>=0 && t<=1)
			return DOMAIN_OUTSIDE;
		
	}
	/* If nbr of intersections is odd, then the point is outside */
		
	return DOMAIN_WITHIN;
}

/**
 * Function for testing the module
 */
void domain_test(void) {
	
	/* Test points */
	double *r; 
	double *z;
	
	/* To convert from int to certain message */
	char *location[]={"Inside","Outside"}; 
	/* Indicating the result */
	int is;
	/* Indicating what the result should be*/
	int should;
	char* filename="iter.wall_2d";
	
	/* Read file */
// domain *d=domain_load(filename);
 
	
 /* TEST BEGINS */
	 printf(" ********* TEST BEGINS ********%\n");
	 
 /* TESTPOINT 1 */
	// r=10;
	 //z=10;
	 should=1;
	// is=domain_check(d, r, z);

	 printf("Should be %s is %s\n",location[should],location[is]);
		if (should!=is){
			printf("INCORRECT!!!\n"); return;}
			printf("CORRECT!!!\n\n");
		
 /* TESTPOINT 2 */
	//   r=5;
	  // z=2;
	   should=0;
	//   is=domain_check(d, r, z);

	  printf("Should be %s is %s\n",location[should],location[is]);
	   	if (should!=is){
	   		printf("INCORRECT!!!\n"); return;}
			printf("CORRECT!!!\n\n");

 /* TESTPOINT 3 */
	 	// r=5.56956;
	     //z=-5.65625;
    	 should=1;
	// 	 is=domain_check(d, r, z);

	  printf("Should be %s is %s\n",location[should],location[is]);
	     if (should!=is){
	 	   	 printf("INCORRECT!!!\n"); return;}
	 		 printf("CORRECT!!!\n\n");			
			 
	 /* TESTPOINT 4 */
		 	// r=4.2815;
		     //z=-3.8125;
	    	 should=0;
	//	 	 is=domain_check(d, r, z);

		  printf("Should be %s is %s\n",location[should],location[is]);
		     if (should!=is){
		 	   	 printf("INCORRECT!!!\n"); return;}
		 		 printf("CORRECT!!!\n\n");					

		 /* TESTPOINT 5 */
			 	// r=6.27722;
			     //z=-3.1875;
		    	 should=0;
		//	 	 is=domain_check(d, r, z);

			  printf("Should be %s is %s\n",location[should],location[is]);
			     if (should!=is){
			 	   	 printf("INCORRECT!!!\n"); return;}
			 		 printf("CORRECT!!!\n\n");	
					 
			 /* TESTPOINT 6 */
				// 	 r=7.56552;
				  //   z=2.84375;
			    	 should=1;
			//	 	 is=domain_check(d, r, z);

				  printf("Should be %s is %s\n",location[should],location[is]);
				     if (should!=is){
				 	   	 printf("INCORRECT!!!\n"); return;}
				 		 printf("CORRECT!!!\n\n");								 

				 /* TESTPOINT 7 */
				//	 	 r=4.19052;
				//	     z=-3.125;
				    	 should=1;
			//		 	 is=domain_check(d, r, z);

					  printf("Should be %s is %s\n",location[should],location[is]);
					     if (should!=is){
					 	   	 printf("INCORRECT!!!\n"); return;}
					 		 printf("CORRECT!!!\n\n");								 					
 /* END OF TEST */	
	   	 printf(" ********* END OF TEST ********%\n");
				
		 printf("TEST DONE, WELL DONE!\n");
		
}

