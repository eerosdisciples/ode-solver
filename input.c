/* Input reader */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "readfile.h"

arguments *input_read(char *filename) {
	arguments *args;
	char *buf;
	FILE *f;
	f = fopen(filename, "r");

	if (!f) {
		perror("ERROR");
		fprintf(stderr, "ERROR: Unable to read file `%s'\n!", filename);
		exit(EXIT_FAILURE);
	}

	/* Initialize args */
	args = malloc(sizeof(arguments));
	args->r0 = args->v0 = NULL;
	args->magfield_file = args->domain_file = NULL;
	args->particle_mass = 0;
	args->points = 1000;
	args->particle_charge = 0;

	int line = 1;

	while (!feof(f)) {
		/* Read name of variable */
		buf = readfile_word(f);

		if (*buf == 0)
			break;

		/* Interpret value */
		if (!strcmp(buf, "t0")) {
			buf = readfile_word(f);
			args->tstart = atof(buf);
		} else if (!strcmp(buf, "npoints")) {
			buf = readfile_word(f);
			args->points = atoi(buf);
		} else if (!strcmp(buf, "r0")) {
			buf = readfile_word(f);
			args->r0 = atodp3(buf);
		} else if (!strcmp(buf, "v0")) {
			buf = readfile_word(f);
			args->v0 = atodp3(buf);
		} else if (!strcmp(buf, "magnetic_field")) {
			buf = readfile_word(f);
			args->magfield_file = malloc(strlen(buf)+1);
			strcpy(args->magfield_file, buf);
		} else if (!strcmp(buf, "domain_file")) {
			buf = readfile_word(f);
			args->domain_file = malloc(strlen(buf)+1);
			strcpy(args->domain_file, buf);
		} else if (!strcmp(buf, "mass")) {
			buf = readfile_word(f);
			args->particle_mass = atof(buf);
		} else if (!strcmp(buf, "charge")) {
			buf = readfile_word(f);
			args->particle_charge = atof(buf);
		} else {
			fprintf(stderr, "ERROR: Invalid syntax in input, line %d\n", line);
			exit(EXIT_FAILURE);
		}
	}

	fclose(f);

	/* Verify that required arguments are set and that they're valid */
	if (args->r0 == NULL)
		fprintf(stderr, "ERROR: Initial position must be set!\n");
	else if (args->v0 == NULL)
		fprintf(stderr, "ERROR: Initial velocity must be set!\n");
	else if (args->magfield_file == NULL)
		fprintf(stderr, "ERROR: Name of magnetic field file must be set!\n");
	else if (args->domain_file == NULL)
		fprintf(stderr, "ERROR: Name of domain file must be set!\n");
	else if (args->particle_mass <= 0)
		fprintf(stderr, "ERROR: Particle mass must be positive! (mass > 0)\n");
	else
		return args;	/* Everything's cool, move along */
	
	/* Ending up here means an argument was not given or
	 * was of an invalid format. Die. */
	exit(EXIT_FAILURE);
}

void input_test(void) {
	arguments *args;
	args = input_read("testinput.input");

	printf("ARGUMENTS READ:\n\n");

	printf("t0 = %f\n", args->tstart);
	printf("npoints = %d\n", args->points);
	printf("r0 = %f, %f, %f\n", args->r0[0], args->r0[1], args->r0[2]);
	printf("v0 = %f, %f, %f\n", args->v0[0], args->v0[1], args->v0[2]);
	printf("magnetic_field = %s\n", args->magfield_file);
	printf("domain_file = %s\n", args->domain_file);
	printf("mass = %f\n", args->particle_mass);
}
