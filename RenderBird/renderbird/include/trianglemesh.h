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
			Vector3f m_position;
			Vector3f m_normal;
			Vector3f m_tangent;
			RGBA32 m_color;
			Vector2f m_uv0;
		};

		struct MeshData
		{
			std::string m_name;
			std::vector<VertexData> m_vertexData;
			std::vector<FaceData> m_faceData;
			BoundingBox m_localBoundingBox;
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
		size_t GetVertexCount()const
		{
			if (m_triMeshData != nullptr)
			{
				return m_triMeshData->m_vertexData.size();
			}
			return 0;
		}

		MeshData* m_triMeshData;
		std::vector<Material*> m_materials;
	};

	class TriangleMeshUtils
	{
		static void SampleTriangleMesh(const Vector2f& uv, Float* pdf);
	};
}

DECLEAR_TYPE(RenderBird, TriangleMesh, Core::Object);
