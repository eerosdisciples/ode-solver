/* ODE Solver */

#include <stdlib.h>
#include <stdio.h>
#include "ode.h"
#include "vector.h"
#include <math.h>
#include "equation.h"
#include "ctsv.h"

#define EPS0 0.1
#define SAFETY_FACTOR 0.99	/* Safety factor beta */

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
	
	double B[2][6]={
		{37.0/378,0,250.0/621,125.0/594,512.0/1771,0},
		{2825.0/27648,0,28575.0/48384,13525.0/55296,277.0/14336,1/4}
	}; /* Stores Cash Carp coefficients. Contains
		b in first row and bhat in second */
	
	/* Choose order of iteration */
	unsigned int order1=4;
	unsigned int order2=5;

	double eps0=EPS0;           /* Tolerans parameter */
	double beta=SAFETY_FACTOR;  /* Safety parameter */
	int flag; // Variable to store value indicating whether the the iteration should be re-done
	 
	 
	vector* Z;
	Z=parameters->Z;

	double h=parameters->step;
 
	/* Variables used in loop*/
	unsigned int i;
		 	 
	/* To store optimal steplenght*/
	double hopt;
	/* Calculate next point */
	
	vector* K = ode_step(equation, parameters,T,order2);
	
	/* Calculate sum do be used in next point for Z_next and Zhat */
	/* */
	
	/* Help variables */
	vector *sum1, *sum2;
	sum1=vnew(Z->n);
	sum2=vnew(Z->n);
	
	/* Initialize sum */
	for (i = 0; i < sum1->n; i++) {
		sum1->val[i] = 0;
		sum2->val[i] = 0;
	}
	
	for (i=0; i<order1; i++){
		vector *ms = vmuls(h*B[0][i], K+i);
		vector *ns = vadd(sum1,ms);	  
		vfree(sum1);
		vfree(ms);

		sum1 = ns;
	}
 
	/* Calculate next point */
	vector* Z_next=  vadd(Z,sum1);

	for (i=0; i < order2; i++){
		vector *ms = vmuls(h*B[1][i], K+i);
		vector *ns = vadd(sum2,ms);	  
		vfree(sum2);
		vfree(ms);

		sum2 = ns;
	}
	
	vector* Zhat=  vadd(Z,sum2);
		 	 
	/* Calculate epsilon. Absolute value of function */
	/* eps = ||Z^ - Z|| */
	double eps;
	vector *scalmul = vmuls(-1, Z_next);
	vector *zadd = vadd(scalmul,Zhat);
	eps = sqrt(vdot(zadd,zadd));

	vfree(scalmul); vfree(zadd);
	/* Choose optimal step */
	if (eps >= eps0) {
		hopt=beta*h*pow(eps0/eps,0.20);
		if (hopt < parameters->step)
			printf("h increases strangely...\n");
		flag=REDO_STEP;
	} else  {
		hopt=beta*h*pow(eps0/eps,0.25);
		if (hopt < parameters->step)
			printf("h decreases strangely...\n");
		flag=OK_STEP;
	}
	 
	/* Save and return calculated values */
	/*ode_solution *solution;
	solution.Z=vinit(2);
	solution.Z->val[0]=Z_next->val[0];
	solution.Z->val[1]=Z_next->val[1];*/
	parameters->step = hopt;
	parameters->flag = flag;
	vector* Zp1 = parameters->Z+1;
	Zp1->val = Z_next->val;
	Zp1->n = Z_next->n;

	/* NOTE: We do not want to do a
	 * vfree on Z, since `val' is now
	 * used in Zp1!. Zhat however is
	 * completely unused from now on */
	free(Z_next);
	vfree(Zhat);
	 
	/* Return the solver object */
	return parameters;
}
/**
 * Calculates the next point depending on the order
 *
 * Same arguments as in ode_solve
 * Z: vector containing values from previous iteration
 *Which-to choose b or bhat CHANGE
 * RETURNS array with K values
 */

