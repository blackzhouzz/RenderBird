#include "arealight.h"
#include "light_component.h"

IMPLEMENT_TYPE(RenderBird, AreaLight)

namespace RenderBird
{
	bool AreaLightUtils::SampleDisk(EntityId areaLightId, const Vector2f& rand2d, SurfaceSample* ss, LightSample* ls)
	{
		Disk* disk = EntityManager::Instance().GetComponent<Disk>(areaLightId);
		Transform* trans = EntityManager::Instance().GetComponent<Transform>(areaLightId);
		LightProperty* lightProp = EntityManager::Instance().GetComponent<LightProperty>(areaLightId);
		if (disk == nullptr || trans == nullptr || lightProp == nullptr)
		{
			return false;
		}
		const Matrix4f objToWorld = TransformUtils::GetMatrix(trans);

		Vector2f pd = SampleUtils::ToUnitDisk(rand2d);
		Vector3f objPos(pd.x * disk->m_radius, pd.y * disk->m_radius, 0);
		ls->m_normal = objToWorld.TransformDirection(Vector3f::UNIT_Z).GetNormalized();
		ls->m_position = objToWorld.TransformPoint(objPos);
		ls->m_pdf = 1.0f / DiskUtils::GetArea(disk);
		Vector3f vecLight = ls->m_position - ss->m_position;
		ls->m_distance = vecLight.Length();
		if (ls->m_distance == 0 || !PathTracingUtils::SampleHemisphere(-vecLight, ls->m_normal))
		{
			ls->m_pdf = 0.0;
		}
		else
		{
			ls->m_wi = vecLight / ls->m_distance;
		}
		ls->m_li = lightProp->m_color;
		return ls->m_pdf > 0.0f;
	}
}

