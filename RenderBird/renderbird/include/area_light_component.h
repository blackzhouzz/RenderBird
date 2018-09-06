#pragma once
#include "renderbird_private.h"
#include "disk_component.h"

namespace RenderBird
{
	struct AreaLightComponent
	{
		bool m_isDoubleSide;
	};

	//struct AreaLightUtils
	//{
	//	static bool SampleDisk(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf);
	//	static Float PdfDisk(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
	//	static RGB32 Le(EntityId id, SurfaceSample* ss, const Vector3f& w);
	//};
}

DECLEAR_TYPE_COMPONENT(RenderBird, AreaLightComponent);
DEFAULT_BEGIN(RenderBird, AreaLightComponent)
DEFAULT_DATA(true)
DEFAULT_END()