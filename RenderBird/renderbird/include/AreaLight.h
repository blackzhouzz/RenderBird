#pragma once
#include "Light.h"
#include "Shape.h"
#include "AreaLightComponent.h"

namespace RenderBird
{
	class AreaLight : public Light
	{
	public:
		AreaLight();
		virtual void OnCreate(EntityId id);
		virtual bool Sample(Sampler* sampler, SurfaceSample* ss, LightSample* ls, Float* pdf);
		virtual RGB32 Le(SurfaceSample* ss, const Vector3f& w)const;
		virtual Float Pdf(const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi)const;
		virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const;
		virtual Vector2f GetDistanceNearFar(const Vector3f& normal);
		bool IsSameHemisphere(const Vector3f& w1, const Vector3f& w2)const;
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
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const
		{
			if (m_shape != nullptr)
				return m_shape->CalcTangentSpace(hitInfo, T, B);
			return false;
		}
	public:
		std::unique_ptr<Shape> m_shape;
		AreaLightComponent* m_areaLight;
	};

}