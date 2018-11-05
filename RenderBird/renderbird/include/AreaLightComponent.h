#pragma once
#include "RenderbirdPrivate.h"
#include "DiskComponent.h"

namespace RenderBird
{
	struct AreaLightComponent
	{
		bool m_isDoubleSide;
	};

}

DECLEAR_TYPE_COMPONENT(RenderBird, AreaLightComponent);
DEFAULT_BEGIN(RenderBird, AreaLightComponent)
DEFAULT_DATA(true)
DEFAULT_END()