#pragma once
#include "Shape.h"
#include "TriangleMesh.h"

namespace RenderBird
{
	class Triangle : public Shape
	{
	public:
		Triangle(TriangleMesh* trimesh, uint32 faceIndex);
		virtual Float GetArea();
		virtual void Sample(const Vector2f& rand2d, LightSample* ls, Float* pdf);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		virtual BoundingBox GetBoundingBox(const Matrix4f& localToWorld)const;
		virtual Vector2f GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal);
	private:
		TriangleMesh * m_trimesh;
		uint32 m_faceIndex;
		const TriangleMesh::FaceData* m_face;
		const Vector3f* m_v0;
		const Vector3f* m_v1;
		const Vector3f* m_v2;
		Float m_area;
	};
}