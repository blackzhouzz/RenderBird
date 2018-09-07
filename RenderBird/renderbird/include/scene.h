#pragma once
#include "renderbird_private.h"
#include "object.h"
#include "trianglemesh.h"
#include "light.h"
#include "accel_structure.h"

namespace RenderBird
{
	class Scene : public Object
	{
	public:
		Scene();
		bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
		void SetupSceneTest();
		void CreateLightTest();
		void CreateMeshTest();
		void CreateCameraTest();
		void CreateShapeTest();
		EntityId GetCamera()const { return m_camera; }
		void AddTriangleMesh(TriangleMesh* mesh);
		void AddTestDiskLight(const Vector3f& pos, Float radius);
		void AddTestSphereLight(const Vector3f& pos, Float radius);
		void AddTestMeshLight(const Vector3f& pos, TriangleMesh* trimesh);
		bool IsLight(EntityId id);
	public:
		std::set<EntityId> m_entities;
		//ComponentGroup* m_sphereEntitiesGroup;
		//ComponentGroup* m_meshEntitiesGroup;
		//ComponentGroup* m_lightsGroup;
		std::vector<TriangleMesh*> m_meshResources;
		EntityId m_camera;
		std::vector<Light*> m_lights;
		std::vector<SceneObject*> m_sceneObjects;
		AccelStructure* m_accel;
	};
}