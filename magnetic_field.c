/* Magnetic field management */

#include <stdio.h>
#include <stdlib.h>
#include "magnetic_field.h"
#include "vector.h"

#define BUFFER_SIZE 1024

double *B_r, *B_phi, *B_z,// Objekt, inte adresser.
		rmin, rmax, zmin, zmax;
int nr, nz;
char buffer[BUFFER_SIZE+1];

/**
 * Reads the next word from file into the buffer.
 * If buffer is empty after a call to word(),
 * an empty line has been discovered.
 *
 * f: Pointer to file to read from
 * RETURNS 1 IF EMPTY LINE WAS ENCOUNTERED
 */
int word(FILE *f) {// Reads one character at a time!
	int c, i=0;

	/* Get character */
	while (i<BUFFER_SIZE && (c=fgetc(f))!=EOF) {// EOF-end of file, c startar om om vi har en ny fil!
		/* If newline... */
		if (c == '\n')
			break;		/* ...stop... */
		/* If space or tab... */
		else if (c == ' ' || c == '\t') {
			/* If buffer is empty... */
			if (i == 0) continue;	/* ...do nothing and continue */
			else break;		/* ...stop... */
		} else	/* Append char to buffer and continue */
			buffer[i++] = c;// kan ta bort (char), ty redan deklarerad
	}
	
	buffer[i] = 0;
	return (i == 0);
}
/**
 * Skip a given number of lines
 *
 * n: Number of lines to skip
 * f: Pointer to file to read from
 */
void skip_lines(int n, FILE *f) {
	int c;
	/* While all lines have not been skipped,
	 * and we haven't reached the end-of-file
	 */
	while (n>0 && (c=fgetc(f))!=EOF) {
		/* If c = newline, decrease counter */
		if (c == '\n') n--; 
	}
}
/**
 * Reads magnetic field values into the given
 * double-array until empty line is found
 * (or file ends).
 *
 * B: array to load data into
 * f: Pointer to file to read from
 */
void read_mf(double *B, FILE *f) {// Vi är ute efter själva objektet och inte pointern! Därav * !
	int i = 0;
	/* While no empty line is encountered... */
	while (!word(f)) {// word returns 0=false if new line etc
		/* Read magnetic field value */
		B[i++] = atof(buffer);// /* Atof converts string char to floating point number (type double), i++ stores the data in i and then takes ++.*/
	}
}
/**
 * Loads magnetic field data from the file
 * with the given name. The data will then
 * be available in the arrays
 * B_r, B_phi and B_z (rmin, rmax, zmin, zmax,
 * nr and nz are also loaded)
 *
 * filename: Name of file containing data
 */
double * load_magnetic_field(char *filename,int choice) {
	/* Open file */
	FILE *f;
	f = fopen(filename, "r");
	/* TODO: Better error management... */
	if (!f) return;

	/* Read 6 first words */
	word(f); word(f); word(f);		/* "rmin", "rmax", "nr" */
	/* Identify rmin, rmax, nr */
	word(f); rmin = atof(buffer);/* rmin value */ // Buffern kommer ihåg var vi är.
	word(f); rmax = atof(buffer);/* rmax value */
	word(f); nr   = atoi(buffer);/* nr value */

	/* Read next 6 words */
	word(f); word(f); word(f);		/* "zmin", "zmax", "nz" */
	/* Identify zmin, zmax, nz */
	word(f); zmin = atof(buffer);/* zmin value */
	word(f); zmax = atof(buffer);/* zmax value */
	word(f); nz   = atoi(buffer);/* nz value */

	/* Calculate size of B-fields */
	int size = nr*nz;
	/* Allocate memory */
	B_r = malloc(size*sizeof(double));
	B_phi=malloc(size*sizeof(double));
	B_z = malloc(size*sizeof(double));

	/* Skip 2 lines */
	skip_lines(2, f);

	/* Read B_r */
	read_mf(B_r, f);
	/* Read B_phi */
	read_mf(B_phi, f);
	/* Read B_z */
	read_mf(B_z, f);
	
	/* Choose field one to return */
	if (choice==1)
	return B_r;
	
	if (choice==2)
	return B_phi;
	
	if (choice==3)
	return B_z;

	/* DONE */
}
