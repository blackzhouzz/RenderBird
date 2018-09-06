#include "bvh.h"
#include <queue>
#include "render_statistic.h"

namespace RenderBird
{
	const Vector3f BVH::planeSetNormals[BVH::C_Bvh_Num_Plane_Set] = {
		Vector3f(1, 0, 0),
		Vector3f(0, 1, 0),
		Vector3f(0, 0, 1),
		Vector3f(sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
		Vector3f(-sqrtf(3) / 3.f,  sqrtf(3) / 3.f, sqrtf(3) / 3.f),
		Vector3f(-sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f),
		Vector3f(sqrtf(3) / 3.f, -sqrtf(3) / 3.f, sqrtf(3) / 3.f)
	};

	BVH::BVH(const std::vector<SceneObject*>& sceneObjects) : AccelStructure(sceneObjects)
	{
		Extents sceneExtents; // that's the extent of the entire scene which we need to compute for the octree 
		for (auto obj : sceneObjects)
		{
			if (obj->GetBoundingBox() == BoundingBox::INVALID)
				continue;
			Extents ext;
			for (uint8 j = 0; j < C_Bvh_Num_Plane_Set; ++j)
			{
				Vector2f nearFar = obj->GetDistanceNearFar(planeSetNormals[j]);
				if (nearFar[0] < ext.d[j][0]) ext.d[j][0] = nearFar[0];
				if (nearFar[1] > ext.d[j][1]) ext.d[j][1] = nearFar[1];
			}
			ext.m_obj = obj;
			extentsList.push_back(ext);
			sceneExtents.ExtendBy(ext); // expand the scene extent of this object's extent 
		}

		// Now that we have the extent of the scene we can start building our octree
		// Using C++ make_unique function here but you don't need to, just to learn something... 
		octree = new Octree(sceneExtents);

		for (uint32 i = 0; i < extentsList.size(); ++i)
		{
			octree->Insert(&extentsList[i]);
		}

		// Build from bottom up
		octree->Build();
	}

	bool BVH::Extents::Intersect(
		const Float* precomputedNumerator,
		const Float* precomputedDenominator,
		Float& tNear,   // tn and tf in this method need to be contained 
		Float& tFar,    // within the range [tNear:tFar] 
		uint8& planeIndex) const
	{
		for (uint8 i = 0; i < C_Bvh_Num_Plane_Set; ++i)
		{
			Float tNearExtents = (d[i][0] - precomputedNumerator[i]) / precomputedDenominator[i];
			Float tFarExtents = (d[i][1] - precomputedNumerator[i]) / precomputedDenominator[i];
			if (precomputedDenominator[i] < 0) std::swap(tNearExtents, tFarExtents);
			if (tNearExtents > tNear) tNear = tNearExtents, planeIndex = i;
			if (tFarExtents < tFar) tFar = tFarExtents;
			if (tNear > tFar) return false;
		}

		return true;
	}

	bool BVH::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		bool hasIntersected = false;
		Float precomputedNumerator[BVH::C_Bvh_Num_Plane_Set];
		Float precomputedDenominator[BVH::C_Bvh_Num_Plane_Set];
		for (uint8 i = 0; i < C_Bvh_Num_Plane_Set; ++i)
		{
			precomputedNumerator[i] = Vector3f::DotProduct(planeSetNormals[i], ray.m_origin);
			precomputedDenominator[i] = Vector3f::DotProduct(planeSetNormals[i], ray.m_direction);
		}

		uint8 planeIndex;
		Float tNear = 0, tFar = C_FLOAT_MAX; // tNear, tFar for the intersected extents 
		if (!octree->root->nodeExtents.Intersect(precomputedNumerator, precomputedDenominator, tNear, tFar, planeIndex) || tFar < 0)
			return false;
		std::priority_queue<BVH::Octree::QueueElement> queue;
		queue.push(BVH::Octree::QueueElement(octree->root, 0));
		while (!queue.empty())
		{
			if (hitInfo->IsHit() && gt(queue.top().t, hitInfo->m_t))
				break;
			const Octree::OctreeNode *node = queue.top().node;
			queue.pop();
			if (node->isLeaf) 
			{
				for (const auto& e : node->nodeExtentsList) 
				{
					Float t = C_FLOAT_MAX;
					RayHitInfo tempHitInfo;
					RenderStatistic::m_numRayIntersect++;
					if (e->m_obj->Intersect(ray, &tempHitInfo))
					{
						if (!hitInfo->IsHit() || lt(tempHitInfo.m_t, hitInfo->m_t))
						{
							*hitInfo = tempHitInfo;
						}
						hasIntersected = true;
					}
				}
			}
			else 
			{
				for (uint8 i = 0; i < 8; ++i) 
				{
					if (node->child[i] != nullptr) 
					{
						Float tNearChild = 0, tFarChild = tFar;
						RenderStatistic::m_numRayIntersect++;
						if (node->child[i]->nodeExtents.Intersect(precomputedNumerator, precomputedDenominator, tNearChild, tFarChild, planeIndex)) 
						{
							Float t = (tNearChild < 0 && tFarChild >= 0) ? tFarChild : tNearChild;
							queue.push(BVH::Octree::QueueElement(node->child[i], t));
						}
					}
				}
			}
		}

		return hasIntersected;
	}
}