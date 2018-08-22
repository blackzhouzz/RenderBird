#pragma once
#include "core_private.h"
#include <cmath>
#include <limits>
#include <assert.h>
#include <mathfu/vector.h>
#include <mathfu/matrix.h>
#include <mathfu/quaternion.h>
#include <mathfu/rect.h>
#include <mathfu/utilities.h>

namespace Core
{
	using Vector2f = mathfu::Vector<Float, 2>;
	using Vector3f = mathfu::Vector<Float, 3>;
	using Vector4f = mathfu::Vector<Float, 4>;
	using Quaternion = mathfu::Quaternion<Float>;
	using Matrix3f = mathfu::Matrix<Float, 3>;
	using Matrix4f = mathfu::Matrix<Float, 4>;
	using Vector2i = mathfu::Vector<int, 2>;

	static const Vector2f C_Zero_v2f(0.0f, 0.0f);
	static const Vector2f C_One_v2f(1.0f, 1.0f);
	static const Vector2f C_AxisX_v2f(1.0f, 0.0f);
	static const Vector2f C_AxisY_v2f(0.0f, 1.0f);
	static const Vector3f C_Zero_v3f(0.0f, 0.0f, 0.0f);
	static const Vector3f C_One_v3f(1.0f, 1.0f, 1.0f);
	static const Vector3f C_AxisX_v3f(1.0f, 0.0f, 0.0f);
	static const Vector3f C_AxisY_v3f(0.0f, 1.0f, 0.0f);
	static const Vector3f C_AxisZ_v3f(0.0f, 0.0f, 1.0f);
	static const Vector4f C_Zero_v4f(0.0f, 0.0f, 0.0f, 0.0f);
	static const Vector4f C_One_v4f(1.0f, 1.0f, 1.0f, 1.0f);
	static const Vector4f C_AxisX_v4f(1.0f, 0.0f, 0.0f, 0.0f);
	static const Vector4f C_AxisY_v4f(0.0f, 1.0f, 0.0f, 0.0f);
	static const Vector4f C_AxisZ_v4f(0.0f, 0.0f, 1.0f, 0.0f);
	static const Vector4f C_AxisW_v4f(0.0f, 0.0f, 0.0f, 1.0f);

	static const Quaternion C_Identity_qf = Quaternion::identity;

	static const Matrix3f C_Identity_m3f = Matrix3f::Identity();
	static const Matrix4f C_Identity_m4f = Matrix4f::Identity();

	static const Float C_FLOAT_EPSILON = 1e-06f;//(Float)std::numeric_limits<Float>::epsilon();
	static const Float C_FLOAT_EPSILON_HIGH = (Float)1e-12f;
	static const Float C_PI = (Float)3.14159265358979323846f;
	static const Float C_1_INV_PI = (Float)0.3183098861837907f;
	static const Float C_HALF_PI = (Float)1.57079632679489661923f;
	static const Float C_QUARTER_PI = (Float)0.785398163397448f;
	static const Float C_2_PI = (Float)6.28318530717958647692f;

	inline Float DegToRad(Float val)
	{
		return  val * (Float)(C_PI / 180.0f);
	}

	inline Float RadToDeg(Float val)
	{
		return (val) * (Float)(180.0f * C_1_INV_PI);
	}

	inline Float AngleModRad(Float a)
	{
		return std::fmod(a, C_2_PI);
	}

	inline Float AngleModEuler(Float a)
	{
		return std::fmod(a, 360.0f);
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

	template<typename T, int d>
	inline mathfu::Vector<T, d> Ceil(const mathfu::Vector<Float, d>& v)
	{
		mathfu::Vector<T, d> ret;
		for (int i = 0; i < d; ++i)
		{
			ret[i] = (T)std::ceil(v[i]);
		}
		return ret;
	}

	template<typename T, int d>
	inline mathfu::Vector<T, d> Floor(const mathfu::Vector<Float, d>& v)
	{
		mathfu::Vector<T, d> ret;
		for (int i = 0; i < d; ++i)
		{
			ret[i] = (T)std::floor(v[i]);
		}
		return ret;
	}

	template<typename T, int d>
	inline mathfu::Vector<T, d> Max(const mathfu::Vector<T, d>& v1, const mathfu::Vector<T, d>& v2)
	{
		mathfu::Vector<T, d> ret;
		for (int i = 0; i < d; ++i)
		{
			ret[i] = std::max(v1[i], v2[i]);
		}
		return ret;
	}

	template<typename T, int d>
	inline mathfu::Vector<T, d> Min(const mathfu::Vector<T, d>& v1, const mathfu::Vector<T, d>& v2)
	{
		mathfu::Vector<T, d> ret;
		for (int i = 0; i < d; ++i)
		{
			ret[i] = std::min(v1[i], v2[i]);
		}
		return ret;
	}

	inline bool SolveQuadratic(Float a, Float b, Float c, Float &x0, Float &x1)
	{
		Float discr = b * b - 4 * a * c;
		if (discr < 0) return false;
		else if (discr == 0) x0 = x1 = -0.5 * b / a;
		else {
			Float q = (b > 0) ?
				-0.5 * (b + sqrt(discr)) :
				-0.5 * (b - sqrt(discr));
			x0 = q / a;
			x1 = c / q;
		}
		if (x0 > x1) std::swap(x0, x1);

		return true;
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
