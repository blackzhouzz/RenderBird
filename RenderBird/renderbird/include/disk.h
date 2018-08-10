#pragma once
#include "renderbird_private.h"
#include "rayhitinfo.h"
#include "sample_utils.h"

namespace RenderBird
{
	struct Disk
	{
		Float m_radius;
		Float m_innerRadius;
		Float m_phiMax;
	};

	struct DiskUtils
	{
		static Float GetArea(Disk* disk);
		static bool Intersect(Disk* disk, const Matrix4f& objToWorld, const Ray& worldRay, RayHitInfo* hitInfo);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, Disk);
DEFAULT_BEGIN(RenderBird, Disk)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_DATA(0.0f)
DEFAULT_END()