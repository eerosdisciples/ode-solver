#ifndef _PROBLEM_FUNCTIONS_H
#define _PROBLEM_FUNCTIONS_H

#include "rkf45.h"
#include "IO_data.h"

solution_data* output_GCM(solution_data*);
solution_data* output_no_GCM(solution_data*);

ode_solution* solve_GCM(vector*, initial_data*);
ode_solution* solve_no_GCM(vector*, initial_data*); 

#endif/*_PROBLEM_FUNCTIONS_H*/