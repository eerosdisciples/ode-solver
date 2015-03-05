/* The main program */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ode.h"


#include "arguments.h"
#include "domain.h"
#include "readfile.h"

int main(int argc, char *argv[]) {

	/* Solve */
	ode_test();
	return 0;
}
