#pragma once
#include "vector.h"

namespace Core
{
	class Sphere
	{
	public:
		Sphere() {}
		Sphere(const Vector3f& center, Float radius);
		//Sphere TransformBy(const Matrix4f& mat)const;
	public:
		Vector3f center;
		Float radius;
		static const Sphere ZERO;
	};
}