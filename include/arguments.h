#ifndef _ARGUMENTS_H
#define _ARGUMENTS_H

typedef struct {
	double tstart, tend;
	double *r0, *v0;
	char *magfield_file, *domain_file;
	double particle_mass;
	double particle_charge;
} arguments;

double *atodp3(const char*);
arguments *parse_args(int, char*[]);

#endif/*_ARGUMENTS_H*/
