#pragma once
#include "vector.h"

namespace Core
{
	class Ray
	{
	public:
		Vector3f origin;
		Vector3f direction;
		float m_time;
	};
}