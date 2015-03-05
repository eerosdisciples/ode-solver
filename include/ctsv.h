#ifndef _CTSV_H
#define _CTSV_H

#include "vector.h"

typedef struct {
	vector *v;		/* List of values to output */
	char **labels;	/* List of labels belonging to vectors `v' */
	unsigned int n;	/* Number of vectors (= number of labels) */
} ctsv_input;

/* Write data to CSV or TSV file */
void ctsv_write(char*, char, ctsv_input*);

/* Test function for this module */
void ctsv_test(void);

#endif/*_CTSV_H*/
