#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	class Material;

	struct RayHitInfo
	{
		RayHitInfo()
		{
		}
		Vector3f m_position;
		Vector3f m_normal;
		Material* m_material;
		Float m_u;
		Float m_v;
		Float m_t;
	};
}