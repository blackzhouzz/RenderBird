#include "DistantLight.h"

namespace RenderBird
{
	void DistantLight::OnCreate(EntityId id)
	{
		Light::OnCreate(id);
		m_distantLight = EntityManager::IntancePtr()->GetComponent<DistantLightComponent>(id);
	}

	bool DistantLight::Sample(Sampler* sampler, SurfaceSample* ss, LightSample* ls)
	{
		Vector3f lightDir = MathUtils::GetForward(m_transform->m_rotation);
		ls->m_n = lightDir;
		ls->m_pos = lightDir;
		ls->m_distance = FLT_MAX;
		ls->m_pdf = 1.0f;
		ss->m_wi = -lightDir;
		return true;
	}
}