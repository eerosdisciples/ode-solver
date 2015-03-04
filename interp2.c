/* Interpolation WORK IN PROGRESS
 *
 * TODO: create function with r_grid, z_grid, B, and vector
 * xyz (cartesian coordinates) as argument. Call this from
 * magnetic_field_get in magnetic_field.c
 *
 * ALSO: make interp2.h file
 */

#include "magnetic_field.h"
#include "vector.h"
/* GNU Scientific Library */
#include <gsl/gsl_errno.h>
#include <gsl/gsl_ieee_utils.h>
/* Interp2d header */
#include <interp2d.h>
/* Interp2d spline support */
#include <interp2d_spline.h>


  /* GSL accelerators that help speed up interpolations */
  gsl_interp_accel *ra, *za;
  /* interp2d interpolation objects */
  interp2d *interp;
  interp2d_spline *interp_s;

/*
 * Create grid.
 * n: number of grid points
 * min: mimimum grid point value
 * max: maximum grid point value
 *
 * RETURNS pointer to grid
 */

double* interp2_create_grid(unsigned int n, double min, double max) {
  /* 
   * Step size between grid points 
   */
  double step = (max - min)/(n-1);
  /*
   * Initialize, allocate and set start point for grid
   */
  double *grid;
  grid = malloc(n);
  grid[0] = min;

  unsigned int i; // loop counter
  /*
   * Store grid points in grid array
   */
  for (i=1; i < n ; i++) {
    grid[i] = grid[i-1] + step;
  }
  return grid;
}

/* Initialization. This is to prepare GSL for what's to come */
void init(magnetic_field *B, double *r_grid, double *z_grid) {

  size_t r_size  = B->nr;
  size_t z_size  = B->nz;

  printf("z_size %d",z_size); // FELSÖKNING: r_size & z_size verkar ok

  /* Prepare the accelerators for both r and z */
  ra = gsl_interp_accel_alloc();
  za = gsl_interp_accel_alloc();
  /* Create interpolation objects */
  /* SEGFAULT FROM HERE */
  interp = interp2d_alloc(interp2d_bicubic, r_size, z_size);
  //interp_s = interp2d_spline_alloc(interp2d_bicubic, r_size, z_size);
  /* Prepare the interpolation objects for our situation */
  /* B_r */
  //interp2d_init(interp, r_grid, z_grid, B->B_r, r_size, z_size);
  //interp2d_spline_init(interp_s, r_grid, z_grid, B->B_r, r_size, z_size);
  /* B_phi */
  //interp2d_init(interp, r_grid, z_grid, B->B_phi, r_size, z_size);
  //interp2d_spline_init(interp_s, r_grid, z_grid, B->B_phi, r_size, z_size);
  /* B_z */
  //interp2d_init(interp, r_grid, z_grid, B->B_z, r_size, z_size);
  //interp2d_spline_init(interp_s, r_grid, z_grid, B->B_z, r_size, z_size);
}


/* 
 * main interpolation function 
 *
 * B: The magnetic field
 * xyz: The point (in cartesian coordinates) in which the field
 *   strength should be evaluated. *CYLINDRICAL NOW*
 */
vector* interp2_interpolate(magnetic_field *B, vector *xyz) {

  double *r_grid = interp2_create_grid(B->nr, B->rmin, B->rmax);
  double *z_grid = interp2_create_grid(B->nz, B->zmin, B->zmax);

  double testvar1 =1;
  double testvar2 = 2;
  double testvar3 = 3;
  
   init(B, r_grid, z_grid); // initialization
  /*
   * Interpolate 
   */
  //double B_r_interp = interp2d_eval(interp, r_grid, z_grid, B->B_r, xyz->val[0], xyz->val[1], ra, za);

  //double B_phi_interp = interp2d_eval(interp, r_grid, z_grid, B->B_phi, xyz->val[0], xyz->val[1], ra, za);

  //double B_z_interp = interp2d_eval(interp, r_grid, z_grid, B->B_z, xyz->val[0], xyz->val[1], ra, za);
  /*
   * Store interpolation values in vector
   */
vector *B_interp = vinit(3, testvar1, testvar2, testvar3);

  //vector *B_interp = vinit(3, B_r_interp, B_phi_interp, B_z_interp);

//printf("programmet körde ok. %f %f %f\n",B_interp->val[0],B_interp->val[1] ,B_interp->val[2]); // FOR TESTING PURPOSES



//printf("%d\n",B->nz);

  return B_interp;
}
