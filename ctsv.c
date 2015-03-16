/* CSV/TSV file writer */

#include <stdio.h>
#include <stdlib.h>
#include "arguments.h"
#include "ctsv.h"
#include "solution_data.h"
#include "vector.h"

/**
 * Writes the given data structure to a CSV or TSV file.
 *
 * filename: Name of file to write data to
 * type: If 'c', use CSV format. If 't', use TSV format.
 * data: Data to write
 * args: Input arguments passed to the program
 */
void ctsv_write(char *filename, char type, solution_data* data, arguments* args) {
	/**
	 * Test function for this module
	 */
	FILE *fp;
	fp = fopen(filename, "w");

	if (fp == NULL) {
		perror("ERROR");
		fprintf(stderr, "ERROR: Unable to read file: '%s'\n", filename);
		exit(EXIT_FAILURE);
	}

	unsigned int i, j;

	/* Print settings? */
	if (args != NULL && args->print_settings) {
		fprintf(fp, "t0=%e\n", args->tstart);
		fprintf(fp, "tend=%e\n", args->tend);
		fprintf(fp, "r0=%e,%e,%e\n", args->r0[0], args->r0[1], args->r0[2]);
		fprintf(fp, "v0=%e,%e,%e\n", args->v0[0], args->v0[1], args->v0[2]);
		fprintf(fp, "mass=%e\n", args->particle_mass);
		fprintf(fp, "charge=%e\n", args->particle_charge);

		/* Insert empty line */
		fprintf(fp, "\n");
	}

	/* Write labels */
	fprintf(fp, "T");
	for (i = 0; i < data->nvars; i++) {
		fprintf(fp, "%c%s", type, data->labels[i]);
	}
	fprintf(fp, ",E\n");

	/* Print data */
	for (i = 0; i < data->points; i++) {
		vector* v = (data->v)+i;
		double t = data->T[i];
		double E = data->E[i];

		/* Time coordinate */
		fprintf(fp, "%e", t);

		/* Print all elements in vector v */
		for (j = 0; j < data->nvars; j++) {
			fprintf(fp, "%c%e", type, v->val[j]);
		}

		/* Print energy */
		fprintf(fp, "%c%e\n", type, E);
	}

	fclose(fp);
}

void ctsv_test(void) {
	unsigned int i,j;
	/* Write different parameters */
	solution_data* inp = malloc(sizeof(solution_data));

	inp->points = 10;
	inp->nvars = 2;
	inp->labels = malloc(sizeof(char*)*inp->nvars);
	for (i = 0; i < inp->nvars; i++) {
		inp->labels[i] = malloc(2);
		inp->labels[i][0] = (char)('a'+i);
		inp->labels[i][1] = 0;
	}

	/* Initialize t */
	inp->T = malloc(sizeof(double)*inp->points);
	for (i = 0; i < inp->points; i++) {
		inp->T[i] = 0.1*i;
	}

	/* Initialize E */
	inp->E = malloc(sizeof(double)*inp->points);
	for (i = 0; i < inp->points; i++) {
		inp->E[i] = 3.14159*inp->T[i];
	}

	/* Initialize vector coordinates */
	inp->v = malloc(sizeof(vector)*inp->points);
	for (i = 0; i < inp->points; i++) {
		inp->v[i].val = malloc(sizeof(double)*inp->nvars);
		inp->v[i].n = inp->nvars;

		for (j = 0; j < inp->nvars; j++) {
			inp->v[i].val[j] = (double)(i + j);
		}
	}

	/* Write output to two different files */
	ctsv_write("test.csv", ',', inp, NULL);
	ctsv_write("test.tsv", '\t', inp, NULL);
}
