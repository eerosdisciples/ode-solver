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

/* Write data to CSV or TSV file */
void ctsv_write(char*, char, ctsv_data*);

/* Test function for this module */
void ctsv_test(void);

#endif/*_CTSV_H*/
