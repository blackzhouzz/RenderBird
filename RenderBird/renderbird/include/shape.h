#pragma once
#include "renderbird_private.h"
#include "pathtracing_common.h"

namespace RenderBird
{
	class Shape
	{
	public:
		virtual Float Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
		virtual Float GetArea() = 0;
		virtual void Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf) = 0;
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const = 0;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const = 0;
	};

}
