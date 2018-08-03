#include "scene.h"
#include "geometry_generator.h"
#include "mesh_component.h"
#include "camera_component.h"
#include "transform.h"
#include "component_group.h"
#include "light_component.h"
#include "fbx_import_utils.h"
#include "disk.h"

namespace RenderBird
{
	bool Scene::Intersect(const Ray& ray, RayHitInfo* hitInfo)
	{
		{
			auto trans = EntityManager::Instance().GetComponent<Transform>(m_diskId);
			auto disk = EntityManager::Instance().GetComponent<Disk>(m_diskId);
			if (DiskUtils::Intersect(disk, TransformUtils::GetMatrix(trans), ray, hitInfo))
				return true;
		}

		ComponentGroupVisitor<Transform, MeshComponent> visitor(m_meshEntitiesGroup);
		bool hasIntersected = false;
		while (visitor.HasNext())
		{
			auto transform = visitor.Get<Transform>();
			auto meshcomponent = visitor.Get<MeshComponent>();
			hasIntersected |= MeshComponentUtils::Intersect(meshcomponent, ray, hitInfo);

			visitor.MoveNext();
		}
		return hasIntersected;
	}

	void Scene::SetupSceneTest()
	{
		CreateMeshTest();
		CreateLightTest();
		CreateCameraTest();
	}

	void Scene::CreateLightTest()
	{
		auto lightArchetype = EntityManager::IntancePtr()->CreateArchetype(
			3,
			TypeOf<Transform>::Value(),
			TypeOf<LightProperty>::Value(),
			TypeOf<DirectionalLight>::Value()
		);

		auto lightEntity = EntityManager::IntancePtr()->CreateEntity(lightArchetype);

		m_entities.insert(lightEntity);

		m_lightsGroup = new ComponentGroup(lightArchetype);

		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(lightEntity);
		const Float dist = 5;
		Vector3f pos = Vector3f(1, 1, 1) * dist;

		TransformUtils::LookAt(trans, pos, Vector3f::ZERO, Vector3f::UNIT_Z);
	}

	void Scene::CreateMeshTest()
	{
		FBXImportUtils::LoadFBX("c:/1234.fbx", this);

		//auto plane1 = GeometryGenerator::GeneratePlane(Vector2f(2.0f, 2.0f));
		//m_meshResources.push_back(plane1);

		auto meshArchetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<MeshComponent>::Value()
		);

		for (int i = 0; i < m_meshResources.size(); ++i)
		{
			auto planeEntity = EntityManager::IntancePtr()->CreateEntity(meshArchetype);
			auto meshComp = EntityManager::IntancePtr()->GetComponent<MeshComponent>(planeEntity);
			meshComp->m_trimesh = m_meshResources[i];
			m_entities.insert(planeEntity);
		}

		m_meshEntitiesGroup = new ComponentGroup(meshArchetype);
	}

	void Scene::CreateCameraTest()
	{
		auto camArchetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<CameraComponent>::Value()
		);

		m_camera = EntityManager::IntancePtr()->CreateEntity(camArchetype);
		auto comp = EntityManager::IntancePtr()->GetComponent<CameraComponent>(m_camera);

		CameraComponentUtils::PerspectiveFovMatrix(comp, 60, 1.0, 1e-2, 1000);

		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(m_camera);
		const Float dist = 4;
		Vector3f pos = Vector3f(0, -3, 1);
		Vector3f dir = Vector3f(0, 1, 0) * dist;

		TransformUtils::LookAt(trans, pos, pos + dir, Vector3f::UNIT_Z);

		m_entities.insert(m_camera);
	}

	void Scene::AddTestDiskLight(const Vector3f& pos)
	{
		auto camArchetype = EntityManager::IntancePtr()->CreateArchetype<Transform, Disk>();
		m_diskId = EntityManager::IntancePtr()->CreateEntity(camArchetype);
		auto disk = EntityManager::IntancePtr()->GetComponent<Disk>(m_camera);
		disk->m_innerRadius = 0;
		disk->m_radius = 0.2;
		disk->m_phiMax = C_TWO_PI;
		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(m_camera);
		trans->m_position = pos;

		m_entities.insert(m_diskId);
	}

	void Scene::AddTriangleMesh(TriangleMesh* mesh)
	{
		m_meshResources.push_back(mesh);
	}
}