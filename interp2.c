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
interp2d_spline *Br;
interp2d_spline *Bphi;
interp2d_spline *Bz;

double rmin, rmax, zmin, zmax;
/*
 * Initializatlize magnetic_field for interpolation.
 * This is to prepare GSL for what's to come.
 * Must be called before interpolation!
 */
void interp2_init_interpolation(magnetic_field *B) {
  size_t r_size = B->nr;
  size_t z_size = B->nz;
  /* Prepare the accelerators for both r and z */
  ra = gsl_interp_accel_alloc();
  za = gsl_interp_accel_alloc();
  /* Create interpolation objects */
  Br = interp2d_spline_alloc(interp2d_bicubic, z_size, r_size);
  Bphi = interp2d_spline_alloc(interp2d_bicubic, z_size, r_size);
  Bz = interp2d_spline_alloc(interp2d_bicubic, z_size, r_size);
  /* Prepare the interpolation objects for our situation */
  /* B_r */
  interp2d_spline_init(Br, B->z_grid, B->r_grid, B->B_r, z_size, r_size);
  /* B_phi */
  interp2d_spline_init(Bphi, B->z_grid, B->r_grid, B->B_phi, z_size, r_size);
  /* B_z */
  interp2d_spline_init(Bz, B->z_grid, B->r_grid, B->B_z, z_size, r_size);

  rmin = B->rmin;
  rmax = B->rmax;
  zmin = B->zmin;
  zmax = B->zmax;
}
/*
 * main interpolation function
 *
 * B: The magnetic field
 * xyz: The point (in cartesian coordinates) in which the field
 * strength should be evaluated.
 */
vector* interp2_interpolate(vector *xyz) {
  /*
   * cylindrical, for testing purposes
   double r = xyz->val[0]; 
   double z = xyz->val[1]; */
  
 	double x = xyz->val[0],
    y = xyz->val[1],
    z = xyz->val[2];
  /* Transform vector coordinates from cartesian to cylindrical */
	double  r = sqrt(x*x + y*y);
	/* Make sure the point is within our definied are */
	if (r < rmin)
		r = rmin;
	else if (r > rmax)
		r = rmax;
	if (z < zmin)
		z = zmin;
	else if (z > zmax)
		z = zmax;
  /*
   * Interpolate
   */
  double B_r_interp = interp2d_spline_eval(Br, z, r, za, ra);
  double B_phi_interp = interp2d_spline_eval(Bphi, z, r, za, ra);
  double B_z_interp = interp2d_spline_eval(Bz, z, r, za, ra);
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
