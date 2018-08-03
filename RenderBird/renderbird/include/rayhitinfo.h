#pragma once
#include "renderbird_private.h"
#include "trianglemesh.h"

namespace RenderBird
{
	class Material;

	struct RayHitInfo
	{
		RayHitInfo()
			: m_position(Vector3f::ZERO)
			, m_normal(Vector3f::ZERO)
			, m_material(nullptr)
			, m_geomNormal(Vector3f::ZERO)
			, m_u(0)
			, m_v(0)
			, m_t(0)
			, m_hasHit(false)
		{
		}
		Vector3f m_position;
		Vector3f m_normal;
		Vector3f m_geomNormal;
		Material* m_material;
		Float m_u;
		Float m_v;
		Float m_t;
		bool m_hasHit;
	};
}