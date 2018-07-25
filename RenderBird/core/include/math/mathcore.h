#pragma once
#include <assert.h>
#include <cmath>
#include <string.h>

namespace Core
{
#ifdef NO_USE_HIGH_PRECISE
	using Float = float;
#else
	using Float = double;
#endif

	static const Float C_FLOAT_EPSILON = (Float)1e-06;
	static const Float C_FLOAT_EPSILON_HIGH = (Float)1e-09;
	static const Float C_PI = (Float)3.14159265358979323846;
	static const Float C_INV_PI = (Float)0.3183098861837907;
	static const Float C_HALF_PI = (Float)1.57079632679489661923;
	static const Float C_TWO_PI = (Float)6.28318530717958647692;

	inline Float DegToRad(Float val)
	{
		return  val * (Float)(C_PI / 180.0);
	}

	inline Float RadToDeg(Float val)
	{
		return (val) * (Float)(180.0 * C_INV_PI);
	}

	inline Float AngleModRad(Float a)
	{
			return std::fmod(a, C_TWO_PI);
	}

	inline Float AngleModEuler(Float a)
	{
		return std::fmod(a, 360.0);
	}

	template<typename T>
	inline T Clamp(T val, T min, T max)
	{
		if (val < min)
			val = min;
		if (val > max)
			val = max;
		return val;
	}

	enum class Axis
	{
		X = 0,
		Y = 1,
		Z = 2,
	};
}
