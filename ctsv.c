/* CSV/TSV file writer */

#include "ctsv.h"
#include <stdio.h>
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

int i;
int j;

/* To choose delimeter */
int d=0;



/* To be able to choose tab or , as delimeter */
char *format[2]={",","\t"};

/* Choose delimeter */
if (type=="t") d=1;



/* write labels */
for (i=0;i<data->n;i++){
	
fprintf(fp,"%sformat[d]",data->labels[i]);
	
}

/* Newline */

fprintf(fp,"\n");

/* Print data */

for (i=0;i<data->n;i++){
	
	/*Print all elements in vector i */
	for (j=0;j<data->v->n;j++){
		
		fprintf(fp,"%f",data->v[j].val[j]);
		
	}
	
	/* Newline */
	fprintf(fp,"\n");
	
}


fclose(fp);

}

void ctsv_test(void) {
/* Write different parameters */
char *filename="plottest.txt"; 
	char type="t";
	
    ctsv_input *data
		data=malloc(sizeof(ctsv_input));
	data->labels=malloc(sizeof(char)*n);
	data->v=(vector *)malloc(rows * cols *sizeof(vector));
}