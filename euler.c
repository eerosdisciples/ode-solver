/*Implemetning Euler method */
#include <stdio.h>
#include <stdlib.h>
#include "magnetic_field.h"
#include "vector.h"
#include "euler.h"
#include <math.h>



/*Finds the cylindrical coordinates corresponding to the cartesian
cacoord[]-array with cartesian coords
cclcoord[]-array with cylindrical coords

Returns: array with cylindrical coords in order r,phi,z
*/


vector findcoords(vector carcoord){

	double x=carcoord.val[0];
	double y=carcoord.val[1];
	double z=carcoord.val[2];
	
	/*Define ccoord array*/
	//double ccoord[3];
	
	/*Transform coords*/
	carcoord.val[0]=sqrt(x*x+y*y);
	carcoord.val[1]=atan(y/x);
	carcoord.val[2]=z;
	
	return carcoord;
	
}

/* Transforms the magnetic field from cylindrical to  x,y,z-coordinates 
Input: x,y,z coordinates in vector ccord, pointers to Bphi,Br and Bz calculated in load_magnetic_field
Bx-field in x-direction
By-field in y-direction
Bz-field in z-direction

Output: vector with val Bx,By,Bz
*/

vector transform(vector carcord,double* Br,double* Bphi,double* Bz){ //Need to send pointer to array

	
	/* finds cylindrical coordinates*/
	vector ccord=findcoords(carcord);
	
	double phi=ccord.val[1];
	
	/* Allocate memory of field-vector and find interpolated values*/
	vector Bcyl=vnew(3); // Reason of segmentation fault is when calling inter with cetrain values(0,0)

	 Bcyl.val[0]=inter(ccord,Br);	 
	 Bcyl.val[1]=inter(ccord,Bphi);
	 Bcyl.val[2]=inter(ccord,Bz);

	
	
	vector Bcar=vnew(3);
	
	/*Calculate and save Bx By Bz, sphi is sin(phi) and cphi is cos(phi)*/
	double sin_phi=sin(phi);
	double cos_phi=cos(phi);
	
	Bcar.val[0]=-Bcyl.val[1]*sin_phi+Bcyl.val[0]*cos_phi;// Array subscript not an integer
	Bcar.val[1]=Bcyl.val[1]*cos_phi+Bcyl.val[0]*sin_phi;
	Bcar.val[2]=Bcyl.val[2];
	
	return Bcar;	
}

/*Approximates a fieldvalue corresponding to Br(r,z),Bphi(r,z) or Bz(r,z), with trilinear interpolation. 
Input: calculated cylindrical coordinates r,phi,z
Output: Approximate to value of field in the given coordinates.
uses: B_r, B_phi and B_z calculated from load_magnetic_field

To transform between double indices and single: t.ex B[r,z]=B[nz]
first index is a row and second is column => nz=r*3+z. Remember: C starts counting on 0!.
*/

double inter(vector ccord,double* B){
	
	double x=ccord.val[0];
	double l=ccord.val[1];
	double z=ccord.val[2];
	
	/* Declare some variables */
	int diff1;
	int diff2;
	int diff3;
	/* round each coordinates to closest integer source: wikipedia trilinear interpolation */

	int rox=round(x);	
	int rol=round(l);
	int roz=round(z);
				
	/* Returns 0 if round gives a lower argument, and 1 if higher
	CAN'T DIVIDE WITH ZERO, therefore check first if zero */
	if (x==0.0){
		 diff1=0;}
		else {
	 diff1=rox/x;	
}

if (l==0.0){
	 diff2=0;
} else {
	 diff2=rol/l;
}
if (z==0.0){
	 diff3=0;}
	else{
	 diff3=roz/z;
}
	/*Calculate next and previous r value */
	int x0=rox-diff1;
	
	int l0=rol-diff2;
	int z0=roz-diff3;
	
	int x1=rox+1-diff1;
	int l1=rol+1-diff2;
	int z1=roz+1-diff3;
	
	/* Finds the value of field in the given coordinates, by using trilinear interpolation.Here 
	y is substituted with l */
	
    double	 xd=(x-x0)/(x1-x0);	
    double   ld=(l-l0)/(l1-l0);
    double   zd=(z-z0)/(z1-z0);
		
	/*Approximate in B, independent of l(thus y)*/
	double c00=B[x0+z0]*(1-xd)+B[3*x1+z0]*xd;
	double c10=B[3*x0+z0]*(1-xd)+B[3*x1+z0]*xd;
	double c01=B[3*x0+z1]*(1-xd)+B[3*x1+z1];
	double c11=B[3*x0+z1]*(1-xd)+B[3*x1+z1];	
	  
	double c0=c00*(1-ld)+c10*ld;
	double c1=c01*(1-ld)+c11*ld;
	
	double c=c0*(1-zd)+c1*zd;
	
	return c;
  
  }  


