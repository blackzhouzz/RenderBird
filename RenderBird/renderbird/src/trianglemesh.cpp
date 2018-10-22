#include "TriangleMesh.h"
#include <algorithm>
#include "RenderStatistic.h"
IMPLEMENT_TYPE(RenderBird, TriangleMesh)

namespace RenderBird
{
	bool TriangleMesh::IntersectTriangle(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)const
	{
		TriangleMesh::MeshData* meshData = this->m_triMeshData;
		const TriangleMesh::FaceData& faceData = meshData->m_faceData[faceIndex];
		const Vector3f& v0 = meshData->m_vertexData[faceData.m_v0].m_pos;
		const Vector3f& v1 = meshData->m_vertexData[faceData.m_v1].m_pos;
		const Vector3f& v2 = meshData->m_vertexData[faceData.m_v2].m_pos;

		Vector2f uv0 = meshData->m_vertexData[faceData.m_v0].m_uv0;
		Vector2f uv1 = meshData->m_vertexData[faceData.m_v1].m_uv0;
		Vector2f uv2 = meshData->m_vertexData[faceData.m_v2].m_uv0;

		if ((meshData->m_flags & TriangleMesh::VCT_UV0) == false)
		{
			uv0 = Vector2f(0, 0);
			uv1 = Vector2f(1, 0);
			uv2 = Vector2f(1, 1);
		}

		const Vector3f& n0 = meshData->m_vertexData[faceData.m_v0].m_n;
		const Vector3f& n1 = meshData->m_vertexData[faceData.m_v1].m_n;
		const Vector3f& n2 = meshData->m_vertexData[faceData.m_v2].m_n;
		Float u, v, t;
		if (MathUtils::RayTriangleIntersect(ray.m_origin, ray.m_direction, ray.m_maxT, v0, v1, v2, &u, &v, &t, true))
		{
			if (lt(t, hitInfo->m_t))
			{
				hitInfo->m_u = u;
				hitInfo->m_v = v;
				hitInfo->m_t = t;
				if (faceData.m_materialId < this->m_materials.size())
					hitInfo->m_material = this->m_materials[faceData.m_materialId];
				hitInfo->m_ng = (Vector3f::CrossProduct(v2 - v0, v1 - v0)).Normalized();
				hitInfo->m_n = (n0 * (1.0f - u - v) + n1 * u + n2 * v).Normalized();
				hitInfo->m_pos = (v0 * (1.0f - u - v) + v1 * u + v2 * v);

				Vector3f dpdu, dpdv;

				Vector3f q1 = v1 - v0;
				Vector3f q2 = v2 - v0;
				Float s1 = uv1.x - uv0.x, t1 = uv1.y - uv0.y;
				Float s2 = uv2.x - uv0.x, t2 = uv2.y - uv0.y;
				Float invDet = s1 * t2 - s2 * t1;
				if (std::abs(invDet) < 1e-6f)
					return false;
				dpdu = (q1 * t2 - t1 * q2).Normalized();
				dpdv = (q2 * s1 - s2 * q1).Normalized();

				//if (degenerateUV || Vector3f::CrossProduct(dpdv, dpdu).LengthSquared() == 0)
				//{
				//	// Handle zero determinant for triangle partial derivative matrix
				//	Vector3f ng = Vector3f::CrossProduct(v2 - v0, v1 - v0);
				//	if (ng.LengthSquared() == 0)
				//		// The triangle is actually degenerate; the intersection is
				//		// bogus.
				//		return false;

				//	CoordinateSystem(ng.Normalized(), &dpdu, &dpdv);
				//}

				hitInfo->m_dpdu = dpdu.Normalized();
				hitInfo->m_dpdv = dpdv.Normalized();
				hitInfo->m_ns = Vector3f::CrossProduct(hitInfo->m_dpdv, hitInfo->m_dpdu).Normalized();
				hitInfo->m_primId = faceIndex;
				RenderStatistic::m_numRayTriangleIntersect++;

				return true;
			}
		}
		return false;
	}

	bool TriangleMesh::CalcTangentSpace(RayHitInfo* hitInfo, Vector3f& T, Vector3f& B)const
	{
		TriangleMesh::MeshData* meshData = this->m_triMeshData;
		const TriangleMesh::FaceData& faceData = meshData->m_faceData[hitInfo->m_primId];

		Vector2f uv0 = meshData->m_vertexData[faceData.m_v0].m_uv0;
		Vector2f uv1 = meshData->m_vertexData[faceData.m_v1].m_uv0;
		Vector2f uv2 = meshData->m_vertexData[faceData.m_v2].m_uv0;

		const Vector3f& v0 = meshData->m_vertexData[faceData.m_v0].m_pos;
		const Vector3f& v1 = meshData->m_vertexData[faceData.m_v1].m_pos;
		const Vector3f& v2 = meshData->m_vertexData[faceData.m_v2].m_pos;

		if ((meshData->m_flags & TriangleMesh::VCT_UV0) == false)
		{
			uv0 = Vector2f(0, 0);
			uv1 = Vector2f(1, 0);
			uv2 = Vector2f(1, 1);
		}

		Vector2f duv02 = uv0 - uv2, duv12 = uv1 - uv2;
		Vector3f dp02 = v0 - v2, dp12 = v1 - v2;
		Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
		bool degenerateUV = std::abs(determinant) < 1e-8;
		if (!degenerateUV)
		{
			Float invdet = 1 / determinant;
			T = (duv12[1] * dp02 - duv02[1] * dp12) * invdet;
			B = (-duv12[0] * dp02 + duv02[0] * dp12) * invdet;
		}

		if (degenerateUV || Vector3f::CrossProduct(T, B).LengthSquared() == 0)
		{
			Vector3f ng = Vector3f::CrossProduct(v2 - v0, v1 - v0);
			if (ng.LengthSquared() == 0)
				return false;

			CoordinateSystem(ng.Normalized(), &T, &B);
		}
		return true;
	}
}