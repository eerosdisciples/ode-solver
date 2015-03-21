#ifndef _CTSV_H
#define _CTSV_H

#include "arguments.h"
#include "IO_data.h"

/* Write data to CSV or TSV file */
void ctsv_write(char*, char, solution_data*, arguments*);

/* Test function for this module */
void ctsv_test(void);

#endif/*_CTSV_H*/
