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
	private:
		MeshComponent* m_mesh;
	};
}