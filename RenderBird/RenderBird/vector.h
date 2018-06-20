#pragma once
#include "mathcore.h"

namespace MathLib
{
#pragma region
	template<typename T>
	class TVector2
	{
	public:
		TVector2();
		TVector2(T x1, T y1);
		TVector2<T> operator + (const TVector2<T>& val)const;
		TVector2<T>& operator += (const TVector2<T>& val);
		TVector2<T> operator - ()const;
		TVector2<T> operator - (const TVector2<T>& val)const;
		TVector2<T>& operator -= (const TVector2<T>& val);
		TVector2<T> operator * (const TVector2<T>& val)const;
		TVector2<T>& operator *= (const TVector2<T>& val);
		TVector2<T> operator * (Float val)const;
		TVector2<T>& operator *= (Float val);
		TVector2<T> operator / (const TVector2<T>& val)const;
		TVector2<T>& operator /= (const TVector2<T>& val);
		bool operator == (const TVector2<T>& val)const;
		bool operator != (const TVector2<T>& val)const;
		T operator [] (const size_t& i)const;
		T& operator [] (const size_t& i);
		Float Length()const;
		Float LengthSQ()const;
		TVector2<T>& Normalize();
		TVector2<T>& Rotate(Float angle);
		bool IsNaN()const;

		T x, y;
		static const TVector2<T> ZERO;
		static const TVector2<T> ONE;
	};

	using Vector2i = TVector2<int>;
	using Vector2f = TVector2<Float>;

	template <typename T>
	const TVector2<T> TVector2<T>::ZERO(0, 0);

	template <typename T>
	const TVector2<T> TVector2<T>::ONE(1, 1);

	template<typename T>
	TVector2<T>::TVector2()
	{
	}

