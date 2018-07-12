#pragma once
#include "renderbird_private.h"
#include "geometry.h"

namespace RenderBird
{
	class TriangleMesh : public Geometry
	{
		//DECLARE_TYPEINFO(TriangleMesh, Geometry)
	public:
		enum FaceFlag
		{
			FF_DoubleSide = 1 << 0,
		};
		struct FaceData
		{
			FaceData()
				: m_flags(FF_DoubleSide)
			{
			}
			int m_v0;
			int m_v1;
			int m_v2;
			int m_materialId;
			uint m_flags;
		};
		struct MeshData
		{
			Vector3f* m_position;
			Vector3f* m_normal;
			Vector3f* m_tangent;
			FaceData* m_faceData;
			Vector2f* m_uv0;
			uint m_faceCount;
			uint m_vertexCount;
		};
		TriangleMesh()
			: m_triMeshData(nullptr)
		{
		}
		void SetMeshData(MeshData* meshData)
		{
			m_triMeshData = meshData;
		}
		virtual GeometryType GetGeometryType()const
		{
			return GT_TriangleMesh;
		}
		Vector3f* GetPosition()
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_position;
			}
			return nullptr;
		}
		Vector3f* GetNormal()
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_normal;
			}
			return nullptr;
		}
		Vector3f* GetTangent()
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_tangent;
			}
			return nullptr;
		}
		FaceData* GetFaceData()
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_faceData;
			}
			return nullptr;
		}
		Vector2f* GetUV0()
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_uv0;
			}
			return nullptr;
		}
		uint GetFaceCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_faceCount;
			}
			return 0;
		}
		uint GetVertexCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_vertexCount;
			}
			return 0;
		}
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo)
		{
			if (m_triMeshData == nullptr)
				return false;
			//Fast, Minimum Storage Ray-Triangle Intersection
			uint faceCount = m_triMeshData->m_faceCount;
			for (uint faceIndex = 0; faceIndex < faceCount; ++faceIndex)
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
				const Float inv_det = 1.0 / det;
				Vector3f t = ray.origin - v0;

				if (faceData.m_flags & FF_DoubleSide)
				{
					u = Dot(t, p) * inv_det;
					if (u < 0.0 || u > 1.0)
						continue;
					Vector3f q = Cross(t, e1);
					v = Dot(ray.direction, q) * inv_det;
					if (v < 0.0 || u + v > 1.0)
						continue;

					time = Dot(e2, q) * inv_det;
					if (hitInfo != nullptr)
					{
						hitInfo->m_t = time;
						hitInfo->m_u = u;
						hitInfo->m_v = v;
					}
					return true;
				}
				else
				{
					if (det < C_FLOAT_EPSILON)
					{
						continue;
					}
					u = Dot(t, p);
					if (u < 0.0 || u > det)
						continue;
					Vector3f q = Cross(t, e1);
					v = Dot(ray.direction, q);
					if (v < 0.0 || u + v > det)
						continue;
					time = Dot(e2, q);
					hitInfo->m_t = time * inv_det;
					hitInfo->m_u = u * inv_det;
					hitInfo->m_v = v * inv_det;
				}

			}
			return false;
		}
	private:
		MeshData* m_triMeshData;
	};
}

DECLEAR_TYPE(RenderBird, TriangleMesh, RenderBird::Geometry);