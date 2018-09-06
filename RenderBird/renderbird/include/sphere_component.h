#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	struct SphereComponent
	{
		Float m_radius;
	};

	//struct SphereComponentUtils
	//{
	//	static Float GetArea(SphereComponent* disk);
	//	static bool Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo);

	//};
}

DECLEAR_TYPE_COMPONENT(RenderBird, SphereComponent);
DEFAULT_BEGIN(RenderBird, SphereComponent)
DEFAULT_DATA(1.0)
DEFAULT_END()
