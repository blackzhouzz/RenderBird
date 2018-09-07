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
#include "area_light_component.h"
#include "area_light.h"
#include "disk.h"
#include "mesh_object.h"
#include "sphere.h"
#include "bvh.h"
#include "triangle_component.h"
#include "triangle.h"

namespace RenderBird
{
	Scene::Scene()
	{
	}
	bool Scene::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		return m_accel->Intersect(ray, hitInfo);
		//bool hasIntersected = false;
		//for (auto obj : m_sceneObjects)
		//{
		//	RayHitInfo tempHitInfo;
		//	if (obj->Intersect(ray, &tempHitInfo))
		//	{
		//		if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
		//		{
		//			*hitInfo = tempHitInfo;
		//		}
		//		hasIntersected = true;
		//	}
		//}
		//{
		//	ComponentGroupVisitor<Transform, MeshComponent> visitor(m_meshEntitiesGroup);
		//	while (visitor.HasNext())
		//	{
		//		RayHitInfo tempHitInfo;

		//		if (MeshComponentUtils::Intersect(visitor.GetEntityId(), ray, &tempHitInfo))
		//		{
		//			if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
		//			{
		//				*hitInfo = tempHitInfo;
		//			}
		//			hasIntersected = true;
		//		}

		//		visitor.MoveNext();
		//	}
		//}

		//{
		//	ComponentGroupVisitor<Transform, SphereComponent> visitor(m_sphereEntitiesGroup);
		//	while (visitor.HasNext())
		//	{
		//		RayHitInfo tempHitInfo;
		//		if (SphereComponentUtils::Intersect(visitor.GetEntityId(), ray, &tempHitInfo))
		//		{
		//			if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
		//			{
		//				*hitInfo = tempHitInfo;
		//			}
		//			hasIntersected = true;
		//		}
		//		visitor.MoveNext();
		//	}
		//}

		//{
		//	ComponentGroupVisitor<AreaLightComponent, Transform, DiskComponent, LightPropertyComponent> visitor(m_lightsGroup);
		//	while (visitor.HasNext())
		//	{
		//		RayHitInfo tempHitInfo;
		//		if (DiskComponentUtils::Intersect(visitor.GetEntityId(), ray, &tempHitInfo))
		//		{
		//			if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
		//			{
		//				*hitInfo = tempHitInfo;
		//			}
		//			hasIntersected = true;
		//		}

		//		visitor.MoveNext();
		//	}
		//}