vector * ode_step(vector *(equation)(double, vector*),ode_solution *parameters, double T,unsigned int order){
	/*TODO 	WHICH COEFFICIENTS TO USE; BHAT OR B*/
	double A[5][6]={
		{1./5,0,0,0,0,0}, /* Stores Cash Carp coefficients */
		{3.0/40,9.0/40,0,0,0,0},
		{3.0/10,-9.0/10,6.0/5,0,0,0},
		{-11.0/54,5.0/2,-70.0/27,35.0/27,0,0},
		{1631.0/55296,175.0/512,575.0/13828,44275.0/110592,253.0/4096,0}
	};
	//  double b_first[5]={37.0/378,0,250.0/621,125.0/594,512.0/1771}; /* Stores Cash Carp coefficients */
	
		
    double alpha[]={1.0/5,3.0/10,1,7.0/8};/* Stores Cash Carp coefficients, if explicit time dependence */
 
	vector* Z;
	Z=parameters->Z;

	double h=parameters->step;
 
	/* Variables used in loop*/
	unsigned int i;
	unsigned int j;
	/* Array to store K-values CHANGE IF PARTICLE TYPE 
	Allocate memory for each vector*/
 
	vector *K;
	K = malloc(sizeof(vector)*(order+1));
	for (i=0;i<=order;i++){	 
		K[i].n = Z->n;
	}

	/* Help variables, need only one if only one iteration is done; CHANGE IF NEEDED */
	vector *sum;
	
	/*vector *sum2; sum2=vinit(order-1);*/
 
	/* Cacluate first K */
	vector* vec = equation(T, Z);
	K[0].val = vec->val;
	free(vec);
 
	/*  Calculate each K up to order. Start from K2 (i=1) */
	for (i=1; i < order; i++) {
		/* Initialize sum */
		sum = vnew(Z->n);
		for (j = 0; j < Z->n; j++)
			sum->val[j] = 0;

		/*Calculate sum to use in argument */ 
		for (j=0; j < i; j++) {
			vector *ms = vmuls(h*A[i][j], K+j);
			vector *ns = vadd(sum, ms);
			vfree(sum);
			vfree(ms);
			sum = ns;
		}

		/* Calculate K */
		vector *ns = vadd(Z, sum);
		vec=equation(T+alpha[i]*h,ns);
		K[i].val = vec->val;
		free(vec);
		vfree(ns);
		vfree(sum);
	}
	return K;
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
	unsigned int points = 2500;
	/* Allocate memory, right now just any size CHANGE!!!!*/
	coordinates=malloc(sizeof(vector)*(points+1));
	/* Set initial point */
	coordinates->val = malloc(sizeof(double)*2);
	coordinates->n = 2;
	/* Initial condition:nbr of animals of each species */
	coordinates->val[0] = 34.91; 
	coordinates->val[1] = 3.857;

	/* To store time */
	double *t = malloc(sizeof(double)*(points+1));
	t[0] = 0;
		
	/* Choose starting steplenght */
	double h=0.3;// Större än 7 ger 
	
	/* Save everything in type 'ode_solution' */ 

	ode_solution *param;
	param = malloc(sizeof(ode_solution));
	param->step=h;
 
	/* First value of flag is 'OK'*/
	param->flag=0;
	
	/* Iterate and save!*/
	
	/* Choose size of intervall */
	
	double Tmax=2;
	double T;
	
	/* Iteration variable */
	unsigned int i=0;	

	/*for (T=0;t->val[0]<Tmax;i++){*/
	for (i = 0; i < points; i++) {
		t[i+1]=t[i]+param->step;

		/* Iterate ones */
		param->Z = coordinates+i;
		ode_solve(equation_predator_prey, param, t[i]);
			
		/* Save new steplenght 
				h=param->step;*/
		/* Check if iteration needs to be re-done */
				/* flag=0 -> ok -> continue */		
		if (param->flag!=0) i=i-1; // Redo step with new calculated h in param
	}
	ctsv_input output;
	output.t=t;
	output.v=coordinates;
	output.labels=malloc(sizeof(char *)*2);
	output.labels[0]="x";
	output.labels[1]="y";
	output.points=points;
	output.nvars=2;
	ctsv_write("Output.csv",',',&output);
}
