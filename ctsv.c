/* CSV/TSV file writer */

#include "ctsv.h"
#include <stdio.h>
#include <stdlib.h>
#include "vector.h"

/**
 * Writes the given data structure to a CSV or TSV file.
 *
 * filename: Name of file to write data to
 * type: If 'c', use CSV format. If 't', use TSV format.
 * data: Data to write
 */
void ctsv_write(char *filename, char type, ctsv_input *data) {
	/**
	 * Test function for this module
	 */
	FILE *fp;
	fp = fopen(filename, "w");

	unsigned int i, j;

	fprintf(fp, "T");
	/* write labels */
	for (i = 0; i < data->nvars; i++) {
		fprintf(fp, "%c%s", type, data->labels[i]);
	}

	/* Newline */
	fprintf(fp,"\n");

	/* Print data */
	for (i = 0; i < data->points; i++) {
		vector* v = (data->v)+i;
		double t = data->t[i];

		fprintf(fp, "%f", t);

		/*Print all elements in vector i */
		for (j = 0; j < data->nvars; j++) {
			fprintf(fp, "%c%f", type, v->val[j]);
		}
	
		/* Newline */
		fprintf(fp,"\n");
	}

	fclose(fp);
}

void ctsv_test(void) {
	unsigned int i,j;
	/* Write different parameters */
	ctsv_input* inp = malloc(sizeof(ctsv_input));

	inp->points = 10;
	inp->nvars = 2;
	inp->labels = malloc(sizeof(char*)*inp->nvars);
	for (i = 0; i < inp->nvars; i++) {
		inp->labels[i] = malloc(2);
		inp->labels[i][0] = (char)('a'+i);
		inp->labels[i][1] = 0;
	}

	inp->t = malloc(sizeof(double)*inp->points);
	for (i = 0; i < inp->points; i++) {
		inp->t[i] = 0.1*i;
	}

	inp->v = malloc(sizeof(vector)*inp->points);
	for (i = 0; i < inp->points; i++) {
		inp->v[i].val = malloc(sizeof(double)*inp->nvars);
		inp->v[i].n = inp->nvars;

		for (j = 0; j < inp->nvars; j++) {
			inp->v[i].val[j] = (double)(i + j);
		}
	}

	ctsv_write("test.csv", ',', inp);
	ctsv_write("test.tsv", '\t', inp);
}
