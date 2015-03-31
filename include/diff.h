#ifndef _DIFF_H
#define _DIFF_H

#include "vector.h"

typedef struct {
	vector *B;		/* Magnetic field in point */
	vector *gradB;	/* Gradient of absolute value of magnetic field */
	vector *curlB;	/* Curl of unit vector b^ */
	double Babs;	/* Absolute value of B, ||B|| */
} diff_data;

diff_data* diff(vector*);
diff_data *ndiff(vector*);

#endif/*_DIFF_H*/
