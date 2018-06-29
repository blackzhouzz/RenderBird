#pragma once
#include "vector.h"

namespace MathLib
{
	class EulerRotation
	{
	public:
		EulerRotation();
		EulerRotation(const Vector3f& val);
		EulerRotation(Float pitch, Float yaw, Float roll);
		EulerRotation operator + (const EulerRotation& rotate)const;
		EulerRotation operator - (const EulerRotation& rotate)const;
		EulerRotation operator * (Float scalar)const;
		EulerRotation& operator += (const EulerRotation& rotate);
		EulerRotation& operator -= (const EulerRotation& rotate);
		bool operator == (const EulerRotation& rotate)const;
		bool operator != (const EulerRotation& rotate)const;
		bool IsZero()const;
		bool IsEqual(const EulerRotation& rotate, Float EPSILON = C_FLOAT_EPSILON)const;
		EulerRotation& Normalize();
		Float pitch;	//rotate round right axis
		Float yaw;		//rotate round up axis
		Float roll;	//rotate round look axis
		static const EulerRotation ZERO;
	};

	inline EulerRotation operator * (Float scalar, const EulerRotation& quat)
	{
		return EulerRotation(scalar * quat.pitch, scalar * quat.yaw, scalar * quat.roll);
	}

	inline EulerRotation FromRadian(Float pitch, Float yaw, Float roll)
	{
		return EulerRotation(DegToRad(pitch), DegToRad(yaw), DegToRad(roll));
	}

	inline Vector3f ToRadian(const EulerRotation& rot)
	{
		return Vector3f(DegToRad(rot.pitch), DegToRad(rot.yaw), DegToRad(rot.roll));
	}

}