#pragma once
#include "renderbird_private.h"
#include "object.h"
#include "math/ray.h"
#include "rayhitinfo.h"
#include "trianglemesh.h"

namespace RenderBird
{
	class Scene : public Object
	{
	public:
		bool Intersect(const Ray& ray, RayHitInfo* hitInfo);
		void SetupSceneTest();
		void CreateLightTest();
		void CreateMeshTest();
		void CreateCameraTest();
		EntityId GetCamera()const { return m_camera; }
		void AddTriangleMesh(TriangleMesh* mesh);
		void AddTestDiskLight(const Vector3f& pos);
	private:
		std::set<EntityId> m_entities;
		ComponentGroup* m_meshEntitiesGroup;
		ComponentGroup* m_lightsGroup;
		std::vector<TriangleMesh*> m_meshResources;
		EntityId m_camera;
		EntityId m_diskId;
	};
}