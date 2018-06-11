#pragma once
#include "LNVector.h"

namespace Luna
{
	class Ray
	{
	public:
		Ray(const Vector3Float& origin, const Vector3Float& dir)
			: mOrigin(origin), mDirection(dir)
		{
		}
		Vector3Float mOrigin;
		Vector3Float mDirection;
	};
}