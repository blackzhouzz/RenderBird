#include "core_private.h"
#include "math/euler_rotation.h"

namespace Core
{
	const EulerRotation EulerRotation::ZERO = EulerRotation(0, 0, 0);

	EulerRotation::EulerRotation(Float pitch, Float yaw, Float roll)
		: pitch(pitch)
		, yaw(yaw)
		, roll(roll)
	{
	}

	EulerRotation::EulerRotation(const Vector3f& val)
		: pitch(val.x)
		, yaw(val.y)
		, roll(val.z)
	{
	}

	EulerRotation EulerRotation::operator + (const EulerRotation& rotate)const
	{
		return EulerRotation(pitch + rotate.pitch, yaw + rotate.yaw, roll + rotate.roll);
	}

	EulerRotation EulerRotation::operator - (const EulerRotation& rotate)const
	{
		return EulerRotation(pitch - rotate.pitch, yaw - rotate.yaw, roll - rotate.roll);
	}

	EulerRotation EulerRotation::operator * (Float scalar)const
	{
		return EulerRotation(pitch * scalar, yaw * scalar, roll * scalar);
	}

	EulerRotation& EulerRotation::operator += (const EulerRotation& rotate)
	{
		pitch += rotate.pitch;
		yaw += rotate.yaw;
		roll += rotate.roll;
		return *this;
	}

	EulerRotation& EulerRotation::operator -= (const EulerRotation& rotate)
	{
		pitch -= rotate.pitch;
		yaw -= rotate.yaw;
		roll -= rotate.roll;
		return *this;
	}

	bool EulerRotation::operator == (const EulerRotation& rotate)const
	{
		return IsEqual(rotate);
	}

	bool EulerRotation::operator != (const EulerRotation& rotate)const
	{
		return !operator == (rotate);
	}

	bool EulerRotation::IsZero()const
	{
		return ((std::abs(pitch) < C_FLOAT_EPSILON) && (std::abs(yaw) < C_FLOAT_EPSILON) && (std::abs(roll) < C_FLOAT_EPSILON));
	}

	bool EulerRotation::IsEqual(const EulerRotation& rotate, Float EPSILON)const
	{
		return ((std::abs(pitch - rotate.pitch) < EPSILON) && (std::abs(yaw - rotate.yaw) < EPSILON) && (std::abs(roll - rotate.roll) < EPSILON));
	}

	EulerRotation& EulerRotation::Normalize()
	{
		pitch = AngleModEuler(pitch);
		yaw = AngleModEuler(yaw);
		roll = AngleModEuler(roll);
		return *this;
	}

}