#pragma once
#include "renderbird_private.h"
#include "trianglemesh.h"

namespace RenderBird
{
	struct MeshComponent
	{
		TriangleMesh* m_trimesh;
	};
	struct MeshComponentUtils
	{
		static bool Intersect(EntityId id, const Ray& ray, RayHitInfo* hitInfo);
		static bool IntersectTriangle(TriangleMesh* trimesh, TriangleMesh::MeshData* meshData, const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex);
	};
}

DECLEAR_TYPE_COMPONENT(RenderBird, MeshComponent);
DEFAULT_BEGIN(RenderBird, MeshComponent)
DEFAULT_DATA(nullptr)
DEFAULT_END()
