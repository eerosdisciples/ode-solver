/* ODE Solver */

#include <stdlib.h>
#include "ode.h"
#include "vector.h"
#include <math.h>
#include "equation.h"


/**
 * Solve an Initial Value Problem (IVP ODE)
 *
 * equation: Pointer to function giving the equation,
 *   that is f in "z' = f(t,z)" where `t' is time
 *   (scalar) and `z' is the unknown (vector)
 * t0: Initial time
 * z0: Initial value
 * n:  Number of points to calculate
 * T:time
 * Z: vector containing values from previous iteration
 * RETURNS a solution to the equation as a defined type ode_solution consisting of Z,step, flag
 */


ode_solution* ode_solve( vector *(equation)(double, vector*),ode_solution *parameters,double T){
	/* TODO: find a better way to calculate absolute value
	Check if everything is correct */
	
	/* Choose order of iteration */
	int order1=4;
	int order2=5;


	 double eps0=0.01;           /* Tolerans parameter */
	 double beta=0.7;           /* Safety parameter */
	 int flag; // Variable to store value indicating whether the the iteration should be re-done
	 
	/* Take out h */
	 double h=parameters->step;
	 	 
	 /* To store optimal steplenght*/
	  double hopt;
	 /* Calculate next point */
	 
	   vector* Z_next=ode_step(equation, parameters,T,order1,0);
	   vector* Zhat=ode_step(equation, parameters,T,order2,1);
	 	 
	 /* Calculate epsilon. Absolute value of function */
	 
	 double eps=pow(vdot(vadd(vmuls(-1,Z_next),Zhat),vadd(vmuls(-1,Z_next),Zhat)),1.0/2);// = absolute value of vector () 
		 
		/* Check that each argument is ok
		 find a faster solution, without calculating k in each turn*/
		 printf("Now Z is  %f and Zhat %f\n",Z_next->val[0],Zhat->val[0]);// FEL I NÄR VI SPARAR VÄRDEN!!
	 
	 /* Choose optimal step */
	 if (eps>=eps0){
		 	 hopt=beta*h*pow(eps0/eps,1.0/5);
		 flag=REDO_STEP;
	 }
	 else  { hopt=beta*h*pow(eps0/eps,1.0/4);
		 flag=OK_STEP;
	 }
	 
	 /* Save and return calculated values */
	   ode_solution solution;
	   solution.Z=vinit(2);
	   solution.Z->val[0]=Z_next->val[0];
	   solution.Z->val[1]=Z_next->val[1];
	   solution.step=hopt;
	   solution.flag=flag;
	 
 /*	ode_solution param0;// HÄR BLIR DET BUS ERROS OM VI HAR PEKARE IST FÖR OBJEKT
 	param0.Z=vinit(2);
 	 param0.Z->val[0]=x0;
 	 param0.Z->val[1]=y0;
 	  param0.step=h;
	 
	 /* Create pointer to object solution and return  */
	 ode_solution *pointer;
	 pointer=&solution;	
	 return pointer;// SHOULD RETURN Z_NEXT, HOPT, FLAG
}
/**
 * Calculates the next point depending on the order
 *
 * Same arguments as in ode_solve
 * Z: vector containing values from previous iteration
 *Which-to choose b or bhat CHANGE
 * RETURNS array with K values
 */

