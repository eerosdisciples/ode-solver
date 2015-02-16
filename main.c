/* Test program */

#include <stdio.h>
#include "magnetic_field.h"
#include "vector.h"

void printv(vector v) {
	printf("[%f %f %f]\n", v.val[0], v.val[1], v.val[2]);
}
int main(int argc, char *argv[]) {
	if (argc == 2)
		load_magnetic_field(argv[1]);

	return 0;
}
