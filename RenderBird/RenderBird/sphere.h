#pragma once
#include "LNVector.h"

namespace Luna
{
	class Sphere
	{
	public:
		Sphere() {}
		Sphere(const Vector3Float& center, float radius);
		//Sphere TransformBy(const Matrix4f& mat)const;
	public:
		Vector3Float mCenter;
		float mRadius;
		static const Sphere ZERO;
	};
}