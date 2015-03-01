/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "arguments.h"
#include "domain.h"
#include "magnetic_field.h"
#include "readfile.h"

int main(int argc, char *argv[]) {
	arguments *args = parse_args(argc, argv);

	printf("domain   = %s\n", args->domain_file);
	printf("magfield = %s\n", args->magfield_file);
	printf("t0       = %f\n", args->tstart);
	printf("tend     = %f\n", args->tend);
	printf("r0       = (%f, %f, %f)\n", args->r0[0], args->r0[1], args->r0[2]);
	printf("v0       = (%f, %f, %f)\n", args->v0[0], args->v0[1], args->v0[2]);
	 
	return 0;
}
