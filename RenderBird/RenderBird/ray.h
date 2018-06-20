#pragma once
#include "vector.h"

namespace MathLib
{
	class Ray
	{
	public:
		Ray(const Vector3f& o, const Vector3f& d)
			: origin(o), direction(d)
		{
		}
		Vector3f origin;
		Vector3f direction;
	};
}