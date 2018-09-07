#include "mesh_object.h"

namespace RenderBird
{
	void MeshObject::OnCreate(EntityId id)
	{
		SceneObject::OnCreate(id);
		m_mesh = EntityManager::IntancePtr()->GetComponent<MeshComponent>(id);
		UpdateBoundingBox();
	}

	bool MeshObject::Intersect(const Ray& worldRay, RayHitInfo* hitInfo)const
	{
		TriangleMesh* trimesh = m_mesh->m_trimesh;
		if (trimesh->m_triMeshData == nullptr)
			return false;

		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		const Matrix4f worldToLocal = localToWorld.Inverse();

		Ray ray = Ray::TransformBy(worldRay, worldToLocal);

		auto faceCount = trimesh->GetFaceCount();
		bool hasIntersected = false;
		for (uint32 faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			hasIntersected |= trimesh->IntersectTriangle(ray, hitInfo, faceIndex);
		}
		if (hasIntersected)
		{
			hitInfo->TransformBy(localToWorld);
			hitInfo->m_obj = this;
		}
		return hasIntersected;
	}

	Vector2f MeshObject::GetDistanceNearFar(const Vector3f& normal)
	{
		Vector2f ret(C_FLOAT_MAX, -C_FLOAT_MAX);
		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		const auto vertexCount = m_mesh->m_trimesh->GetVertexCount();
		for (auto i = 0; i < vertexCount; ++i)
		{
			auto localPos = m_mesh->m_trimesh->m_triMeshData->m_vertexData[i].m_pos;
			auto worldPos = localToWorld * localPos;
			Float d = Vector3f::DotProduct(normal, worldPos);
			ret[0] = d < ret[0] ? d : ret[0];
			ret[1] = d > ret[1] ? d : ret[1];
		}
		return ret;
	}

	void MeshObject::UpdateBoundingBox()
	{
		m_boundingBox = BoundingBox::INVALID;
		const Matrix4f localToWorld = TransformUtils::GetMatrix(m_transform);
		const auto vertexCount = m_mesh->m_trimesh->GetVertexCount();
		for (auto i = 0; i < vertexCount; ++i)
		{
			auto localPos = m_mesh->m_trimesh->m_triMeshData->m_vertexData[i].m_pos;
			auto worldPos = localToWorld * localPos;
			m_boundingBox += worldPos;
		}
	}
}