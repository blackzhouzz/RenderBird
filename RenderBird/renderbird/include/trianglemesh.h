#pragma once
#include "renderbird_private.h"
#include "material.h"

namespace RenderBird
{
	class TriangleMesh : public Object
	{
	public:
		struct FaceData
		{
			int m_v0;
			int m_v1;
			int m_v2;
			int m_materialId;
			uint32 m_flags;
		};

		struct VertexData
		{
			Vector3f m_pos;
			Vector3f m_n;
			Vector3f m_tangent;
			RGBA32 m_color;
			Vector2f m_uv0;
		};

		enum VertexChannelFlag
		{
			VCT_Position = 1 << 0,
			VCT_Normal = 1 << 1,
			VCT_TANGENT = 1 << 2,
			VCT_UV0 = 1 << 3,
			VCT_COLOR0 = 1 << 4,
		};
		struct MeshData
		{
			MeshData()
				: m_flags(0)
			{
			}
			bool HasFlag(VertexChannelFlag flag)const
			{
				return (m_flags & flag) != 0;
			}
			std::string m_name;
			std::vector<VertexData> m_vertexData;
			std::vector<FaceData> m_faceData;
			BoundingBox m_localBoundingBox;
			uint32 m_flags;
		};

		TriangleMesh()
			: m_triMeshData(nullptr)
		{
		}
		void SetMeshData(MeshData* meshData)
		{
			m_triMeshData = meshData;
		}
		MeshData* GetMeshData()const
		{
			return m_triMeshData;
		}
		size_t GetFaceCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_faceData.size();
			}
			return 0;
		}
		const FaceData* GetFaceData(uint32 index)const
		{
			if (m_triMeshData != nullptr)
			{
				return &m_triMeshData->m_faceData[index];
			}
			return nullptr;
		}
		size_t GetVertexCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_vertexData.size();
			}
			return 0;
		}

		bool IntersectTriangle(const Ray& ray, RayHitInfo* hitInfo, uint32 faceIndex)const;

		MeshData* m_triMeshData;
		std::vector<Material*> m_materials;
		Matrix4f m_localToWorld;
	};
}

DECLEAR_TYPE(RenderBird, TriangleMesh, Core::Object);
