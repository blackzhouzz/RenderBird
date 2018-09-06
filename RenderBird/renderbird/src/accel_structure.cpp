#include "accel_structure.h"
#include "render_statistic.h"

namespace RenderBird
{
	bool AccelStructure::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		bool hasIntersected = false;
		for (auto obj : m_sceneObjects)
		{
			RayHitInfo tempHitInfo;

			RenderStatistic::m_numRayIntersect++;
			if (obj->Intersect(ray, &tempHitInfo))
			{
				if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
				{
					*hitInfo = tempHitInfo;
				}
				hasIntersected = true;
			}
		}
		return hasIntersected;
	}
}