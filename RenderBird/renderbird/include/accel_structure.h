#pragma once
#include "renderbird_private.h"
#include "scene_object.h"

namespace RenderBird
{
	class AccelStructure
	{
	public:
		AccelStructure(const std::vector<SceneObject*>& sceneObjects)
			: m_sceneObjects(sceneObjects)
		{
		}
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)const;
	protected:
		std::vector<SceneObject*> m_sceneObjects;
	};
}