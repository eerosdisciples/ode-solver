/* The main program */
#include "readfile.h"
#include "domain.h"
#include <stdio.h>
#include "domain.h"

int main(int argc, char *argv[]) {// argc- hur många argument och argv själva filen
	 domain *d=domain_load(argv[1]);
		 double r[2]={5.39718,5.39718};
	 double z[2]={-3.5,-1};
	 
int inside = domain_check(d, r, z);

 
if (inside==1)
printf("Not in Domain \n");

if (inside==0)
printf("In Domain\n");
	 // domain_test();

}
