/* Test program */

#include <stdio.h>
#include "include/matrix.h"

void printm(matrix m) {
	printf("[%f %f %f]\n", m.val[0][0], m.val[1][0], m.val[2][0]);
}
int main(){
	matrix a = mnew(3,1), b = mnew(3,1);
	a.val[0][0] = 1, a.val[1][0] = 2, a.val[2][0] = 3;
	b.val[0][0] = 1, b.val[1][0] = 2, b.val[2][0] = 3;
	
	matrix m = madd(a,b);
	
	printm(m);
	printm(mmuls(10.0, a));

	return 0;
}