	template<typename T>
	TVector2<T>::TVector2(T x1, T y1)
		: x(x1), y(y1)
	{
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator + (const TVector2<T>& val)const
	{
		return TVector2<T>(x + val.x, y + val.y);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::operator += (const TVector2<T>& val)
	{
		x += val.x;
		y += val.y;
		return *this;
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator - ()const
	{
		return TVector2<T>(-x, -y);
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator - (const TVector2<T>& val)const
	{
		return TVector2<T>(x - val.x, y - val.y);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::operator -= (const TVector2<T>& val)
	{
		x -= val.x;
		y -= val.y;
		return *this;
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator * (const TVector2<T>& val)const
	{
		return TVector2<T>(x * val.x, y * val.y);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::operator *= (const TVector2<T>& val)
	{
		x *= val.x;
		y *= val.y;
		return *this;
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator * (Float val)const
	{
		return TVector2<T>(x * val, y * val);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::operator *= (Float val)
	{
		x *= val;
		y *= val;
		return *this;
	}

	template<typename T>
	inline TVector2<T> TVector2<T>::operator / (const TVector2<T>& val)const
	{
		return TVector2<T>(x / val.x, y / val.y);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::operator /= (const TVector2<T>& val)
	{
		x /= val.x;
		y /= val.y;
		return *this;
	}

	template<typename T>
	inline bool TVector2<T>::operator == (const TVector2<T>& val)const
	{
		return x == val.x && y == val.y;
	}

	template<typename T>
	inline bool TVector2<T>::operator != (const TVector2<T>& val)const
	{
		return x != val.x || y != val.y;
	}

	template<typename T>
	inline T TVector2<T>::operator [] (const size_t& i)const
	{
		assert(i < 2);
		return *(&x + i);
	}

	template<typename T>
	inline T& TVector2<T>::operator [] (const size_t& i)
	{
		assert(i < 2);
		return *(&x + i);
	}

	template<typename T>
	inline Float TVector2<T>::Length()const
	{
		return (Float)sqrt(x * x + y * y);
	}

	template<typename T>
	inline Float TVector2<T>::LengthSQ()const
	{
		return (x * x + y * y);
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::Normalize()
	{
		Float len = Length();
		if (len == 0)
			return *this;
		len = 1.0 / len;
		x *= len;
		y *= len;
		return *this;
	}

	template<typename T>
	inline TVector2<T>& TVector2<T>::Rotate(Float angle)
	{
		Float xt = (x * std::cos(angle)) - (y * std::cos(angle));
		Float yt = (y * std::cos(angle)) + (x * std::cos(angle));
		x = (T)xt;
		y = (T)yt;
		return *this;
	}

	template<typename T>
	inline bool TVector2<T>::IsNaN()const
	{
		return std::isnan(x) || std::isnan(y);
	}

	template <typename T>
	inline T Dot(const TVector2<T> v1, const TVector2<T> v2)
	{
		return (v1.x * v2.x + v1.y * v2.y);
	}

	template <typename T>
	inline T AbsDot(const TVector2<T> v1, const TVector2<T> v2)
	{
		return std::abs(v1.x * v2.x + v1.y * v2.y);
	}

	template <typename T>
	inline T Cross(const TVector2<T> v1, const TVector2<T> v2)
	{
		return (v1.x * v2.y - v1.y * v2.x);
	}

	template <typename T>
	inline bool IsEqual(const TVector2<T>& v1, const TVector2<T>& v2, Float epsilon = C_FLOAT_EPSILON_HIGH)
	{
		return ((std::abs(v1.x - v2.x) < epsilon)
			&& (std::abs(v1.y - v2.y) < epsilon)
			);
	}
#pragma endregion TVector2

#pragma region
	template<typename T>
	class TVector3
	{
	public:
		TVector3();
		TVector3(T x1, T y1, T z1);
		TVector3<T> operator + (const TVector3<T>& val)const;
		TVector3<T>& operator += (const TVector3<T>& val);
		TVector3<T> operator - ()const;
		TVector3<T> operator - (const TVector3<T>& val)const;
		TVector3<T>& operator -= (const TVector3<T>& val);
		TVector3<T> operator * (const TVector3<T>& val)const;
		TVector3<T>& operator *= (const TVector3<T>& val);
		TVector3<T> operator * (Float val)const;
		TVector3<T>& operator *= (Float val);
		TVector3<T> operator / (const TVector3<T>& val)const;
		TVector3<T>& operator /= (const TVector3<T>& val);
		bool operator == (const TVector3<T>& val)const;
		bool operator != (const TVector3<T>& val)const;
		T operator [] (const size_t& i)const;
		T& operator [] (const size_t& i);
		Float Length()const;
		Float LengthSQ()const;
		TVector3<T>& Normalize();
		TVector3<T> GetNormalized()const;
		bool IsNaN()const;
		bool IsNormalized()const;
		void Floor(const TVector3<T>& v);
		void Ceil(const TVector3<T>& v);

		T x, y, z;
		static const TVector3<T> ZERO;
		static const TVector3<T> UNIT_X;
		static const TVector3<T> UNIT_Y;
		static const TVector3<T> UNIT_Z;
		static const TVector3<T> ONE;
	};

	using Vector3i = TVector3<int>;
	using Vector3f = TVector3<Float>;

	template <typename T>
	const TVector3<T> TVector3<T>::ZERO(0, 0, 0);

	template <typename T>
	const TVector3<T> TVector3<T>::UNIT_X(1, 0, 0);

	template <typename T>
	const TVector3<T> TVector3<T>::UNIT_Y(0, 1, 0);

	template <typename T>
	const TVector3<T> TVector3<T>::UNIT_Z(0, 0, 1);

	template <typename T>
	const TVector3<T> TVector3<T>::ONE(1, 1, 1);

	template<typename T>
	TVector3<T>::TVector3()
	{
	}

	template<typename T>
	TVector3<T>::TVector3(T x1, T y1, T z1)
		: x(x1)
		, y(y1)
		, z(z1)
	{
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator + (const TVector3<T>& val)const
	{
		return TVector3<T>(x + val.x, y + val.y, z + val.z);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::operator += (const TVector3<T>& val)
	{
		x += val.x;
		y += val.y;
		z += val.z;
		return *this;
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator - ()const
	{
		return TVector3<T>(-x, -y, -z);
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator - (const TVector3<T>& val)const
	{
		return TVector3<T>(x - val.x, y - val.y, z - val.z);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::operator -= (const TVector3<T>& val)
	{
		x -= val.x;
		y -= val.y;
		z -= val.z;
		return *this;
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator * (const TVector3<T>& val)const
	{
		return TVector3<T>(x * val.x, y * val.y, z * val.z);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::operator *= (const TVector3<T>& val)
	{
		x *= val.x;
		y *= val.y;
		z *= val.z;
		return *this;
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator * (Float val)const
	{
		return TVector3<T>(x * val, y * val, z * val);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::operator *= (Float val)
	{
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::operator / (const TVector3<T>& val)const
	{
		return TVector3<T>(x / val.x, y / val.y, z / val.z);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::operator /= (const TVector3<T>& val)
	{
		x /= val.x;
		y /= val.y;
		z /= val.z;
		return *this;
	}

	template<typename T>
	inline bool TVector3<T>::operator == (const TVector3<T>& val)const
	{
		return x == val.x && y == val.y && z == val.z;
	}

	template<typename T>
	inline bool TVector3<T>::operator != (const TVector3<T>& val)const
	{
		return x != val.x || y != val.y || z != val.z;
	}

	template<typename T>
	inline T TVector3<T>::operator [] (const size_t& i)const
	{
		assert(i < 3);
		return *(&x + i);
	}

	template<typename T>
	inline T& TVector3<T>::operator [] (const size_t& i)
	{
		assert(i < 3);
		return *(&x + i);
	}

	template<typename T>
	inline Float TVector3<T>::Length()const
	{
		return (Float)sqrt(x * x + y * y + z * z);
	}

	template<typename T>
	inline Float TVector3<T>::LengthSQ()const
	{
		return (x * x + y * y + z * z);
	}

	template<typename T>
	inline TVector3<T>& TVector3<T>::Normalize()
	{
		Float len = Length();
		if (len == 0)
			return *this;
		len = 1.0 / len;
		x *= len;
		y *= len;
		z *= len;
		return *this;
	}

	template<typename T>
	inline TVector3<T> TVector3<T>::GetNormalized()const
	{
		TVector3<T> v = TVector3<T>.ZERO;
		Float len = Length();
		if (len == 0)
			return v;
		len = 1.0 / len;
		v.x = x * len;
		v.y = y * len;
		v.z = z * len;
		return v;
	}

	template<typename T>
	inline bool TVector3<T>::IsNaN()const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z);
	}

	template<typename T>
	inline bool TVector3<T>::IsNormalized()const
	{
		return std::abs(LengthSQ() - 1.0) < C_FLOAT_EPSILON;
	}

	template<typename T>
	inline void TVector3<T>::Floor(const TVector3<T>& v)
	{
		x = v.x < x ? v.x : x;
		y = v.y < y ? v.y : y;
		y = v.z < z ? v.z : z;
	}

	template<typename T>
	inline void TVector3<T>::Ceil(const TVector3<T>& v)
	{
		x = v.x > x ? v.x : x;
		y = v.y > y ? v.y : y;
		y = v.z > z ? v.z : z;
	}

	template <typename T>
	inline T Dot(const TVector3<T> v1, const TVector3<T> v2)
	{
		return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	template <typename T>
	inline T AbsDot(const TVector3<T> v1, const TVector3<T> v2)
	{
		return std::abs(v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
	}

	template <typename T>
	inline TVector3<T> Cross(const TVector3<T> v1, const TVector3<T> v2)
	{
		return TVector3<T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x);
	}

	template <typename T>
	inline T Distance(const TVector3<T> v1, const TVector3<T> v2)
	{
		return (v1 - v2).Length();
	}

	template <typename T>
	inline T DistanceSQ(const TVector3<T> v1, const TVector3<T> v2)
	{
		return (v1 - v2).LengthSQ();
	}

	template <typename T>
	inline TVector3<T> ReflectVector(const TVector3<T>& v, const TVector3<T>& n)
	{
		return v - 2 * (Dot(v, n)) * n;
	}

	template <typename T>
	inline TVector3<T> ProjectVector(const TVector3<T>& v, const TVector3<T>& n)
	{
		return n * Dot(v, n) / n.LengthSQ();
	}

	template <typename T>
	inline TVector3<T> OrthoVector(const TVector3<T>& v, const TVector3<T>& n)
	{
		return v - ProjectVector(v, n);
	}

	template <typename T>
	inline TVector3<T> SnapVector(const TVector3<T>& vec, Float val)
	{
		return TVector3<T>(vec.x - std::fmod(vec.x, val), vec.y - std::fmod(vec.y, val), vec.z - std::fmod(vec.z, val));
	}

	template <typename T>
	inline TVector3<T> SnapVector(const TVector3<T>& vec, const TVector3<T>& val)
	{
		return TVector3<T>(vec.x - std::fmod(vec.x, val.x), vec.y - std::fmod(vec.y, val.y), vec.z - std::fmod(vec.z, val.z));
	}

	template <typename T>
	inline TVector3<T> SlerpVector(const TVector3<T>& a, const TVector3<T>& b, Float t)
	{
		TVector3<T> ret = TVector3<T>::ZERO;
		Float angle = std::acos(Dot(a, b));

		if (0.0 == angle)
		{
			ret[0] = a[0];
			ret[1] = a[1];
			ret[2] = a[2];
		}
		else if (C_PI == angle)
		{
			ret[0] = 0;
			ret[1] = 0;
			ret[2] = 0;
		}
		else
		{
			ret[0] = (std::sin((1.0 - t)*angle) / std::sin(angle))*a[0] + (std::sin(t*angle) / std::sin(angle))*b[0];
			ret[1] = (std::sin((1.0 - t)*angle) / std::sin(angle))*a[1] + (std::sin(t*angle) / std::sin(angle))*b[1];
			ret[2] = (std::sin((1.0 - t)*angle) / std::sin(angle))*a[2] + (std::sin(t*angle) / std::sin(angle))*b[2];
		}
		return ret;
	}

	template <typename T>
	inline bool IsEqual(const TVector3<T>& v1, const TVector3<T>& v2, Float epsilon = C_FLOAT_EPSILON_HIGH)
	{
		return ((std::abs(v1.x - v2.x) < epsilon)
			&& (std::abs(v1.y - v2.y) < epsilon)
			&& (std::abs(v1.z - v2.z) < epsilon)
			);
	}

	template <typename T>
	inline TVector3<T> AbsVector(const TVector3<T>& vec)
	{
		return TVector3<T>(std::abs(vec.x), std::abs(vec.y), std::abs(vec.z));
	}

#pragma endregion TVector3

#pragma region
	template<typename T>
	class TVector4
	{
	public:
		TVector4();
		TVector4(T x1, T y1, T z1, T w1);
		TVector4<T> operator + (const TVector4<T>& val)const;
		TVector4<T>& operator += (const TVector4<T>& val);
		TVector4<T> operator - ()const;
		TVector4<T> operator - (const TVector4<T>& val)const;
		TVector4<T>& operator -= (const TVector4<T>& val);
		TVector4<T> operator * (const TVector4<T>& val)const;
		TVector4<T>& operator *= (const TVector4<T>& val);
		TVector4<T> operator * (Float val)const;
		TVector4<T>& operator *= (Float val);
		TVector4<T> operator / (const TVector4<T>& val)const;
		TVector4<T>& operator /= (const TVector4<T>& val);
		bool operator == (const TVector4<T>& val)const;
		bool operator != (const TVector4<T>& val)const;
		T operator [] (const size_t& i)const;
		T& operator [] (const size_t& i);
		bool IsNaN()const;

		T x, y, z, w;
		static const TVector4<T> ZERO;
		static const TVector4<T> ONE;
	};

	using Vector4i = TVector4<int>;
	using Vector4f = TVector4<Float>;

	template <typename T>
	const TVector4<T> TVector4<T>::ZERO(0, 0, 0, 0);

	template <typename T>
	const TVector4<T> TVector4<T>::ONE(1, 1, 1, 1);

	template<typename T>
	TVector4<T>::TVector4()
	{
	}

	template<typename T>
	TVector4<T>::TVector4(T x1, T y1, T z1, T w1)
		: x(x1), y(y1), z(z1), w(w1)
	{
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator + (const TVector4<T>& val)const
	{
		return TVector4<T>(x + val.x, y + val.y, z + val.z, w + val.w);
	}

	template<typename T>
	inline TVector4<T>& TVector4<T>::operator += (const TVector4<T>& val)
	{
		x += val.x;
		y += val.y;
		z += val.z;
		w += val.w;
		return *this;
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator - ()const
	{
		return TVector4<T>(-x, -y, -z, -w);
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator - (const TVector4<T>& val)const
	{
		return TVector4<T>(x - val.x, y - val.y, z - val.z, w - val.w);
	}

	template<typename T>
	inline TVector4<T>& TVector4<T>::operator -= (const TVector4<T>& val)
	{
		x -= val.x;
		y -= val.y;
		z -= val.z;
		w -= val.w;
		return *this;
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator * (const TVector4<T>& val)const
	{
		return TVector4<T>(x * val.x, y * val.y, z * val.z, w * val.w);
	}

	template<typename T>
	inline TVector4<T>& TVector4<T>::operator *= (const TVector4<T>& val)
	{
		x *= val.x;
		y *= val.y;
		z *= val.z;
		w *= val.w;
		return *this;
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator * (Float val)const
	{
		return TVector4<T>(x * val, y * val, z * val, w * val);
	}

	template<typename T>
	inline TVector4<T>& TVector4<T>::operator *= (Float val)
	{
		x *= val;
		y *= val;
		z *= val;
		w *= val;
		return *this;
	}

	template<typename T>
	inline TVector4<T> TVector4<T>::operator / (const TVector4<T>& val)const
	{
		return TVector4<T>(x / val.x, y / val.y, z / val.z, w / val.w);
	}

	template<typename T>
	inline TVector4<T>& TVector4<T>::operator /= (const TVector4<T>& val)
	{
		x /= val.x;
		y /= val.y;
		z /= val.z;
		w /= val.w;
		return *this;
	}

	template<typename T>
	inline bool TVector4<T>::operator == (const TVector4<T>& val)const
	{
		return x == val.x && y == val.y&& z == val.z&& w == val.w;
	}

	template<typename T>
	inline bool TVector4<T>::operator != (const TVector4<T>& val)const
	{
		return x != val.x || y != val.y || z != val.z || w != val.w;
	}

	template<typename T>
	inline T TVector4<T>::operator [] (const size_t& i)const
	{
		assert(i < 4);
		return *(&x + i);
	}

	template<typename T>
	inline T& TVector4<T>::operator [] (const size_t& i)
	{
		assert(i < 4);
		return *(&x + i);
	}

	template<typename T>
	inline bool TVector4<T>::IsNaN()const
	{
		return std::isnan(x) || std::isnan(y) || std::isnan(z) || std::isnan(w);
	}

	template <typename T>
	inline bool IsEqual(const TVector4<T>& v1, const TVector4<T>& v2, Float epsilon = C_FLOAT_EPSILON_HIGH)
	{
		return ((std::abs(v1.x - v2.x) < epsilon)
			&& (std::abs(v1.y - v2.y) < epsilon)
			&& (std::abs(v1.z - v2.z) < epsilon)
			&& (std::abs(v1.w - v2.w) < epsilon)
			);
	}
#pragma endregion TVector4

}
