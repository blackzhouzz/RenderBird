#pragma once
#include "RenderbirdPrivate.h"
#include "Shape.h"
#include "SphereComponent.h"

namespace RenderBird
{
	class Sphere : public Shape
	{
	public:
		Sphere(SphereComponent* sphere);
		virtual Float Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
		virtual Float GetArea();
		virtual void Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
	private:
		SphereComponent * m_sphere;
	};
}
