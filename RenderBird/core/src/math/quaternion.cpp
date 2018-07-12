#include "math/quaternion.h"

namespace Core
{
	const Quaternion Quaternion::ZERO(0.0, 0.0, 0.0, 0.0);
	const Quaternion Quaternion::IDENTITY(1.0, 0.0, 0.0, 0.0);

	Quaternion::Quaternion(Float w1, Float x1, Float y1, Float z1)
		: x(x1), y(y1), z(z1), w(w1)
	{
	}

	Quaternion::Quaternion(Float* val)
	{
		memcpy(&w, val, sizeof(Float) * 4);
	}

	Float Quaternion::operator [] (unsigned int i)const
	{
		assert(i < 4);
		return *(&w + i);
	}

	Float& Quaternion::operator [] (unsigned int i)
	{
		assert(i < 4);
		return *(&w + i);
	}

	void Quaternion::Zero()
	{
		w = x = y = z = 0.0;
	}

	void Quaternion::Identity()
	{
		x = y = z = 0.0;
		w = 1.0;
	}

	Quaternion Quaternion::operator + (const Quaternion& quat)const
	{
		return Quaternion(w + quat.w, x + quat.x, y + quat.y, z + quat.z);
	}

	Quaternion Quaternion::operator - (const Quaternion& quat)const
	{
		return Quaternion::Quaternion(w - quat.w, x - quat.x, y - quat.y, z - quat.z);
	}

	Quaternion Quaternion::operator * (const Quaternion& quat)const
	{
		// NOTE:  Multiplication is not generally commutative, so in most
		// cases p*q != q*p.
		return Quaternion(
			w * quat.w - x * quat.x - y * quat.y - z * quat.z,
			w * quat.x + x * quat.w + y * quat.z - z * quat.y,
			w * quat.y + y * quat.w + z * quat.x - x * quat.z,
			w * quat.z + z * quat.w + x * quat.y - y * quat.x);
	}

	Quaternion Quaternion::operator * (Float scalar)const
	{
		return Quaternion(scalar*w, scalar*x, scalar*y, scalar*z);
	}

	Quaternion Quaternion::operator - ()const
	{
		return Quaternion(-w, -x, -y, -z);
	}

	bool Quaternion::operator == (const Quaternion& quat)const
	{
		return (quat.w == w) && (quat.x == x) && (quat.y == y) && (quat.z == z);
	}

	bool Quaternion::operator != (const Quaternion& quat)const
	{
		return !operator == (quat);
	}

	Float Quaternion::Dot(const Quaternion& quat)const
	{
		return w * quat.w + x * quat.x + y * quat.y + z * quat.z;
	}

	Float Quaternion::NormSQ()const
	{
		return w * w + x * x + y * y + z * z;
	}

	Float Quaternion::Normalize(void)
	{
		Float len = NormSQ();
		Float factor = 1.0 / sqrt(len);
		*this = *this * factor;
		return len;
	}

	Quaternion Quaternion::Inverse()const
	{
		Float norm = w * w + x * x + y * y + z * z;
		if (norm > 0.0)
		{
			Float invNorm = 1.0 / norm;
			return Quaternion(w * invNorm, -x * invNorm, -y * invNorm, -z * invNorm);
		}
		else
		{
			// return an invalid result to flag the error
			return ZERO;
		}
	}

	Quaternion Quaternion::UnitInverse()const
	{
		return Quaternion(w, -x, -y, -z);
	}

	Quaternion Quaternion::Exp()const
	{
		// If q = A*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// exp(q) = cos(A)+sin(A)*(x*i+y*j+z*k).  If sin(A) is nearClip zero,
		// use exp(q) = cos(A)+A*(x*i+y*j+z*k) since A/sin(A) has limit 1.

		Float angle(sqrt(x*x + y * y + z * z));
		Float sinTemp = sin(angle);

		Quaternion result;
		result.w = cos(angle);

		if (abs(sinTemp) >= C_FLOAT_EPSILON)
		{
			Float coeff = sinTemp / angle;
			result.x = coeff * x;
			result.y = coeff * y;
			result.z = coeff * z;
		}
		else
		{
			result.x = x;
			result.y = y;
			result.z = z;
		}

		return result;
	}

	Quaternion Quaternion::Log()const
	{
		// If q = cos(A)+sin(A)*(x*i+y*j+z*k) where (x,y,z) is unit length, then
		// log(q) = A*(x*i+y*j+z*k).  If sin(A) is nearClip zero, use log(q) =
		// sin(A)*(x*i+y*j+z*k) since sin(A)/A has limit 1.

		Quaternion result;
		result.w = 0.0;

		if (abs(w) < 1.0)
		{
			Float angle(acos(w));
			Float sinTemp = sin(angle);
			if (abs(sinTemp) >= C_FLOAT_EPSILON)
			{
				Float coeff = angle / sinTemp;
				result.x = coeff * x;
				result.y = coeff * y;
				result.z = coeff * z;
				return result;
			}
		}
		result.x = x;
		result.y = y;
		result.z = z;
		return result;
	}

	Float Quaternion::GetRoll(bool reprojectAxis)const
	{
		if (reprojectAxis)
		{
			Float tx = 2.0 * x;
			Float ty = 2.0 * y;
			Float tz = 2.0 * z;
			Float twz = tz * w;
			Float txy = ty * x;
			Float tyy = ty * y;
			Float tzz = tz * z;

			// Vector3f(1.0-(tyy+tzz), txy+twz, txz-twy);

			return Float(std::atan2((Float)(txy + twz), (Float)(1.0 - (tyy + tzz))));

		}
		else
		{
			return Float(std::atan2((Float)(2 * (x*y + w * z)), (Float)(w*w + x * x - y * y - z * z)));
		}
	}

	Float Quaternion::GetPitch(bool reprojectAxis)const
	{
		if (reprojectAxis)
		{
			Float tx = 2.0 * x;
			Float ty = 2.0 * y;
			Float tz = 2.0 * z;
			Float twx = tx * w;
			Float txx = tx * x;
			Float tyz = tz * y;
			Float tzz = tz * z;

			// Vector3f(txy-twz, 1.0-(txx+tzz), tyz+twx);
			return std::atan2((Float)(tyz + twx), (Float)(1.0 - (txx + tzz)));
		}
		else
		{
			// internal version
			return std::atan2((Float)(2 * (y * z + w * x)), (Float)(w * w - x * x - y * y + z * z));
		}
	}

	Float Quaternion::GetYaw(bool reprojectAxis)const
	{
		if (reprojectAxis)
		{
			// yaw = std::atan2(localz.x, localz.z)
			// pick parts of axisZ() implementation that we need
			Float tx = 2.0 * x;
			Float ty = 2.0 * y;
			Float tz = 2.0 * z;
			Float twy = ty * w;
			Float txx = tx * x;
			Float txz = tz * x;
			Float tyy = ty * y;

			// Vector3f(txz+twy, tyz-twx, 1.0-(txx+tyy));
			return std::atan2((Float)(txz + twy), (Float)(1.0 - (txx + tyy)));

		}
		else
		{
			// internal version
			return std::asin(-2 * (x * z - w * y));
		}
	}

}