#pragma once
#include "renderbird_private.h"
#include "geometry.h"

namespace RenderBird
{
	class TriangleMesh : public Geometry
	{
	public:
		enum FaceFlag
		{
			FF_DoubleSide = 1 << 0,
		};
		struct FaceData
		{
			FaceData()
				: m_flags(0)
			{
			}
			int m_v0;
			int m_v1;
			int m_v2;
			int m_materialId;
			uint32 m_flags;
		};
		struct MeshData
		{
			Vector3f* m_position;
			Vector3f* m_normal;
			Vector3f* m_tangent;
			FaceData* m_faceData;
			Vector2f* m_uv0;
			uint32 m_faceCount;
			uint32 m_vertexCount;
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
		uint32 GetFaceCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_faceCount;
			}
			return 0;
		}
		uint32 GetVertexCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_vertexCount;
			}
			return 0;
		}
		bool IntersectTriangle(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex);
		virtual bool Intersect(const Ray& ray, RayHitInfo* hitInfo);
	private:
		MeshData* m_triMeshData;
	};
}

DECLEAR_TYPE(RenderBird, TriangleMesh, RenderBird::Geometry);