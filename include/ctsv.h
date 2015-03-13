#ifndef _CTSV_H
#define _CTSV_H

#include "vector.h"

//typedef struct {
//	double* t;				/* Time points */
//	vector *v;				/* List of values to output */
//	char **labels;			/* List of labels belonging to vectors `v' */
//	unsigned int points;	/* Number of points */
//	unsigned int nvars;		/* Number of variables (or columns) */
//} ctsv_input;


typedef struct {
	double* T; 				/* Time points */
	double* E;  			/* Energy */
	vector *v;				/* List of coordinates to output */
	char **labels;			/* List of labels */
	unsigned int points;	/* Number of points */
	unsigned int nvars;		/* Number of coordinate-variables (or columns) */
} ctsv_data;

/* Write data to CSV or TSV file */
void ctsv_write(char*, char, ctsv_data*);

/* Test function for this module */
void ctsv_test(void);

#endif/*_CTSV_H*/
