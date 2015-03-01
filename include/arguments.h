#ifndef _ARGUMENTS_H
#define _ARGUMENTS_H

typedef struct {
	double tstart, tend;
	double *r0, *v0;
	char *magfield_file, *domain_file;
} arguments;

arguments *parse_args(int, char*[]);

#endif/*_ARGUMENTS_H*/
