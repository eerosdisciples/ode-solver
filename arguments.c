/* Argument parser */

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arguments.h"
#include "input.h"

void help(void) {
	printf(
"solver [-e time] -d file -f file -r x,y,z -t time -v vx,vy,vz [input_file]\n\n"

"-d name; --domain-file name \tName of domain file\n"
"-n points; --number-of-points points Number of output points\n"
"-f name; --B-field-file name\tName of magnetic field file\n"
"-r x,y,z; --r0 x,y,z        \tInitial position. x, y, z are numbers\n"
"-t time; --tstart time      \tInitial time value\n"
"-v vx,vy,vz; --v0 vx,vy,vz  \tInitial velocity. vx, vy, vz are numbers\n"
	);
}
void version(void) {
	printf("VERSION\n");
}

/* ASCII to double pointer (3-component) */
double *atodp3(const char *s) {
	char buffer[1024];
	int bi=0, di=0;
	double *dp = malloc(sizeof(double)*3);

	/* Allow for easier syntax below */
	s--;

	/* Turn comma-separated list of numbers into
	 * actual double list */
	while (*(++s) && di < 3) {
		if (*s == ',') {
			buffer[bi] = 0;
			dp[di++] = atof(buffer);
			bi = 0;
		} else buffer[bi++] = *s;
	}

	/* Handle last number */
	if (di < 3) {
		buffer[bi] = 0;
		dp[di] = atof(buffer);
	}

	return dp;
}

arguments *parse_args(int argc, char *argv[]) {
	arguments *args;
	int c, option_index;

	args = malloc(sizeof(arguments));
	args->domain_file = args->magfield_file = NULL;
	args->tstart = 0.;
	args->r0 = args->v0 = NULL;

	while (1) {
		static struct option long_options[] = {
			{"B-field-file", 	required_argument, 	0, 'f'},
			{"domain-file", 	required_argument, 	0, 'd'},
			{"help", 			no_argument, 		0, 0},
			{"number-of-points",required_argument, 	0, 'n'},
			{"tstart", 			required_argument, 	0, 't'},
			{"r0",				required_argument,	0, 'r'},
			{"v0",				required_argument,	0, 'v'},
			{"version", 		no_argument, 		0, 0},
			{0,0,0,0}
		};

		option_index = 0;
		c = getopt_long(argc, argv, "d:e:f:i:t:v:x:",
				long_options, &option_index);

		if (c == -1)
			break;

		switch (c) {
			case 0:
				if (long_options[option_index].flag != 0)
					break;
				if (!strcmp(long_options[option_index].name, "help")) {
					help();
					exit(EXIT_SUCCESS);
				} else if (!strcmp(long_options[option_index].name,"version")) {
					version();
					exit(EXIT_SUCCESS);
				}
				break;

			case 'd':	/* Name of domain file */
				args->domain_file = optarg;
				break;
			case 'f':	/* Name of magnetic field file */
				args->magfield_file = optarg;
				break;
			case 'i':
				args = input_read(optarg);
				break;
			case 'r':	/* Initial position (x0) */
				args->r0 = atodp3(optarg);
				break;
			case 't':	/* Initial time (t0) */
				args->tstart = atof(optarg);
				break;
			/* Initial position and velocity are given as lists of
			 * numbers. Example:
			 *   -v0 1.23,4.2,-1e3
			 *   -x0 -555,4.32e-2,0.007
			 */
			case 'v':	/* Initial velocity (v0) */
				args->v0 = atodp3(optarg);
				break;

			case '?':
				/* Unknown argument. Getopt already printed error msg */
				break;

			default: exit(EXIT_FAILURE);
		}
	}

	/* Handle remaining input arguments */
	while (optind < argc) {
		args = input_read(argv[optind++]);
	}

	if (args->domain_file == NULL)
		printf("ERROR: Expected domain file!\n");
	else if (args->magfield_file == NULL)
		printf("ERROR: Expected magnetic field file!\n");
	else if (args->r0 == NULL)
		printf("ERROR: Expected initial position!\n");
	else if (args->v0 == NULL)
		printf("ERROR: Expected initial velocity!\n");
	else return args;

	exit(EXIT_FAILURE);
}
