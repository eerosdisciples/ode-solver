/* Domain manager */

#include <errno.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "domain.h"
#include "readfile.h"

extern int errno;

/**
 * Read the numeric data from a domain file
 *
 * f: The C file object to read through
 * d: Domain object to load data into
 * n: Number of points to read
 * 
 * Called from domain_load
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
    /* Read Z value */
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
 * Check if a points lies within or outside of the domain.
 *
 * d: Domain for the problem
 * r: chage in Radial coordinate, two points
 * z: change in Z-coordinate, two points
 *
 * RETURNS DOMAIN_WITHIN (0) or
 * DOMAIN_OUTSIDE (1) depending on whether the given point
 * is inside or outside the given domain
 */
int domain_check(domain *d, double *r, double *z) { 
  /* Determinant */
  double det;
	
  /* Parametrization*/
	
  /* A point inside the contour */
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

  int i;	
  for (i=0;i < d->n-1; i++) {
    x10=d->r[i];
    x11=d->r[i+1]-x10;
    y10=d->z[i];
    y11=d->z[i+1]-y10;
	double tol=0;
    /* Check if matrix is zero */
    if (fabs(x00-x10)<=tol && fabs(y00-y10)<=tol)
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
   return DOMAIN_WITHIN;
}

/**
 * Function for testing the module
 */
void domain_test(void) {
  /* Check wether reading file is OK */
  domain *d = domain_load("iter.wall_2d");

  srand(time(NULL));
  int i = rand() % (d->n);

  printf("Number of points: %d\n", d->n);
  printf("First point:      i=0 , r=%f, z=%f\n", d->r[0], d->z[0]);
  printf("Random point:     i=%2d, r=%f, z=%f\n", i, d->r[i], d->z[i]);
  printf("Last point:       i=%2d, r=%f, z=%f\n", d->n-1, d->r[d->n-1], d->z[d->n-1]);
	
  /* Test points */
  double r[2]; 
  double z[2];
	
  /* To convert from int to certain message */
  char *location[]={"Inside","Outside"}; 
  /* Indicating the result */
  int is;
  /* Indicating what the result should be*/
  int should;
	
		
  /* TEST BEGINS */
  printf(" ********* TEST BEGINS ********\n");
	 
  /* TESTPOINT 1 */
  r[0]=4.79839;r[1]=6.92137;
  z[0]=1.78125; z[1]=0.4375;
  should=0;
	 
  is=domain_check(d, r, z);

  printf("Should be %s, is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;
  }
  printf("CORRECT!!!\n\n");
		
  /* TESTPOINT 2 */
  r[0]=6.03226;r[1]=6.47681;
  z[0]=-4.8125;z[1]=-0.78125;

  should=1;
  is=domain_check(d, r, z);

  printf("Should be %s, is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");

  /* TESTPOINT 3 */
  r[0]=6.07762;r[1]=6.64012;
  z[0]=4.125;z[1]=3.65625;
  should=0;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");			
			 
  /* TESTPOINT 4 */
  r[0]=7.91028;r[1]=7.21169;
  z[0]=-1.46875;z[1]=-1.78125;
  should=1;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");					

  /* TESTPOINT 5 */
  r[0]=4.16331;r[1]=4.3629;
  z[1]=-1.96875;z[1]=-3.75;
  should=1;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");	
					 
  /* TESTPOINT 6 */
  r[0]=5.44254; r[1]=5.59677;
  z[0]=-4.03125;z[1]=-4.71875;
  should=1;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");								 

  /* TESTPOINT 7 */
  r[0]=4.39012;r[1]=4.57157;
  z[0]=-2.3125;z[1]=3.3125;
  should=0;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");		
  
  /* TESTPOINT 8 */
  r[0]=7.88717;r[1]=7.99016;
  z[0]=-1.34802;z[1]=-1.16703;
  should=1;
  is=domain_check(d, r, z);

  printf("Should be %s is %s\n",location[should],location[is]);
  if (should!=is){
    printf("INCORRECT!!!\n"); return;}
  printf("CORRECT!!!\n\n");	
  						 					
  /* END OF TEST */	
  printf(" ********* END OF TEST ********\n");
				
  printf("TEST DONE, WELL DONE!\n");
}

