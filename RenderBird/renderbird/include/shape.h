#pragma once
#include "RenderbirdPrivate.h"
#include "PathTracingCommon.h"
#include "Sampler.h"

namespace RenderBird
{
	class Shape
	{
	public:
		virtual Float Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
		virtual Float GetArea() = 0;
		virtual void Sample(Sampler* sampler, LightSample* ls) = 0;
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const = 0;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const = 0;
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const = 0;
	};

}
