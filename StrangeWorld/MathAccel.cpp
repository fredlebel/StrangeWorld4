#include <stdlib.h>
#include "MathAccel.h"

double* MathAccel::cosLookup = NULL;
double* MathAccel::sinLookup = NULL;
double* MathAccel::toRadLookup = NULL;
#if defined( USE_SQRT_LOOKUP )
int* MathAccel::sqrtTableLookup = NULL;
#endif
