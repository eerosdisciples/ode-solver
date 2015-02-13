/* Test program */

#include <stdio.h>
#include "include/vector.h"

void printv(vector v) {
	printf("[%f %f %f]\n", v.val[0], v.val[1], v.val[2]);
}
int main(){
	/*vector a = vnew(3,1), b = vnew(3,1);
	a.val[0] = 1, a.val[1] = 2, a.val[2] = 3;
	b.val[0] = 1, b.val[1] = 2, b.val[2] = 3;*/
	vector a = vinit(3, 1.0,2.0,3.0),
		   b = vinit(3, 1.0,2.0,3.0);
	
	vector v = vadd(a,b);
	
	printv(v);
	printv(vmuls(10.0, a));

	return 0;
}
