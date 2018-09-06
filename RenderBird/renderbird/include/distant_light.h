#pragma once
#include "light.h"
#include "distant_light_component.h"

namespace RenderBird
{
	class DistantLight : public Light
	{
	public:
		DistantLight()
			: m_distantLight(nullptr)
		{
		}
		virtual void OnCreate(EntityId id);
		virtual bool Sample(const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf);
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const { return 1.0; }
		//virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const { return false; }
	protected:
		DistantLightComponent* m_distantLight;
	};
}