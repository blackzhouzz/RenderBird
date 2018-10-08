#include "Light.h"

namespace RenderBird
{
	void Light::OnCreate(EntityId id)
	{
		SceneObject::OnCreate(id);
		m_lightProp = EntityManager::Instance().GetComponent<LightPropertyComponent>(id);
	}

	RGB32 Light::Le(SurfaceSample* ss, const Vector3f& w)const
	{
		return m_lightProp->m_color;
	}
}