#include <stdlib.h>
#include "MathAccel.h"

double* MathAccel::cosLookup = nullptr;
double* MathAccel::sinLookup = nullptr;
double* MathAccel::toRadLookup = nullptr;
#if defined( USE_SQRT_LOOKUP )
double* MathAccel::sqrtTableLookup = nullptr;
#endif
