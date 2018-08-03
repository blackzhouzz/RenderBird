#pragma once
#include "vector.h"

namespace Core
{
	class Ray
	{
	public:
		Vector3f origin;
		Vector3f direction;
		Float m_time;
		Vector3f GetPoint(Float t)const
		{
			return origin + direction * t; 
		}
		static const Float MaxT;
	};
}