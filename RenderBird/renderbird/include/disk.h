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
		virtual Float GetArea();
		virtual void Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
	private:
		DiskComponent* m_disk;
	};
}
