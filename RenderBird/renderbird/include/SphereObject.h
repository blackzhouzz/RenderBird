#pragma once
#include "RenderbirdPrivate.h"
#include "Shape.h"
#include "SphereComponent.h"

namespace RenderBird
{
	class SphereObject : public Shape
	{
	public:
		SphereObject(SphereComponent* sphere);
		virtual Float Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
		virtual Float GetArea();
		virtual void Sample(Sampler* sampler, LightSample* ls);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const;
	private:
		SphereComponent * m_sphere;
	};
}
