#include "trianglemesh.h"
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

	//"Fast, Minimum Storage Ray-Triangle Intersection
	static bool ray_triangle_intersect_ex(
		Vector3f ray_P, Vector3f ray_dir, Float ray_t,
		const Vector3f v0, const Vector3f v1, const Vector3f v2,
		Float *isect_u, Float *isect_v, Float *isect_t)
	{
		Vector3f edge1 = v1 - v0;
		Vector3f edge2 = v2 - v0;

		// Begin calculating determinant; also used to calculate U parameter

		Vector3f pvec = Cross(ray_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle

		const Float det = Dot(edge1, pvec);

		// No backface culling in this experiment, determinant within "epsilon" as
		// defined in M&T paper is considered 0

		if (det > -C_FLOAT_EPSILON && det < C_FLOAT_EPSILON)
			return false;

		const Float inv_det = 1.0f / det;

		// Calculate vector from vertex to ray origin

		Vector3f tvec = ray_P - v0;

		// Calculate U parameter and test bounds
		const Float u = Dot(tvec, pvec) * inv_det;
		if (u < 0.0f || u > 1.0f)
			return false;

		// Prepare to test V parameter

		Vector3f qvec = Cross(tvec, edge1);

		// Calculate V parameter and test bounds

		const Float v = Dot(ray_dir, qvec) * inv_det;
		if (v < 0.0f || u + v >= 1.0f)
			return false;

		// Calculate t, final check to see if ray intersects triangle. Test to
		// see if t > tFar added for consistency with other algorithms in experiment.

		const Float t = Dot(edge2, qvec) * inv_det;
		if (t <= C_FLOAT_EPSILON || t >= ray_t)
			return false;

		*isect_u = u;
		*isect_v = v;
		*isect_t = t;

		return true;
	}

	bool TriangleMesh::IntersectTriangle(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)
	{
		const FaceData& faceData = m_triMeshData->m_faceData[faceIndex];
		const Vector3f& v0 = m_triMeshData->m_position[faceData.m_v0];
		const Vector3f& v1 = m_triMeshData->m_position[faceData.m_v1];
		const Vector3f& v2 = m_triMeshData->m_position[faceData.m_v2];

		Float u, v, t;
		if (ray_triangle_intersect_ex(ray.origin, ray.direction, 100000, v0, v1, v2, &u, &v, &t))
		{
			hitInfo->m_u = u;
			hitInfo->m_v = v;
			hitInfo->m_t = t;
			return true;
		}
		return false;
	}


	bool TriangleMesh::Intersect(const Ray& ray, RayHitInfo* hitInfo)
	{
		if (m_triMeshData == nullptr)
			return false;
		uint32 faceCount = m_triMeshData->m_faceCount;
		for (uint32 faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			if (IntersectTriangle(ray, hitInfo, faceIndex))
			{
				return true;
			}
		}
		return false;
	}
}