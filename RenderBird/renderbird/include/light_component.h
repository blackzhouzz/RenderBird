#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	enum class LightType
	{

	};

	struct LightProperty
	{
		RGB32 m_color;
		Float m_intensity;
	};

	struct DistantLightComponent
	{

	};

	struct DistantLightComponentUtils
	{
		static bool Sample(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, LightProperty);
DEFAULT_BEGIN(RenderBird, LightProperty)
DEFAULT_DATA(RGB32::WHITE)
DEFAULT_DATA(1.0f)
DEFAULT_END()

DECLEAR_TYPE_COMPONENT(RenderBird, DistantLightComponent);
DEFAULT_BEGIN(RenderBird, DistantLightComponent)
DEFAULT_END()