#pragma once
#include <assert.h>
#include <math.h>

namespace MathLib
{

#undef Float
#ifdef NO_USE_HIGH_PRECISE
#define Float float
#else
#define Float double
#endif

	static const Float C_FLOAT_EPSILON = (Float)1e-06;
	static const Float C_FLOAT_EPSILON_HIGH = (Float)1e-09;
	static const Float C_PI = (Float)3.14159265358979323846;
}
