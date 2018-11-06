#include "Triangle.h"
#include "SampleUtils.h"

namespace RenderBird
{
	Triangle::Triangle(TriangleMesh* trimesh, uint32 faceIndex)
		: m_trimesh(trimesh)
		, m_faceIndex(faceIndex)
	{
		m_face = trimesh->GetFaceData(m_faceIndex);
		m_v0 = &trimesh->GetMeshData()->m_vertexData[m_face->m_v0].m_pos;
		m_v1 = &trimesh->GetMeshData()->m_vertexData[m_face->m_v1].m_pos;
		m_v2 = &trimesh->GetMeshData()->m_vertexData[m_face->m_v2].m_pos;
		m_area = Vector3f::CrossProduct(*m_v2 - *m_v0, *m_v1 - *m_v0).Length() * 0.5f;
	}

	Float Triangle::GetArea()
	{
		return m_area;
	}

	void Triangle::Sample(Sampler* sampler, LightSample* ls)
	{
		auto meshData = m_trimesh->GetMeshData();
		Vector2f uv = SampleUtils::UniformTriangle(sampler->Next2D());
		ls->m_pos = (*m_v0 * (1.0f - uv[0] - uv[1]) + *m_v1 * uv[0] + *m_v2 * uv[1]);
		ls->m_n = Vector3f::CrossProduct(*m_v2 - *m_v0, *m_v1 - *m_v0).Normalized();
		if (meshData->HasFlag(TriangleMesh::VertexChannelFlag::VCT_Normal))
		{
			const Vector3f& n0 = meshData->m_vertexData[m_face->m_v0].m_n;
			const Vector3f& n1 = meshData->m_vertexData[m_face->m_v1].m_n;
			const Vector3f& n2 = meshData->m_vertexData[m_face->m_v2].m_n;
			ls->m_n = (n0 * (1.0f - uv[0] - uv[1]) + n1 * uv[0] + n2 * uv[1]).Normalized();
		}

		if (GetArea() == 0.0f)
		{
			ls->m_pdf = 0.0f;
		}
		else
		{
			ls->m_pdf = 1.0f / GetArea();
		}
	}

	bool Triangle::CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const
	{
		return m_trimesh->CalcTangentSpace(hitInfo, T, B);
	}

	bool Triangle::Intersect(const Ray& ray, RayHitInfo* hitInfo)const
	{
		return m_trimesh->IntersectTriangle(ray, hitInfo, m_faceIndex);
	}

	BoundingBox Triangle::GetBoundingBox(const Matrix4f& localToWorld)const
	{
		BoundingBox ret = BoundingBox::INVALID;
		ret += *m_v0;
		ret += *m_v1;
		ret += *m_v2;
		return ret;
	}

	static void GetNearFar(const Matrix4f& localToWorld, const Vector3f& localPos, const Vector3f& normal, Vector2f& nearFar)
	{
		const auto worldPos = localToWorld * localPos;
		Float d = Vector3f::DotProduct(normal, worldPos);
		nearFar[0] = d < nearFar[0] ? d : nearFar[0];
		nearFar[1] = d > nearFar[1] ? d : nearFar[1];
	}

	Vector2f Triangle::GetDistanceNearFar(const Matrix4f& localToWorld, const Vector3f& normal)
	{
		Vector2f ret(C_FLOAT_MAX, -C_FLOAT_MAX);

		GetNearFar(localToWorld, *m_v0, normal, ret);
		GetNearFar(localToWorld, *m_v1, normal, ret);
		GetNearFar(localToWorld, *m_v2, normal, ret);

		return ret;
	}
}