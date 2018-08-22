#include "scene.h"
#include "geometry_generator.h"
#include "mesh_component.h"
#include "camera_component.h"
#include "transform.h"
#include "component_group.h"
#include "light_component.h"
#include "fbx_import_utils.h"
#include "disk_component.h"
#include "sphere_component.h"

namespace RenderBird
{
	bool Scene::Intersect(const Ray& ray, RayHitInfo* hitInfo)
	{
		bool hasIntersected = false;

		{
			ComponentGroupVisitor<Transform, MeshComponent> visitor(m_meshEntitiesGroup);
			while (visitor.HasNext())
			{
				hasIntersected |= MeshComponentUtils::Intersect(visitor.GetEntityId(), ray, hitInfo);
				visitor.MoveNext();
			}
		}

		{
			ComponentGroupVisitor<Transform, SphereComponent> visitor(m_sphereEntitiesGroup);
			while (visitor.HasNext())
			{
				hasIntersected |= SphereComponentUtils::Intersect(visitor.GetEntityId(), ray, hitInfo);
				visitor.MoveNext();
			}
		}

		return hasIntersected;
	}

	void Scene::SetupSceneTest()
	{
		//CreateShapeTest();
		CreateMeshTest();
		//CreateLightTest();
		//AddTestDiskLight(Vector3f(0, 0, 1.2));
		AddTestDiskLight(Vector3f(0, 0, 1.8));
		CreateCameraTest();
	}

	void Scene::CreateShapeTest()
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<SphereComponent>::Value()
		);

		auto entity = EntityManager::IntancePtr()->CreateEntity(archetype);
		m_entities.insert(entity);

		auto comp = EntityManager::IntancePtr()->GetComponent<SphereComponent>(entity);
		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(entity);

		m_sphereEntitiesGroup = new ComponentGroup(archetype);
	}

	void Scene::CreateLightTest()
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			3,
			TypeOf<Transform>::Value(),
			TypeOf<LightProperty>::Value(),
			TypeOf<DistantLightComponent>::Value()
		);

		m_lightId = EntityManager::IntancePtr()->CreateEntity(archetype);

		m_entities.insert(m_lightId);

		m_lightsGroup = new ComponentGroup(archetype);

		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(m_lightId);
		Vector3f pos = Vector3f(40, 0, 40);


		TransformUtils::LookAt(trans, pos, C_Zero_v3f, C_AxisZ_v3f);
		auto mat = trans->m_rotation.ToMatrix().GetColumn(2);
		auto dir = MathUtils::GetForward(trans->m_rotation);
		dir = dir;
	}

	void Scene::CreateMeshTest()
	{
		FBXImportUtils::LoadFBX("c:/1234.fbx", this);

		//auto plane1 = GeometryGenerator::GeneratePlane(Vector2f(2.0f, 2.0f));
		//m_meshResources.push_back(plane1);

		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<MeshComponent>::Value()
		);

		for (int i = 0; i < m_meshResources.size(); ++i)
		{
			auto planeEntity = EntityManager::IntancePtr()->CreateEntity(archetype);
			auto meshComp = EntityManager::IntancePtr()->GetComponent<MeshComponent>(planeEntity);
			auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(planeEntity);
			meshComp->m_trimesh = m_meshResources[i];
			if (i == 0)
			{
				//trans->m_position = Vector3f(0, 0, 0.5f);
			}
			m_entities.insert(planeEntity);
		}

		m_meshEntitiesGroup = new ComponentGroup(archetype);
	}

	void Scene::CreateCameraTest()
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<CameraComponent>::Value()
		);

		m_camera = EntityManager::IntancePtr()->CreateEntity(archetype);
		auto comp = EntityManager::IntancePtr()->GetComponent<CameraComponent>(m_camera);

		CameraComponentUtils::PerspectiveFovMatrix(comp, 45.0f, 1.0f, 1e-2f, 1000.0f);

		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(m_camera);
		//Vector3f pos = Vector3f(0, 4, 0);
		//Vector3f at = Vector3f(0, 0, 0);

		Vector3f pos = Vector3f(0, 4, 1);
		Vector3f at = Vector3f(0, 0, 1);

		TransformUtils::LookAt(trans, pos, at, C_AxisZ_v3f);

		m_entities.insert(m_camera);
	}

	void Scene::AddTestDiskLight(const Vector3f& pos)
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype<Transform, DiskComponent, LightProperty>();
		m_lightId = EntityManager::IntancePtr()->CreateEntity(archetype);
		auto disk = EntityManager::IntancePtr()->GetComponent<DiskComponent>(m_lightId);
		disk->m_innerRadius = 0;
		disk->m_radius = 1.0f;
		disk->m_phiMax = C_2_PI;
		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(m_lightId);
		//TransformUtils::LookAt(trans, pos, pos + dir, C_AxisZ_v3f);
		trans->m_position = pos;
		trans->m_rotation = Quaternion::FromEulerAngles(Vector3f(0, DegToRad(180.0), 0));
		auto mat = TransformUtils::GetMatrix(trans);
		auto vec = MathUtils::TransformDirection(mat, C_AxisZ_v3f);

		auto light = EntityManager::IntancePtr()->GetComponent<LightProperty>(m_lightId);

		m_entities.insert(m_lightId);
	}

	void Scene::AddTriangleMesh(TriangleMesh* mesh)
	{
		m_meshResources.push_back(mesh);
	}
}