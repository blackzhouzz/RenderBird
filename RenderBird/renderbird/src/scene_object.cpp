#include "scene_object.h"

namespace RenderBird
{
	SceneObject::SceneObject()
		: m_id(EntityId::INVALID)
		, m_transform(nullptr)
		, m_boundingBox(BoundingBox::INVALID)
	{
	}

	void SceneObject::OnCreate(EntityId id)
	{
		m_transform = EntityManager::Instance().GetComponent<Transform>(id);
	}
}