vector * ode_step(vector *(equation)(double, vector*),ode_solution *parameters, double T,int order,int which){

	/*TODO 	WHICH COEFFICIENTS TO USE; BHAT OR B*/
 double A[5][6]={{1./5,0,0,0,0,0}, /* Stores Cash Carp coefficients */
	 			{3.0/40,9.0/40,0,0,0,0},
				{3.0/10,-9.0/10,6.0/5,0,0,0},
				{-11.0/54,5.0/2,-70.0/27,35.0/27,0,0},
				{1631.0/55296,175.0/512,575.0/13828,44275.0/110592,253.0/4096,0}}; 
  //  double b_first[5]={37.0/378,0,250.0/621,125.0/594,512.0/1771}; /* Stores Cash Carp coefficients */
    double B[2][5]={{37.0/378,0,250.0/621,125.0/594,512.0/1771},{2825.0/27648,0,28575.0/48384,13525.0/55296,277.0/14336,1/4}}; /* Stores Cash Carp coefficients. Contains
		b in first row and bhat in second */
		
    double alpha[6]={1.0/5,3.0/10,1,7.0/8};/* Stores Cash Carp coefficients, if explicit time dependence */
 
 
 vector* Z; // DETHÄR ÄR FEL!!!
 Z=parameters->Z;
 //Z->val[0]=parameters->Z->val[0];
 //Z->val[1]=parameters->Z->val[1];
 double h=parameters->step;
 
 printf("first step is %f\n",h);
 printf("Z.n is %d",Z->n);
 
 /* Variables used in loop*/
 int i;
 int j;
 /* Array to store K-values CHANGE IF PARTICLE TYPE 
 Allocate memory for each vector*/
 
 vector *K[order];
 
 for (i=0;i<=order-1;i++){	 
 K[i]=vinit(Z->n);//malloc(sizeof(vector));
}

 /* Help variables, need only one if only one iteration is done; CHANGE IF NEEDED */
 vector *sum;
 sum=vinit(Z->n);
 
 /*vector *sum2;
 sum2=vinit(order-1);
 
 /* Cacluate first K */
 
 /*  Calculate each K up to order. Start from K2*/

 K[0]=equation(T+alpha[0]*h,vadd(Z,sum));
 
 for(i=1;i<order-1;i++){
	 
	 /*Cacalculate sum to use in argument */ 
	 
	 for (j=0;j<i-1;j++){
		   sum = vadd(sum,vmuls(h*A[i][j],K[j]));//  K[j]);
	 }
	 /* Calculate K */
	  K[i]=equation(T+alpha[i]*h,vadd(Z,sum));
   printf("one element of K is %f and %f \n",K[i]->val[0],K[i]->val[1]);
	  
 }
 
 /* Calculate sum do be used in next point */
 for (i=0;i<order;i++){
	  sum=vadd(sum,vmuls(h*B[which][i],K[i]));	  
 }
 
 /* Calculate next point */

 
 
	return vadd(Z,sum);
}


/* Test function for this module */
void ode_test(void) {

	/*TODO: find a better way to store values
	store time-and coordinates  size varies????
*/	
/* Predator prey model
 */

	/* Initiate vector to store calculated points */
	vector* coordinates;
	/* Allocate memory, right now just any size CHANGE!!!!*/
	coordinates=vinit(20);
	/* To store time */
	vector* t;
	t=vinit(11);
	t->val[0]=0;
		
	/* Choose starting point */
	double x0=1;
	double y0=1;
	
	/* Choose starting steplenght */
	
	double h=1.5;// Större än 7 ger nan
	
	
	/* Save everything in type 'ode_solution' */ 
	ode_solution param0;// HÄR BLIR DET BUS ERROS OM VI HAR PEKARE IST FÖR OBJEKT
	param0=malloc(sizeof(ode_solution))
	 param0.Z=vinit(2);
	 param0.Z->val[0]=x0;
	 param0.Z->val[1]=y0;
	  param0.step=h;
 
	/* First value of flag is 'OK'*/
	  	 param0.flag=0;
	
	/* Iterate and save!*
	
	/* Choose size of intervall */
	
	double Tmax=2;
	
	double T;
	
	/* Create pointer to param*/
	ode_solution *param;
	param=&param0;
	
	/* Iteration variable */
	int i=0;
	
	int antaliterationer=0;
	
		for (T=0;t->val[0]<Tmax;i++){
			antaliterationer++;
			printf("det här är iteration %d och i är %d \n\n\n ",antaliterationer,i);
		/* Iterate ones */
			printf("t[i] är %f\n",t->val[i]);
			
			printf("n är %d\n",param->Z->n);// Kolla & och * dereferencing
			
			param=ode_solve(equation_predator_prey,param,t->val[i]);
			
		/* Save new steplenght 
				h=param->step;*/
		/* Check if iteration needs to be re-done */
				/* flag=0 -> ok -> continue */		
					if (param->flag==0) {												
						t->val[i+1]=t->val[i]+param->step;						
			/* Save new values in array */

			coordinates->val[i]=param->Z->val[0];
			coordinates->val[i+1]=param->Z->val[1];
			
				} else i=i-1; // Redo step with new calculated h in param
				
					}
	
	return;
}