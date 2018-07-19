#include "geometry_generator.h"

namespace RenderBird
{
	TriangleMesh* GeometryGenerator::GenerateBox(Float size)
	{
		TriangleMesh* triMesh = new TriangleMesh();
		//TriangleMesh::MeshData* meshData = new TriangleMesh::MeshData();
		//meshData->m_position = new Vector3f[24];
		//meshData->m_normal = new Vector3f[24];
		//meshData->m_faceData = new TriangleMesh::FaceData[12];
		//meshData->m_vertexCount = 24;
		//meshData->m_faceCount = 12;

		//const Float halfSize = size * 0.5f;

		////top
		//meshData->m_position[0] = Vector3f(-halfSize, +halfSize, +halfSize);	//a
		//meshData->m_position[1] = Vector3f(+halfSize, +halfSize, +halfSize);	//b	
		//meshData->m_position[2] = Vector3f(+halfSize, -halfSize, +halfSize);	//c
		//meshData->m_position[3] = Vector3f(-halfSize, -halfSize, +halfSize);	//d

		//meshData->m_faceData[0].m_v0 = 0;
		//meshData->m_faceData[0].m_v1 = 3;
		//meshData->m_faceData[0].m_v2 = 1;

		//meshData->m_faceData[1].m_v0 = 1;
		//meshData->m_faceData[1].m_v1 = 3;
		//meshData->m_faceData[1].m_v2 = 2;

		////bottom
		//meshData->m_position[4] = Vector3f(-halfSize, +halfSize, -halfSize);	//e
		//meshData->m_position[5] = Vector3f(+halfSize, +halfSize, -halfSize);	//f	
		//meshData->m_position[6] = Vector3f(+halfSize, -halfSize, -halfSize);	//g
		//meshData->m_position[7] = Vector3f(-halfSize, -halfSize, -halfSize);	//h

		//meshData->m_faceData[0].m_v0 = 4;
		//meshData->m_faceData[0].m_v1 = 7;
		//meshData->m_faceData[0].m_v2 = 5;

		//meshData->m_faceData[1].m_v0 = 5;
		//meshData->m_faceData[1].m_v1 = 7;
		//meshData->m_faceData[1].m_v2 = 6;

		//triMesh->SetMeshData(meshData);
		return triMesh;
	}

	TriangleMesh* GeometryGenerator::GeneratePlane(Vector2f size)
	{
		TriangleMesh* triMesh = new TriangleMesh();
		TriangleMesh::MeshData* meshData = new TriangleMesh::MeshData();
		meshData->m_position = new Vector3f[4];
		meshData->m_normal = new Vector3f[4];
		meshData->m_faceData = new TriangleMesh::FaceData[2];
		meshData->m_vertexCount = 4;
		meshData->m_faceCount = 2;

		const Vector2f halfSize = size * 0.5f;

		//top
		meshData->m_position[0] = Vector3f(-halfSize.x, +halfSize.y, 0);	//a
		meshData->m_position[1] = Vector3f(+halfSize.x, +halfSize.y, 0);	//b	
		meshData->m_position[2] = Vector3f(+halfSize.x, -halfSize.y, 0);	//c
		meshData->m_position[3] = Vector3f(-halfSize.x, -halfSize.y, 0);	//d

		meshData->m_normal[0] = Vector3f(0, 0, 1);	//a
		meshData->m_normal[1] = Vector3f(0, 0, 1);	//b	
		meshData->m_normal[2] = Vector3f(0, 0, 1);	//c
		meshData->m_normal[3] = Vector3f(0, 0, 1);	//d

		meshData->m_faceData[0].m_v0 = 0;
		meshData->m_faceData[0].m_v1 = 3;
		meshData->m_faceData[0].m_v2 = 1;

		meshData->m_faceData[1].m_v0 = 1;
		meshData->m_faceData[1].m_v1 = 3;
		meshData->m_faceData[1].m_v2 = 2;

		triMesh->SetMeshData(meshData);
		return triMesh;
	}
}