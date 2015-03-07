/* Interpolation of magnetic_field object
 * input given in cylindrical coordinates
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
#include <math.h>

/* GSL accelerators that help speed up interpolations */
gsl_interp_accel *ra, *za;
/* interp2d interpolation objects */
interp2d *interp;
interp2d_spline *interp_s;

/*
 * Initializatlize magnetic_field for interpolation.
 * This is to prepare GSL for what's to come.
 * Must be called before interpolation!
 */
void interp2_init_interpolation(magnetic_field *B) {

  size_t r_size  = B->nr;
  size_t z_size  = B->nz;

  /* Prepare the accelerators for both r and z */
  ra = gsl_interp_accel_alloc();
  za = gsl_interp_accel_alloc();
  /* Create interpolation objects */
  interp = interp2d_alloc(interp2d_bicubic, r_size, z_size);
  interp_s = interp2d_spline_alloc(interp2d_bicubic, r_size, z_size);
  /* Prepare the interpolation objects for our situation */
  /* B_r */
  interp2d_init(interp, B->r_grid, B->z_grid, B->B_r, r_size, z_size);
  interp2d_spline_init(interp_s, B->r_grid, B->z_grid, B->B_r, r_size, z_size);
  /* B_phi */
  interp2d_init(interp, B->r_grid, B->z_grid, B->B_phi, r_size, z_size);
  interp2d_spline_init(interp_s, B->r_grid, B->z_grid, B->B_phi, r_size, z_size);
  /* B_z */
  interp2d_init(interp, B->r_grid, B->z_grid, B->B_z, r_size, z_size);
  interp2d_spline_init(interp_s, B->r_grid, B->z_grid, B->B_z, r_size, z_size);
}
/* 
 * main interpolation function 
 *
 * B: The magnetic field
 * xyz: The point (in cartesian coordinates) in which the field
 * strength should be evaluated. 
 */
vector* interp2_interpolate(magnetic_field *B, vector *xyz) {

  double r,
    x = xyz->val[0],
    y = xyz->val[1],
    z = xyz->val[2];

  /* Transform vector coordinates from cartesian to cylindrical */
  r   = sqrt(x*x + y*y);
  /*
   * Interpolate 
   */
  double B_r_interp = interp2d_eval(interp, B->r_grid, B->z_grid, B->B_r, r, z, ra, za);

  double B_phi_interp = interp2d_eval(interp, B->r_grid, B->z_grid, B->B_phi, r, z, ra, za);

  double B_z_interp = interp2d_eval(interp, B->r_grid, B->z_grid, B->B_z, r, z, ra, za);
  /*
   * Transform field to cartesian coordinates
   */
  double sinp = y/r, cosp = x/r;
  double B_x_interp = B_r_interp * cosp - B_phi_interp * sinp;
  double B_y_interp = B_r_interp * sinp + B_phi_interp * cosp;
  /*
   * Store interpolation values in vector
   */
  vector *B_interp = vinit(3, B_x_interp, B_y_interp, B_z_interp);

  return B_interp;
}
