#pragma once
#include "light.h"
#include "shape.h"
#include "area_light_component.h"

namespace RenderBird
{
	class AreaLight : public Light
	{
	public:
		AreaLight();
		virtual void OnCreate(EntityId id);
		virtual bool Sample(const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf);
		virtual RGB32 Le(SurfaceSample* ss, const Vector3f& w)const;
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const;
		virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const;
		virtual Vector2f GetDistanceNearFar(const Vector3f& normal);
		void SetShape(std::unique_ptr<Shape> shape)
		{
			m_shape = std::move(shape);
			UpdateBoundingBox();
		}
		Shape* GetShape()const
		{
			return m_shape.get();
		}
		virtual void UpdateBoundingBox();
	public:
		std::unique_ptr<Shape> m_shape;
		AreaLightComponent* m_areaLight;
	};

}