#include "mesh_component.h"

IMPLEMENT_TYPE(RenderBird, MeshComponent)

namespace RenderBird
{
	static bool ray_triangle_intersect(
		Vector3f ray_P, Vector3f ray_dir, Float ray_t,
		const Vector3f tri_a, const Vector3f tri_b, const Vector3f tri_c,
		Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide)
	{

		const Vector3f P = ray_P;
		const Vector3f dir = ray_dir;

		/* Calculate vertices relative to ray origin. */
		const Vector3f v0 = tri_a - P;
		const Vector3f v1 = tri_b - P;
		const Vector3f v2 = tri_c - P;

		/* Calculate triangle edges. */
		const Vector3f e0 = v0 - v1;
		const Vector3f e1 = v1 - v2;
		const Vector3f e2 = v2 - v0;

		/* Perform edge tests. */

		const Float U = Vector3f::DotProduct(Vector3f::CrossProduct(v0 + v1, e0), ray_dir);
		const Float V = Vector3f::DotProduct(Vector3f::CrossProduct(v1 + v2, e1), ray_dir);
		const Float W = Vector3f::DotProduct(Vector3f::CrossProduct(v2 + v0, e2), ray_dir);
		const Float minUVW = std::min(U, std::min(V, W));
		const Float maxUVW = std::max(U, std::max(V, W));

		if (minUVW < 0.0f && maxUVW > 0.0f) {
			return false;
		}

		/* Calculate geometry normal and denominator. */
		const Vector3f Ng1 = Vector3f::CrossProduct(e1, e0);
		//const Vec3vfM Ng1 = stable_triangle_normal(e2,e1,e0);
		const Vector3f Ng = Ng1 + Ng1;
		const Float den = Vector3f::DotProduct(Ng, dir);
		/* Avoid division by 0. */
		if (den == 0.0f)
		{
			return false;
		}

		/* Perform depth test. */
		const Float T = Vector3f::DotProduct(v0, Ng);
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
		Float *isect_u, Float *isect_v, Float *isect_t, bool doubleSide)
	{
		Vector3f edge1 = v1 - v0;
		Vector3f edge2 = v2 - v0;

		// Begin calculating determinant; also used to calculate U parameter

		Vector3f pvec = Vector3f::CrossProduct(ray_dir, edge2);

		// If determinant is near zero, ray lies in plane of triangle

		const Float det = Vector3f::DotProduct(edge1, pvec);
		Float u, v, t;
		if (doubleSide)
		{
			// No backface culling in this experiment, determinant within "epsilon" as
			// defined in M&T paper is considered 0

			if (det > -C_FLOAT_EPSILON && det < C_FLOAT_EPSILON)
				return false;

			const Float inv_det = 1.0f / det;

			// Calculate vector from vertex to ray origin

			Vector3f tvec = ray_P - v0;

			// Calculate U parameter and test bounds
			u = Vector3f::DotProduct(tvec, pvec) * inv_det;
			if (u < 0.0f || u > 1.0f)
				return false;

			// Prepare to test V parameter

			Vector3f qvec = Vector3f::CrossProduct(tvec, edge1);

			// Calculate V parameter and test bounds

			v = Vector3f::DotProduct(ray_dir, qvec) * inv_det;
			//sub epsilon solve z fighting
			if (v < 0.0f || u + v >= 1.0f - C_FLOAT_EPSILON)
				return false;

			// Calculate t, final check to see if ray intersects triangle. Test to
			// see if t > tFar added for consistency with other algorithms in experiment.

			t = Vector3f::DotProduct(edge2, qvec) * inv_det;
			if (t <= C_FLOAT_EPSILON || t >= ray_t)
				return false;
		}
		else
		{
			if (det < C_FLOAT_EPSILON)
				return false;
			Vector3f tvec = ray_P - v0;

			u = Vector3f::DotProduct(tvec, pvec);
			if (u < 0.0f || u > det)
				return false;

			Vector3f qvec = Vector3f::CrossProduct(tvec, edge1);

			v = Vector3f::DotProduct(ray_dir, qvec);
			if (v < 0.0f || u + v > det)
				return false;

			t = Vector3f::DotProduct(edge2, qvec);
			const Float inv_det = 1.0f / det;

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
		if (ray_triangle_intersect_ex(ray.m_origin, ray.m_direction, ray.m_maxT, v0, v1, v2, &u, &v, &t, true))
		{
			if (lt(t, hitInfo->m_t))
			{
				hitInfo->m_u = u;
				hitInfo->m_v = v;
				hitInfo->m_t = t;
				if (faceData.m_materialId < trimesh->m_materials.size())
					hitInfo->m_material = trimesh->m_materials[faceData.m_materialId];
				hitInfo->m_ng = (Vector3f::CrossProduct(v2 - v0, v1 - v0)).Normalized();

				hitInfo->m_n = (n0 * (1.0f - u - v) + n1 * u + n2 * v).Normalized();
				hitInfo->m_pos = (v0 * (1.0f - u - v) + v1 * u + v2 * v);

				Vector3f dpdu, dpdv;
				//Vector2f uv[3];
				//if (meshData->uv) {
				//	uv[0] = mesh->uv[v[0]];
				//	uv[1] = mesh->uv[v[1]];
				//	uv[2] = mesh->uv[v[2]];
				//}
				//else {
				//	//uv[0] = Vector2f(0, 0);
				//	//uv[1] = Vector2f(1, 0);
				//	//uv[2] = Vector2f(1, 1);
				//}

				Vector2f duv02 = uv0 - uv2, duv12 = uv1 - uv2;
				Vector3f dp02 = v0 - v2, dp12 = v1 - v2;
				Float determinant = duv02[0] * duv12[1] - duv02[1] * duv12[0];
				bool degenerateUV = std::abs(determinant) < 1e-8;
				if (!degenerateUV) {
					Float invdet = 1 / determinant;
					dpdu = (duv12[1] * dp02 - duv02[1] * dp12) * invdet;
					dpdv = (-duv12[0] * dp02 + duv02[0] * dp12) * invdet;
				}

				if (degenerateUV || Vector3f::CrossProduct(dpdu, dpdv).LengthSquared() == 0) {
					// Handle zero determinant for triangle partial derivative matrix
					Vector3f ng = Vector3f::CrossProduct(v2 - v0, v1 - v0);
					if (ng.LengthSquared() == 0)
						// The triangle is actually degenerate; the intersection is
						// bogus.
						return false;

					CoordinateSystem(ng.Normalized(), &dpdu, &dpdv);
				}

				hitInfo->m_dpdu = dpdu;
				hitInfo->m_dpdv = dpdv;
				hitInfo->m_ns = Vector3f::CrossProduct(dpdu, dpdv).Normalized();

				return true;
			}
		}
		return false;
	}

	bool MeshComponentUtils::Intersect(EntityId id, const Ray& worldRay, RayHitInfo* hitInfo)
	{
		MeshComponent* comp = EntityManager::Instance().GetComponent<MeshComponent>(id);
		Transform* trans = EntityManager::Instance().GetComponent<Transform>(id);
		if (comp == nullptr || trans == nullptr)
			return false;

		TriangleMesh* trimesh = comp->m_trimesh;
		if (trimesh->m_triMeshData == nullptr)
			return false;

		const Matrix4f localToWorld = TransformUtils::GetMatrix(trans);
		const Matrix4f worldToLocal = localToWorld.Inverse();

		Ray ray = Ray::TransformBy(worldRay, worldToLocal);

		auto faceCount = trimesh->GetFaceCount();
		bool hasIntersected = false;
		for (uint32 faceIndex = 0; faceIndex < faceCount; ++faceIndex)
		{
			hasIntersected |= IntersectTriangle(trimesh, trimesh->m_triMeshData, ray, hitInfo, faceIndex);
		}
		if (hasIntersected)
		{
			hitInfo->TransformBy(localToWorld);
			hitInfo->m_id = id;
		}
		return hasIntersected;
	}
}