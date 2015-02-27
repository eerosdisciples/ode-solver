/* Magnetic field reader */

#include <errno.h>
#include <stdlib.h>
#include "magnetic_field.h"
#include "vector.h"
#include "readfile.h"


#define BUFFER_SIZE 1024
/* testa om gamla read_mf fungerar*/
char buffer[BUFFER_SIZE+1];


void read_mf(double *B, FILE *f) {
int i = 0;
/* While no empty line is encountered... */
while (!readfile_word(f)) {
/* Read magnetic field value */
B[i++] = atof(buffer);
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
   readfile_word(f); B->rmin = atof(buffer);/* rmin value */
   readfile_word(f); B->rmax = atof(buffer);/* rmax value */
   readfile_word(f); B->nr = atoi(buffer);/* nr value */
   /*
    *Read and discard next three words
    * "zmin", "zmax", "nz" 
    */
   readfile_word(f); readfile_word(f); readfile_word(f);
   /* Identify zmin, zmax, nz: next three words */
   readfile_word(f); B->zmin = atof(buffer);/* zmin value */
   readfile_word(f); B->zmax = atof(buffer);/* zmax value */
   readfile_word(f); B->nz = atoi(buffer);/* nz value */
   /* Calculate size of B-field matrix */
   int size = B->nr*B->nz;
   /* Allocate memory */
   B->B_r = malloc(size*sizeof(double));
   B->B_phi=malloc(size*sizeof(double));
   B->B_z = malloc(size*sizeof(double));
   /* Skip 2 lines */
   readfile_skip_lines(2, f);

   /* Read B_r */
read_mf(B->B_r, f);





  
}

/*
 * Calculates the magnetic field strength in a given point
 * `xyz'.
 *
 * B: The magnetic field
 * xyz: The point (in cartesian coordinates) in which the field
 *   strength should be evaluated.
 *
 * RETURNS the field strength at the given point in
 * cartesian coordinates
 */
vector* magnetic_field_get(magnetic_field *B, vector *xyz) {
	return NULL;
}

/**
 * Function for testing this module
 */
void magnetic_field_test(void) {}

