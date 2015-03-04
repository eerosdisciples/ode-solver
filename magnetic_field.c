/* Magnetic field reader */

#include <errno.h>
#include <stdlib.h>
#include "magnetic_field.h"
#include "vector.h"
#include "readfile.h"
#include "interp2.h"

/* Read data from file
 * 
 * Called from magnetic_field_load
 */
void magnetic_field_read_data(double *B, FILE *f, unsigned int size) {
  unsigned int i;
  char *word = readfile_word(f);
  /* Read every value in matrix */
  for (i = 0; i < size; i++)  {
    /* Read magnetic field value */
    B[i] = atof(word);
    word =  readfile_word(f);
  }
}

/*
 * Load magnetic field data from the given file.
 *
 * filename: File to load magnetic field data from
 *
 * RETURNS a magnetic_field containing the loaded data
 */
magnetic_field* magnetic_field_load(char *filename) {

  FILE *f;
  magnetic_field *B=NULL;

  f = fopen(filename, "r");
  /* File error */
  if (!f) {
    perror("ERROR");
    fprintf(stderr, "Unable to open file: %s\n", filename);
    exit(EXIT_FAILURE);
  }

  B = malloc(sizeof(magnetic_field));
  /* Memory error */
  if (B == NULL) {
    fprintf(stderr, "ERROR: Memory error!\n");
    exit(EXIT_FAILURE);
  }

  /*
   *Read and discard first three words
   * "rmin", "rmax", "nr" 
   */
  readfile_word(f); readfile_word(f); readfile_word(f);
  /* Identify rmin, rmax, nr: next three words */
  B->rmin = atof(readfile_word(f));/* rmin value */
  B->rmax = atof(readfile_word(f));/* rmax value */
  B->nr = atoi(readfile_word(f));/* nr value */

  /*
   *Read and discard next three words
   * "zmin", "zmax", "nz" 
   */
  readfile_word(f); readfile_word(f); readfile_word(f);
  /* Identify zmin, zmax, nz: next three words */
  B->zmin = atof(readfile_word(f));/* zmin value */
  B->zmax = atof(readfile_word(f));/* zmax value */
  B->nz = atoi(readfile_word(f));/* nz value */
  /* Calculate size of B-field matrix */
  unsigned int size = B->nr*B->nz;
  /* Allocate memory */
  B->B_r = malloc(size*sizeof(double));
  B->B_phi=malloc(size*sizeof(double));
  B->B_z = malloc(size*sizeof(double));
  /* Skip 2 lines */
  readfile_skip_lines(2, f);

  /* Read B_r */
  magnetic_field_read_data(B->B_r, f, size);
  /* Skip 1 line */
  readfile_skip_lines(1, f);
  /* Read B_phi */
  magnetic_field_read_data(B->B_phi, f, size);
  /* Skip 1 line */
  readfile_skip_lines(1, f);
  /* Read B_z */
  magnetic_field_read_data(B->B_z, f, size);
  return B;

}

/*
 * WORK IN PROGRESS
 *
 * Calculates the magnetic field strength in a given point
 * `xyz'.
 *
 * B: The magnetic field
 * xyz: The point (in cartesian coordinates) in which the field
 *   strength should be evaluated. *CYLINDRICAL NOW*
 *
 * RETURNS the field strength at the given point in
 * cartesian coordinates *CYLINDRICAL NOW*
 */
vector* magnetic_field_get(magnetic_field *B, vector *xyz) {

  vector *B_interp = interp2_interpolate(B, xyz);
 
    return B_interp;
}

/**
 * Function for testing data reading
 */
void magnetic_field_test_read(void) {
#define SIZE 131840
  
  magnetic_field *B = magnetic_field_load("iter2d.bkg");
  printf("First B_r value should be -1.0482107, is %f\n", B->B_r[0]);
  printf("Last B_r value should be 0.07055279 %f\n", B->B_r[SIZE]);
  printf("First B_phi value should be -9.333124, is %f\n", B->B_phi[0]);
  printf("Last B_phi value should be 63.764371, is %f\n", B->B_phi[SIZE]);
  printf("First B_z value should be -0.60929124, is %f\n", B->B_z[0]);
  printf("Last B_z value should be 0.44897631, is %f\n", B->B_z[SIZE]);
  printf("nz should be 513, is %d.\n", B->nz);
  printf("nr should be 257, is %d.\n", B->nr);
 
  /*vector *xyz;
  vector *test;
  test = magnetic_field_get(B, xyz);*/
}

/*
 * Function for testing interpolation of magnetic field strength values,
 * testing exact points, the first points r = 3.5 and z = -5.5.
 * Currently using cylindrical coordinates.
 */
void magnetic_field_test_interp(void) {
  magnetic_field *B  = magnetic_field_load("iter2d.bkg");
  
  vector *xyz = vinit(3, 5, 0, -5.5);
  vector *B_interp = magnetic_field_get(B, xyz);

   /*printf("B_interp[0] = B_r in (r,z) = (3.5, -5.5): %f, should be -1.0482107.\n", B_interp->val[0]);
  printf("B_interp[1] = B_phi in (r,z) = (3.5, -5.5) : %f, should be -9.333124.\n",B_interp->val[1]);
  printf("B_interp[2] = B_z i in (r,z) = (3.5, -5.5): %f, is -0.60929124.\n",B_interp->val[2]);*/

   printf("B_interp[0] = B_r in (x,y,z) = (5, 0, -5.5): %f, should be -1.0482107.\n", B_interp->val[0]);
  printf("B_interp[1] = B_phi in (x,y,z) = (5, 0, -5.5) : %f, should be -9.333124.\n",B_interp->val[1]);
  printf("B_interp[2] = B_z i in (x,y,z) = (5, 0, -5.5): %f, is -0.60929124.\n",B_interp->val[2]);
}

