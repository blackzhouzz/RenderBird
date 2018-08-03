#include "mesh_component.h"

IMPLEMENT_TYPE(RenderBird, MeshComponent)

namespace RenderBird
{
	//static bool ray_triangle_intersect(
	//	Vector3f ray_P, Vector3f ray_dir, Float ray_t,
	//	const Vector3f tri_a, const Vector3f tri_b, const Vector3f tri_c,
	//	Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide)
	//{

	//	const Vector3f P = ray_P;
	//	const Vector3f dir = ray_dir;

	//	/* Calculate vertices relative to ray origin. */
	//	const Vector3f v0 = tri_a - P;
	//	const Vector3f v1 = tri_b - P;
	//	const Vector3f v2 = tri_c - P;

	//	/* Calculate triangle edges. */
	//	const Vector3f e0 = v0 - v1;
	//	const Vector3f e1 = v1 - v2;
	//	const Vector3f e2 = v2 - v0;

	//	/* Perform edge tests. */

	//	const Float U = Dot(Cross(v0 + v1, e0), ray_dir);
	//	const Float V = Dot(Cross(v1 + v2, e1), ray_dir);
	//	const Float W = Dot(Cross(v2 + v0, e2), ray_dir);
	//	const Float minUVW = std::min(U, std::min(V, W));
	//	const Float maxUVW = std::max(U, std::max(V, W));

	//	if (minUVW < 0.0f && maxUVW > 0.0f) {
	//		return false;
	//	}

	//	/* Calculate geometry normal and denominator. */
	//	const Vector3f Ng1 = Cross(e1, e0);
	//	//const Vec3vfM Ng1 = stable_triangle_normal(e2,e1,e0);
	//	const Vector3f Ng = Ng1 + Ng1;
	//	const Float den = Dot(Ng, dir);
	//	/* Avoid division by 0. */
	//	if (den == 0.0f)
	//	{
	//		return false;
	//	}

	//	/* Perform depth test. */
	//	const Float T = Dot(v0, Ng);
	//	const int sign_den = ((int)(den) & 0x80000000);
	//	const Float sign_T = xor_signmask(T, sign_den);
	//	if ((sign_T < 0.0f) ||
	//		(sign_T > ray_t * xor_signmask(den, sign_den)))
	//	{
	//		return false;
	//	}

	//	const Float inv_den = 1.0f / den;

	//	*isect_u = U * inv_den;
	//	*isect_v = V * inv_den;
	//	*isect_t = T * inv_den;
	//	return true;
	//}

	//"Fast, Minimum Storage Ray-Triangle Intersection
	static bool ray_triangle_intersect_ex(
		Vector3f ray_P, Vector3f ray_dir, Float ray_t,
		const Vector3f v0, const Vector3f v1, const Vector3f v2,
		Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide)
	{
		Vector3f edge1 = v1 - v0;
		Vector3f edge2 = v2 - v0;

		// Begin calculating determinant; also used to calculate U parameter

		Vector3f pvec = Cross(ray_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle

		const Float det = Dot(edge1, pvec);
		Float u, v, t;
		if (doubleSide)
		{
			// No backface culling in this experiment, determinant within "epsilon" as
			// defined in M&T paper is considered 0

			if (det > -C_FLOAT_EPSILON && det < C_FLOAT_EPSILON)
				return false;

			const Float inv_det = 1.0 / det;

			// Calculate vector from vertex to ray origin

			Vector3f tvec = ray_P - v0;

			// Calculate U parameter and test bounds
			u = Dot(tvec, pvec) * inv_det;
			if (u < 0.0 || u > 1.0)
				return false;

			// Prepare to test V parameter

			Vector3f qvec = Cross(tvec, edge1);

			// Calculate V parameter and test bounds

			v = Dot(ray_dir, qvec) * inv_det;
			//sub C_FLOAT_EPSILON_HIGH solve z fighting
			if (v < 0.0 || u + v >= 1.0 - C_FLOAT_EPSILON_HIGH)
				return false;

			// Calculate t, final check to see if ray intersects triangle. Test to
			// see if t > tFar added for consistency with other algorithms in experiment.

			t = Dot(edge2, qvec) * inv_det;
			if (t <= C_FLOAT_EPSILON || t >= ray_t)
				return false;
		}
		else
		{
			if (det < C_FLOAT_EPSILON)
				return false;
			Vector3f tvec = ray_P - v0;

			u = Dot(tvec, pvec);
			if (u < 0.0 || u > det)
				return false;

			Vector3f qvec = Cross(tvec, edge1);

			v = Dot(ray_dir, qvec);
			if (v < 0.0 || u + v > det)
				return false;

			t = Dot(edge2, qvec);
			const Float inv_det = 1.0 / det;

			t *= inv_det;
			u *= inv_det;
			v *= inv_det;
		}
		*isect_u = u;
		*isect_v = v;
		*isect_t = t;

		return true;
	}

	bool MeshComponentUtils::IntersectTriangle(TriangleMesh* trimesh, TriangleMesh::MeshData* meshData, const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)
	{
		const TriangleMesh::FaceData& faceData = meshData->m_faceData[faceIndex];
		const Vector3f& v0 = meshData->m_vertexData[faceData.m_v0].m_position;
		const Vector3f& v1 = meshData->m_vertexData[faceData.m_v1].m_position;
		const Vector3f& v2 = meshData->m_vertexData[faceData.m_v2].m_position;

		Float u, v, t;
		if (ray_triangle_intersect_ex(ray.origin, ray.direction, 10000, v0, v1, v2, &u, &v, &t, true))
		{
			if (hitInfo->m_hasHit == false || lt(t, hitInfo->m_t))
			{
				hitInfo->m_u = u;
				hitInfo->m_v = v;
				hitInfo->m_t = t;
				hitInfo->m_material = trimesh->m_materials[faceData.m_materialId];
				hitInfo->m_geomNormal = (Cross(v1 - v0, v2 - v0)).GetNormalized();
				const Vector3f& n0 = meshData->m_vertexData[faceData.m_v0].m_normal;
				const Vector3f& n1 = meshData->m_vertexData[faceData.m_v1].m_normal;
				const Vector3f& n2 = meshData->m_vertexData[faceData.m_v2].m_normal;
				hitInfo->m_normal = (n0 * (1.0 - u - v) + n1 * u + n2 * v).GetNormalized();
				return true;
			}
		}
		return false;
	}

	bool MeshComponentUtils::Intersect(MeshComponent* comp, const Ray& ray, RayHitInfo* hitInfo)
	{
		TriangleMesh* trimesh = comp->m_trimesh;
		if (trimesh->m_triMeshData == nullptr)
			return false;
		uint32 faceCount = trimesh->GetFaceCount();
		bool hasIntersected = false;
		for (uint32 faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			hasIntersected |= IntersectTriangle(trimesh, trimesh->m_triMeshData, ray, hitInfo, faceIndex);
		}
		return hasIntersected;
	}
}