/* Implements Euler method.
  indata: startingpoints, timestep, timesintervall
  output:nothing right now
  
 parameters:
B-vector with field in the current coordinates, uses transform
carcoords-cartesian coordinates in current interation
  frac-in how many part the intervall should be divided-to calculate h
  h-timestep
  tmax-size of intervall
  vector z0-starting values of r1,r2,r3,v1,v2,v3
  
  uses field calculated from magnetic_field.c  
  */
  
  void eulermethod(int frac,int tmax,vector z0,double* Br,double* Bphi,double* Bz){
	  
	  /* allocate memory for array storing each variable
	   c starts counting with 0
	   */
	  
	  vector r1=vnew(tmax-1);
	  vector r2=vnew(tmax-1);
	  vector r3=vnew(tmax-1);
	  vector v1=vnew(tmax-1);
	  vector v2=vnew(tmax-1);
	  vector v3=vnew(tmax-1);
	  
	  /* allocate memory for field and coordinates to be used in loop */
	  
	  vector carcords=vnew(3);
	  vector B=vnew(3);
	    
	  /* calculate timestep */
	  double h=(double)tmax/frac;
	  
	  
	  /* iterate once with starting values */
	  
	  /* calculates field in coordinates 0,0,0 */
	  carcords.val[0]=0;
	  carcords.val[1]=0;
	  carcords.val[2]=0;
	  
	  B=transform(carcords,Br,Bphi,Bz);
	  double B1=B.val[0];
	  double B2=B.val[1];
	  double B3=B.val[2];
	  
	 double z1=z0.val[0];
	 double z2=z0.val[1];
	 double z3=z0.val[2];
	 double z4=z0.val[3];
	 double z5=z0.val[4];
	 double z6=z0.val[5];
	  
	  /* Save starting values in each array in position 0 */
	  
	  r1.val[0]= z1;
	  r1.val[0]= z2;
	  r1.val[0]= z3;
	  
	  v1.val[0]= z4;
	  v2.val[0]= z5;
	  v3.val[0]= z6;
	  
	  /* Make one iteration */
	  
	  r1.val[1]= z1+h*z4;
	  r1.val[1]= z2+h*z5;
	  r1.val[1]= z3+h*z6;
	  
	  v1.val[1]= z4+h*(z5*B3-z6*B2);
	  v2.val[1]= z5+h*(z6*B1-z4*B3);
	  v3.val[1]= z6+h*(z4*B2-z5*B3);
	  
	  /* Do the rest of the iterations 	 */
	  int i;
	  for (i=1;i<tmax-1;i++){// tmax-1 because we calculate r and v at next t, thus stop at tmax-1 gives all values up to tmax.
	  	
		  /* determine coordinates where the particle is right now */
	  
		  carcords.val[0]=r1.val[i];
		  carcords.val[1]=r2.val[i];
		  carcords.val[2]=r3.val[i];
		
		  /*Calculate field at this point */
		
		        B=transform(carcords,Br,Bphi,Bz); 
		
		  /* New variables  */
		  
	       B1=B.val[0];
		   B2=B.val[1];
		   B3=B.val[2];
		   
	 	  z1=r1.val[i];
	 	  z2=r2.val[i];
	 	  z3=r3.val[i];
	 	  z4=v1.val[i];
	 	  z5=v2.val[i];
	 	  z6=v3.val[i];
	  
		
		  r1.val[i+1]= z1+h*z4;
		  r1.val[i+1]= z2+h*z5;
		  r1.val[i+1]= z3+h*z6;
	  
		  v1.val[i+1]= z4+h*(z5*B3-z6*B2);
		  v2.val[i+1]= z5+h*(z6*B1-z4*B3);
		  v3.val[i+1]= z6+h*(z4*B2-z5*B3);
		  
		  printf("Iteration number %d\n",i);
		  
	  }
	  
	
  }
