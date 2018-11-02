#pragma once
#include "Light.h"
#include "DistantLightComponent.h"

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
		virtual bool Sample(Sampler* sampler, SurfaceSample* ss, LightSample* ls, Float* pdf);
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const { return 0.0; }
		virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const { return true; }
	protected:
		DistantLightComponent* m_distantLight;
	};
}