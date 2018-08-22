#pragma once
#include "math_core.h"

namespace Core
{
	class Ray
	{
	public:
		Ray()
			: m_origin(C_Zero_v3f)
			, m_direction(C_Zero_v3f)
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

		static Ray TransformBy(const Ray& ray, const Matrix4f& mat);
	};
}