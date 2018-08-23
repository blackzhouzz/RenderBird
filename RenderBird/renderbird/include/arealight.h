#pragma once
#include "renderbird_private.h"
#include "disk_component.h"

namespace RenderBird
{
	struct AreaLight
	{
		bool m_isDoubleSide;
	};

	struct AreaLightUtils
	{
		static bool SampleDisk(EntityId areaLightId, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, AreaLight);
DEFAULT_BEGIN(RenderBird, AreaLight)
DEFAULT_DATA(true)
DEFAULT_END()