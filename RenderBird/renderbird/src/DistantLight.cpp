#include "DistantLight.h"

namespace RenderBird
{
	void DistantLight::OnCreate(EntityId id)
	{
		Light::OnCreate(id);
		m_distantLight = EntityManager::IntancePtr()->GetComponent<DistantLightComponent>(id);
	}

	bool DistantLight::Sample(const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf)
	{
		Vector3f lightDir = MathUtils::GetForward(m_transform->m_rotation);
		ls->m_n = lightDir;
		ls->m_pos = lightDir;
		ls->m_wi = -lightDir;
		ls->m_distance = FLT_MAX;
		ls->m_li = m_lightProp->m_color;
		*pdf = 1.0f;
		return true;
	}
}