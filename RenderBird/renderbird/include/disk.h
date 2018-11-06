#pragma once
#include "RenderbirdPrivate.h"
#include "Shape.h"
#include "DiskComponent.h"

namespace RenderBird
{
	class Disk : public Shape
	{
	public:
		Disk(DiskComponent* disk);
		virtual Float GetArea();
		virtual void Sample(Sampler* sampler, LightSample* ls);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const;
	private:
		DiskComponent* m_disk;
	};
}
