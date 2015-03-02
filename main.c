/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "domain.h"
#include "magnetic_field.h"
#include "readfile.h"

int main(int argc, char *argv[]) {
	arguments *args;
	domain *dom;
	magnetic_field *mf;

	args = parse_args(argc, argv);

	/* Load domain */
	dom = domain_load(args->domain_file);
	/* Load magnetic field */
	mf  = magnetic_field_load(args->magfield_file);

	/* Solve */

	return 0;
}
