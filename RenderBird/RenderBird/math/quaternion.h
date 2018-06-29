#pragma once
#include "mathcore.h"
//#include "vector.h"

namespace MathLib
{
	class Quaternion
	{
	public:
		Quaternion(Float w1 = 1.0, Float x1 = 0.0, Float y1 = 0.0, Float z1 = 0.0);
		Quaternion(const Quaternion& q);
		~Quaternion(void) {}
		Quaternion(Float* val);
		Float operator [] (unsigned int i)const;
		Float& operator [] (unsigned int i);
		void Zero();
		void Identity();
		Quaternion& operator = (const Quaternion& quat);
		Quaternion operator + (const Quaternion& quat)const;
		Quaternion operator - (const Quaternion& quat)const;
		Quaternion operator * (const Quaternion& quat)const;
		Quaternion operator * (Float scalar)const;
		friend Quaternion operator * (Float scalar, const Quaternion& quat);
		Quaternion operator - ()const;
		bool operator == (const Quaternion& quat)const;
		bool operator != (const Quaternion& quat)const;
		Float Dot(const Quaternion& quat)const;
		Float NormSQ()const;
		Float Normalize(void);
		Quaternion Inverse()const;
		Quaternion UnitInverse()const;
		Quaternion Exp()const;
		Quaternion Log()const;
		Float GetRoll(bool reprojectAxis = true)const;
		Float GetPitch(bool reprojectAxis = true)const;
		Float GetYaw(bool reprojectAxis = true)const;
	public:
		Float w, x, y, z;
		static const Quaternion ZERO;
		static const Quaternion IDENTITY;
		static const Quaternion DEFAULT;
	};
	inline Quaternion operator * (Float scalar, const Quaternion& quat)
	{
		return Quaternion(scalar*quat.w, scalar*quat.x, scalar*quat.y, scalar*quat.z);
	}
}