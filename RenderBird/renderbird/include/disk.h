#pragma once
#include "renderbird_private.h"
#include "shape.h"
#include "disk_component.h"

namespace RenderBird
{
	class Disk : public Shape
	{
	public:
		Disk(DiskComponent* disk);
		virtual Float Pdf(const Matrix4f& localToWorld, const RayHitInfo& hitInfo, SurfaceSample* ss, const Vector3f& wi);
		virtual Float GetArea();
		virtual bool Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
	private:
		DiskComponent* m_disk;
	};
}
