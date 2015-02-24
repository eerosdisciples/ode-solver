/* Magnetic field reader */

#include <stdlib.h>
#include "magnetic_field.h"
#include "vector.h"

/**
 * Load magnetic field data from the given file.
 *
 * filename: File to load magnetic field data from
 *
 * RETURNS a magnetic_field containing the loaded data
 */
magnetic_field *magnetic_field_load(char *filename) {
	return NULL;
}

/**
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
vector *magnetic_field_get(magnetic_field *B, vector *xyz) {
	return NULL;
}

/**
 * Function for testing this module
 */
void magnetic_field_test(void) {}

