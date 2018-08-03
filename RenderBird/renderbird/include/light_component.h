#pragma once
#include "renderbird_private.h"

namespace RenderBird
{
	struct LightProperty
	{
		RGBA8 m_color;
		Float m_intensity;
	};

	struct DirectionalLight
	{

	};

	struct AreaLight
	{
		TriangleMesh* m_mesh;
		Float m_area;
		bool m_doubleSide;
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, LightProperty);
DEFAULT_BEGIN(RenderBird, LightProperty)
DEFAULT_DATA(RGBA8::WHITE)
DEFAULT_DATA(1.0)
DEFAULT_END()

DECLEAR_TYPE_COMPONENT(RenderBird, DirectionalLight);
DEFAULT_BEGIN(RenderBird, DirectionalLight)
DEFAULT_END()


DECLEAR_TYPE_COMPONENT(RenderBird, AreaLight);
DEFAULT_BEGIN(RenderBird, AreaLight)
DEFAULT_DATA(nullptr)
DEFAULT_DATA(0.0)
DEFAULT_DATA(true)
DEFAULT_END()