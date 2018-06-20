#pragma once
#include "vector.h"
using namespace MathLib;

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
	};
}