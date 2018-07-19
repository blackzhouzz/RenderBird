#include "trianglemesh.h"
#include "renderbird_register.h"
#include <algorithm>

IMPLEMENT_TYPE(RenderBird, TriangleMesh)

namespace RenderBird
{
	static Float xor_signmask(Float x, int y)
	{
		return Float(int(x) ^ y);
	}

	static bool ray_triangle_intersect(
		Vector3f ray_P, Vector3f ray_dir, Float ray_t,
		const Vector3f tri_a, const Vector3f tri_b, const Vector3f tri_c,
		Float *isect_u, Float *isect_v, Float *isect_t)
	{

		const Vector3f P = ray_P;
		const Vector3f dir = ray_dir;

		/* Calculate vertices relative to ray origin. */
		const Vector3f v0 = tri_c - P;
		const Vector3f v1 = tri_a - P;
		const Vector3f v2 = tri_b - P;

		/* Calculate triangle edges. */
		const Vector3f e0 = v2 - v0;
		const Vector3f e1 = v0 - v1;
		const Vector3f e2 = v1 - v2;

		/* Perform edge tests. */

		const Float U = Dot(Cross(v2 + v0, e0), ray_dir);
		const Float V = Dot(Cross(v0 + v1, e1), ray_dir);
		const Float W = Dot(Cross(v1 + v2, e2), ray_dir);
		const Float minUVW = std::min(U, std::min(V, W));
		const Float maxUVW = std::max(U, std::max(V, W));

		if (minUVW < 0.0f && maxUVW > 0.0f) {
			return false;
		}

		/* Calculate geometry normal and denominator. */
		const Vector3f Ng1 = Cross(e1, e0);
		//const Vec3vfM Ng1 = stable_triangle_normal(e2,e1,e0);
		const Vector3f Ng = Ng1 + Ng1;
		const Float den = Dot(Ng, dir);
		/* Avoid division by 0. */
		if (den == 0.0f)
		{
			return false;
		}

		/* Perform depth test. */
		const Float T = Dot(v0, Ng);
		const int sign_den = ((int)(den) & 0x80000000);
		const Float sign_T = xor_signmask(T, sign_den);
		if ((sign_T < 0.0f) ||
			(sign_T > ray_t * xor_signmask(den, sign_den)))
		{
			return false;
		}

		const Float inv_den = 1.0f / den;

		*isect_u = U * inv_den;
		*isect_v = V * inv_den;
		*isect_t = T * inv_den;
		return true;
	}

	bool TriangleMesh::IntersectTriangleEx(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)
	{
		const FaceData& faceData = m_triMeshData->m_faceData[faceIndex];
		const Vector3f& v0 = m_triMeshData->m_position[faceData.m_v0];
		const Vector3f& v1 = m_triMeshData->m_position[faceData.m_v1];
		const Vector3f& v2 = m_triMeshData->m_position[faceData.m_v2];

		Float u, v, t;
		return ray_triangle_intersect(ray.origin, ray.direction, 100000, v0, v1, v2, &u, &v, &t);
	}


	//Fast, Minimum Storage Ray-Triangle Intersection
	bool TriangleMesh::IntersectTriangle(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)
	{
		Float u, v, time;
		const FaceData& faceData = m_triMeshData->m_faceData[faceIndex];
		const Vector3f& v0 = m_triMeshData->m_position[faceData.m_v0];
		const Vector3f& v1 = m_triMeshData->m_position[faceData.m_v1];
		const Vector3f& v2 = m_triMeshData->m_position[faceData.m_v2];
		Vector3f e1 = v1 - v0;
		Vector3f e2 = v2 - v1;
		Vector3f p = Cross(ray.direction, e2);
		const Float det = Dot(e1, p);
		if (det == 0)
			return false;
		const Float inv_det = 1.0 / det;
		Vector3f t = ray.origin - v0;

		if (faceData.m_flags & FF_DoubleSide)
		{
			u = Dot(t, p) * inv_det;
			if (u < 0.0 || u > 1.0)
				return false;
			Vector3f q = Cross(t, e1);
			v = Dot(ray.direction, q) * inv_det;
			if (v < 0.0 || u + v > 1.0)
				return false;

			time = Dot(e2, q) * inv_det;
			if (hitInfo != nullptr)
			{
				hitInfo->m_t = time;
				hitInfo->m_u = u;
				hitInfo->m_v = v;
			}
		}
		else
		{
			if (det < C_FLOAT_EPSILON)
			{
				return false;
			}
			u = Dot(t, p);
			if (u < 0.0 || u > det)
				return false;
			Vector3f q = Cross(t, e1);
			v = Dot(ray.direction, q);
			if (v < 0.0 || u + v > det)
				return false;
			time = Dot(e2, q);
			hitInfo->m_t = time * inv_det;
			hitInfo->m_u = u * inv_det;
			hitInfo->m_v = v * inv_det;
		}
		return true;
	}

	bool TriangleMesh::Intersect(const Ray& ray, RayHitInfo* hitInfo)
	{
		if (m_triMeshData == nullptr)
			return false;
		uint32 faceCount = m_triMeshData->m_faceCount;
		for (uint32 faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			if (IntersectTriangleEx(ray, hitInfo, faceIndex))
			{
				return true;
			}
		}
		return false;
	}
}