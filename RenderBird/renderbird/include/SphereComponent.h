#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"

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
