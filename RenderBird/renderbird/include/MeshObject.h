#pragma once
#include "RenderbirdPrivate.h"
#include "SceneObject.h"
#include "MeshComponent.h"

namespace RenderBird
{
	class MeshObject : public SceneObject
	{
	public:
		virtual void OnCreate(EntityId id);
		virtual bool Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const;
		virtual Vector2f GetDistanceNearFar(const Vector3f& normal);
		virtual void UpdateBoundingBox();
		virtual bool CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const;
	private:
		MeshComponent* m_mesh;
	};
}