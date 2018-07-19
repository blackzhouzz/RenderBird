#include "scene.h"
#include "geometry_generator.h"

namespace RenderBird
{
	bool Scene::Intersect(const Ray& ray, RayHitInfo* hitInfo)
	{
		for (auto mesh : m_meshes)
		{
			if (mesh->Intersect(ray, hitInfo))
			{
				return true;
			}
		}
		return false;
	}

	void Scene::SetupTestScene()
	{
		auto plane1 = GeometryGenerator::GeneratePlane(Vector2f(2.0f, 2.0f));
		m_meshes.push_back(plane1);
	}
}