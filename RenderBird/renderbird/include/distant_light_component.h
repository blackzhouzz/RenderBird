#pragma once
#include "renderbird_private.h"
#include "light_component.h"

namespace RenderBird
{
	struct DistantLightComponent
	{

	};

	//struct DistantLightComponentUtils
	//{
	//	static bool Sample(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf);
	//};
}


DECLEAR_TYPE_COMPONENT(RenderBird, DistantLightComponent);
DEFAULT_BEGIN(RenderBird, DistantLightComponent)
DEFAULT_END()
