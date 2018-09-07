#pragma once
#include "renderbird_private.h"
#include "scene_object.h"
#include "mesh_component.h"

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