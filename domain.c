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
 * r: Radial coordinate
 * z: Z-coordinate
 *
 * RETURNS one of DOMAIN_WITHIN, DOMAIN_ONDOMAIN or
 * DOMAIN_OUTSIDE depending on wether the given point
 * is inside, on the border of or outside the given
 * domain
 */
int domain_check(domain *d, double *r, double* z) { 
		double det;/* Determinant */
	
	/* Variables used in method */
	double x00=r[0];
	double x10=r[1];
	double y00=z[0];
	double y10=z[1];
	double x01;
	double y01;
	double x11;
	double y11;
	double s;
	double t;
	/* Used in loop */  
	int i;
	
	/* Check if matrix is zero */
	if (x00-x10==0 && y00-y10==0)
		return DOMAIN_OUTSIDE;

	for (i=0;i<d->n;i++){
		x01=d->r[i];
		x11=d->r[i+1];
		y01=d->z[i];
		y11=d->z[i+1];
		
		/* Calculates the determinant */
		det=x11*y01-x01*y11;
		
		/* Check if determinant is zero */
		if (det==0)
		return DOMAIN_WITHIN;
		
		/* Calculates s and t */
		s=1/det*((x00-x10)*y01-(y00-y10)*x01);
		t=1/det*(-(-x00+x10)*y11+(y00-y10)*x11);
		
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
	domain *d = domain_load("iter.wall_2d");
	printf("%d\n", d->n);
}

