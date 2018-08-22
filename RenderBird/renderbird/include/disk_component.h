#pragma once
#include "renderbird_private.h"
#include "sample_utils.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	struct DiskComponent
	{
		Float m_radius;
		Float m_innerRadius;
		Float m_phiMax;
	};

	struct DiskComponentUtils
	{
		static Float GetArea(DiskComponent* comp);
		static bool Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, DiskComponent);
DEFAULT_BEGIN(RenderBird, DiskComponent)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_END()