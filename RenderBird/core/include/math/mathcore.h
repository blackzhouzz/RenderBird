#pragma once
#include <cmath>
#include <limits>
#include <assert.h>

namespace Core
{
#ifdef NO_USE_HIGH_PRECISE
	using Float = float;
#else
	using Float = double;
#endif

	static const Float C_FLOAT_EPSILON = 1e-06;//(Float)std::numeric_limits<Float>::epsilon();
	static const Float C_FLOAT_EPSILON_HIGH = (Float)1e-12;
	static const Float C_PI = (Float)3.14159265358979323846;
	static const Float C_INV_PI = (Float)0.3183098861837907;
	static const Float C_HALF_PI = (Float)1.57079632679489661923;
	static const Float C_QUARTER_PI = (Float)0.785398163397448;
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

	inline Float xor_signmask(Float x, int y)
	{
		return Float(int(x) ^ y);
	}

	/**
	* checks whether a <= b with epsilon window
	*/
	template <typename T>
	bool eq(T a, T b, T epsilon = std::numeric_limits<T>::epsilon())
	{
		return std::fabs(a - b) <= epsilon;
	}
	/**
	* checks whether a < b with epsilon window
	*/
	template <typename T>
	bool lt(T a, T b) 
	{
		if (!eq(a, b)) 
		{ // if a < b then a != b
			return a < b;
		}
		return false;
	}

	/**
	* checks whether a <= b with epsilon window
	*/
	template <typename T>
	bool lte(T a, T b) 
	{
		if (eq(a, b)) 
		{
			return true;
		}
		return a < b;
	}

	/**
	* checks whether a > b with epsilon window
	*/
	template <typename T>
	bool gt(T a, T b)
	{
		if (!eq(a, b))
		{ // if a > b then a != b
			return a > b;
		}
		return false;
	}

	/**
	* checks whether a >= b with epsilon window
	*/
	template <typename T>
	bool gte(T a, T b)
	{
		if (eq(a, b))
		{
			return true;
		}
		return a > b;
	}
}
