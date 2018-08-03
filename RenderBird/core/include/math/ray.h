#pragma once
#include "vector.h"

namespace Core
{
	class Ray
	{
	public:
		Ray()
			: m_origin(Vector3f::ZERO)
			, m_direction(Vector3f::ZERO)
			, m_maxT(DefaultMaxT)
		{
		}
		Vector3f m_origin;
		Vector3f m_direction;
		Float m_maxT;
		Vector3f GetPoint(Float t)const
		{
			return m_origin + m_direction * t;
		}
		static const Float DefaultMaxT;
	};
}