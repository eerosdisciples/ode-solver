/* CSV/TSV file writer */

#include <stdio.h>
#include <stdlib.h>
#include "ctsv.h"
#include "vector.h"

/**
 * Writes the given data structure to a CSV or TSV file.
 *
 * filename: Name of file to write data to
 * type: If 'c', use CSV format. If 't', use TSV format.
 * data: Data to write
 */
void ctsv_write(char *filename, char type, ctsv_data* data) {
	/**
	 * Test function for this module
	 */
	FILE *fp;
	fp = fopen(filename, "w");

	unsigned int i, j;

	/* Write labels */
	fprintf(fp, "T");
	for (i = 0; i < data->nvars; i++) {
		fprintf(fp, "%c%s", type, data->labels[i]);
	}
	fprintf(fp, "E\n");

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
	ctsv_data* inp = malloc(sizeof(ctsv_data));

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
	ctsv_write("test.csv", ',', inp);
	ctsv_write("test.tsv", '\t', inp);
}
