#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"
#include "disk_component.h"

namespace RenderBird
{
	struct LightPropertyComponent
	{
		RGB32 m_color;
		Float m_intensity;
	};

}

DECLEAR_TYPE_COMPONENT(RenderBird, LightPropertyComponent);
DEFAULT_BEGIN(RenderBird, LightPropertyComponent)
DEFAULT_DATA(RGB32::WHITE)
DEFAULT_DATA(1.0f)
DEFAULT_END()

