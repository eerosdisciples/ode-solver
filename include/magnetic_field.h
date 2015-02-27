#ifndef _MAGNETIC_FIELD_H
#define _MAGNETIC_FIELD_H

#include "vector.h"
#include <stdio.h>

/* The magnetic field type */
typedef struct {
	double *B_r, *B_phi, *B_z;	/* Magnetic field components */
	double rmin, rmax, nr;		/* min(r), max(r) and number of r points */
	double zmin, zmax, nz;		/* min(z), max(z) and number of z points */
} magnetic_field;

/* Load magnetic field data */
magnetic_field* magnetic_field_load(char*);
/* Get the value of the magnetic field at a certain point */
vector* magnetic_field_get(magnetic_field*, vector*);
/* Function for testing this module */
void magnetic_field_test(void);

void read_mf(double*,FILE*);

#endif/*_MAGNETIC_FIELD_H*/
