#ifndef _MAGNETIC_FIELD_H
#define _MAGNETIC_FIELD_H

#include "vector.h"
#include <stdio.h>

/* The magnetic field type */
typedef struct {
  double *B_r, *B_phi, *B_z;	/* Magnetic field components */
  double rmin, rmax;			/* min(r), max(r) */
  double zmin, zmax;			/* min(z), max(z) */
  unsigned int nr, nz;		/* Number of R and z points */
  double *r_grid;               /* grid points in r-direction */
  double *z_grid;               /* grid points in z-direction */
  } magnetic_field;

/* read magnetic field data file */
void magnetic_field_read_data(double*, FILE*, unsigned int);
/* create grid of field points */
double* magnetic_field_create_grid(unsigned int, double, double);
/* creates magnetic_field object, calls the two previous functions. */
magnetic_field* magnetic_field_load(char*);
/* Returns the magnetic field strength in a point of cartesian coordinates */
vector* magnetic_field_get(magnetic_field*, vector*);

#endif/*_MAGNETIC_FIELD_H*/
