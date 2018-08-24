#include "light_component.h"
#include "sample_utils.h"

IMPLEMENT_TYPE(RenderBird, LightProperty)
IMPLEMENT_TYPE(RenderBird, DistantLightComponent)

namespace RenderBird
{
	bool DistantLightComponentUtils::Sample(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls, Float* pdf)
	{
		DistantLightComponent* comp = EntityManager::Instance().GetComponent<DistantLightComponent>(id);
		Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
		LightProperty* lightProp = EntityManager::Instance().GetComponent<LightProperty>(id);
		if (comp == nullptr || trans == nullptr || lightProp == nullptr)
			return false;
		Vector3f lightDir = MathUtils::GetForward(trans->m_rotation);
		ls->m_normal = lightDir;
		ls->m_position = lightDir;
		ls->m_wi = -lightDir;
		ls->m_distance = FLT_MAX;
		ls->m_li = lightProp->m_color;
		*pdf = 1.0f;
		return true;
	}
}