		//return hasIntersected;
	}

	void Scene::SetupSceneTest()
	{
		//CreateShapeTest();
		CreateMeshTest();
		//CreateLightTest();
		//AddTestDiskLight(Vector3f(0, 0, 1.98), 0.5);
		//AddTestSphereLight(Vector3f(0, 0, 0.6), 0.2);
		CreateCameraTest();
		//m_accel = new AccelStructure(m_sceneObjects);
		m_accel = new BVH(m_sceneObjects);
	}

	bool Scene::IsLight(EntityId id)
	{
		return EntityManager::IntancePtr()->GetComponent<LightPropertyComponent>(id) != nullptr;
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

		//m_sphereEntitiesGroup = new ComponentGroup(archetype);
	}

	void Scene::CreateLightTest()
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			3,
			TypeOf<Transform>::Value(),
			TypeOf<LightPropertyComponent>::Value(),
			TypeOf<DistantLightComponent>::Value()
		);

		auto id = EntityManager::IntancePtr()->CreateEntity(archetype);

		m_entities.insert(id);

		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(id);
		Vector3f pos = Vector3f(40, 0, 40);


		TransformUtils::LookAt(trans, pos, C_Zero_v3f, C_AxisZ_v3f);
		auto mat = trans->m_rotation.ToMatrix().GetColumn(2);
		auto dir = MathUtils::GetForward(trans->m_rotation);
		dir = dir;
	}

	void Scene::CreateMeshTest()
	{
		FBXImportUtils::LoadFBX("c:/123.fbx", this);

		//auto plane1 = GeometryGenerator::GeneratePlane(Vector2f(2.0f, 2.0f));
		//m_meshResources.push_back(plane1);

		auto archetype = EntityManager::IntancePtr()->CreateArchetype(
			2,
			TypeOf<Transform>::Value(),
			TypeOf<MeshComponent>::Value()
		);

		for (int i = 0; i < m_meshResources.size(); ++i)
		{
			auto id = EntityManager::IntancePtr()->CreateEntity(archetype);
			auto meshComp = EntityManager::IntancePtr()->GetComponent<MeshComponent>(id);
			auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(id);
			//trans->m_pos = Vector3f(0, 0, 1.0f);
			meshComp->m_trimesh = m_meshResources[i];
			if (i == 0)
			{
				//trans->m_pos = Vector3f(0, 0, 0.5f);
			}
			m_entities.insert(id);
			MeshObject* meshObj = CreateSceneObject<MeshObject>(id);
			meshObj = meshObj;
			m_sceneObjects.push_back(meshObj);
		}

		//m_meshEntitiesGroup = new ComponentGroup(archetype);

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

	void Scene::AddTestMeshLight(const Vector3f& pos, TriangleMesh* trimesh)
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype<AreaLightComponent, Transform, TriangleComponent, LightPropertyComponent>();

		for (int i = 0; i < trimesh->GetFaceCount(); ++i)
		{
			EntityId lightId = EntityManager::IntancePtr()->CreateEntity(archetype);

			auto light = EntityManager::IntancePtr()->GetComponent<LightPropertyComponent>(lightId);
			light->m_color = RGB32::WHITE * 10;
			auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(lightId);
			trans->m_pos = pos;

			m_entities.insert(lightId);

			AreaLight* areaLight = CreateSceneObject<AreaLight>(lightId);
			Triangle* shape = new Triangle(trimesh, i);
			areaLight->SetShape(std::unique_ptr<Shape>(shape));
			m_lights.push_back(areaLight);
			m_sceneObjects.push_back(areaLight);
		}
	}

	void Scene::AddTestDiskLight(const Vector3f& pos, Float radius)
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype<AreaLightComponent, Transform, DiskComponent, LightPropertyComponent>();

		EntityId lightId = EntityManager::IntancePtr()->CreateEntity(archetype);
		auto disk = EntityManager::IntancePtr()->GetComponent<DiskComponent>(lightId);
		auto area = EntityManager::IntancePtr()->GetComponent<AreaLightComponent>(lightId);
		disk->m_innerRadius = 0;
		disk->m_radius = radius;
		disk->m_phiMax = C_2_PI;
		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(lightId);
		trans->m_pos = pos;
		//trans->m_rotation = Quaternion::FromEulerAngles(Vector3f(0, DegToRad(180.0), 0));
		auto mat = TransformUtils::GetMatrix(trans);
		auto vec = MathUtils::TransformDirection(mat, C_AxisZ_v3f);

		auto light = EntityManager::IntancePtr()->GetComponent<LightPropertyComponent>(lightId);
		light->m_color = RGB32::WHITE;

		m_entities.insert(lightId);
		//m_lightsGroup = new ComponentGroup(EntityManager::IntancePtr()->CreateArchetype<AreaLightComponent, Transform, DiskComponent, LightPropertyComponent>());
			
		AreaLight* areaLight = CreateSceneObject<AreaLight>(lightId);
		Disk* shape = new Disk(disk);
		areaLight->SetShape(std::unique_ptr<Shape>(shape));
		m_lights.push_back(areaLight);
		m_sceneObjects.push_back(areaLight);
	}

	void Scene::AddTestSphereLight(const Vector3f& pos, Float radius)
	{
		auto archetype = EntityManager::IntancePtr()->CreateArchetype<AreaLightComponent, Transform, SphereComponent, LightPropertyComponent>();

		EntityId lightId = EntityManager::IntancePtr()->CreateEntity(archetype);
		auto sphere = EntityManager::IntancePtr()->GetComponent<SphereComponent>(lightId);
		auto area = EntityManager::IntancePtr()->GetComponent<AreaLightComponent>(lightId);
		sphere->m_radius = radius;
		auto trans = EntityManager::IntancePtr()->GetComponent<Transform>(lightId);
		trans->m_pos = pos;
		auto light = EntityManager::IntancePtr()->GetComponent<LightPropertyComponent>(lightId);

		m_entities.insert(lightId);

		AreaLight* areaLight = CreateSceneObject<AreaLight>(lightId);
		Sphere* shape = new Sphere(sphere);
		areaLight->SetShape(std::unique_ptr<Shape>(shape));
		m_lights.push_back(areaLight);
		m_sceneObjects.push_back(areaLight);
	}

	void Scene::AddTriangleMesh(TriangleMesh* mesh)
	{
		m_meshResources.push_back(mesh);
	}
}