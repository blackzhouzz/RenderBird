#include "arealight.h"
#include "light_component.h"

IMPLEMENT_TYPE(RenderBird, AreaLight)

namespace RenderBird
{
	bool AreaLightUtils::SampleDisk(EntityId id, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls)
	{
		DiskComponent* comp = EntityManager::Instance().GetComponent<DiskComponent>(id);
		Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
		LightProperty* lightProp = EntityManager::Instance().GetComponent<LightProperty>(id);
		AreaLight* areaLight = EntityManager::Instance().GetComponent<AreaLight>(id);
		if (comp == nullptr || trans == nullptr || lightProp == nullptr || areaLight == nullptr)
		{
			return false;
		}
		const Matrix4f objToWorld = TransformUtils::GetMatrix(trans);

		Vector2f pd = SampleUtils::ToUnitDisk(rand2d);
		Vector3f objPos(pd.x * comp->m_radius, pd.y * comp->m_radius, 0);
		ls->m_normal = MathUtils::TransformDirection(objToWorld, C_AxisZ_v3f).Normalized();
		ls->m_position = objToWorld * (objPos);
		ls->m_pdf = 1.0f / DiskComponentUtils::GetArea(comp);
		Vector3f vecLight = ls->m_position - ss->m_position;
		ls->m_distance = vecLight.Length();
		if (ls->m_distance == 0 || (!areaLight->m_isDoubleSide && !PathTracingUtils::SampleHemisphere(-vecLight, ls->m_normal)))
		{
			ls->m_pdf = 0.0;
		}
		else
		{
			ls->m_wi = vecLight / ls->m_distance;
			auto lensqr = vecLight.LengthSquared();
			auto absdot = std::abs(Vector3f::DotProduct(ls->m_normal, -ls->m_wi));
			ls->m_pdf *= lensqr / absdot;

		}

		ls->m_li = lightProp->m_color;
		return ls->m_pdf > 0.0f;
	}
}

