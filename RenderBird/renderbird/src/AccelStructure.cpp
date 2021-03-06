#include "AccelStructure.h"
#include "RenderStatistic.h"

namespace RenderBird
{
	bool AccelStructure::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		bool hasIntersected = false;
		for (auto obj : m_sceneObjects)
		{
			RayHitInfo tempHitInfo;

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