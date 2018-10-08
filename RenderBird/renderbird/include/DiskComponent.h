#pragma once
#include "RenderbirdPrivate.h"
#include "SampleUtils.h"
#include "PathTracingCommon.h"

namespace RenderBird
{
	struct DiskComponent
	{
		Float m_radius;
		Float m_innerRadius;
		Float m_phiMax;
	};

	//struct DiskComponentUtils
	//{
	//	static Float GetArea(DiskComponent* comp);
	//	static bool Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo);
	//};
}

DECLEAR_TYPE_COMPONENT(RenderBird, DiskComponent);
DEFAULT_BEGIN(RenderBird, DiskComponent)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_END()