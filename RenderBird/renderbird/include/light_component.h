#pragma once
#include "renderbird_private.h"

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

	struct DirectionalLight
	{

	};

}

DECLEAR_TYPE_COMPONENT(RenderBird, LightProperty);
DEFAULT_BEGIN(RenderBird, LightProperty)
DEFAULT_DATA(RGB32::WHITE)
DEFAULT_DATA(1.0f)
DEFAULT_END()

DECLEAR_TYPE_COMPONENT(RenderBird, DirectionalLight);
DEFAULT_BEGIN(RenderBird, DirectionalLight)
DEFAULT_END()