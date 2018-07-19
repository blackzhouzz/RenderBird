#pragma once
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
		void SetupTestScene();
	private:
		std::vector<TriangleMesh*> m_meshes;
	};
}