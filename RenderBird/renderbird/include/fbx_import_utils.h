#pragma once
#include "renderbird_private.h"
#include "scene.h"
#include <fbxsdk.h>

namespace RenderBird
{
	class FBXImportUtils
	{
	public:
		static void LoadFBX(const std::string& filePath, Scene* scene);
	};


	class FBXImporter : public Singleton<FBXImporter>
	{
	public:
		struct ImportState
		{
		};
		FBXImporter();
		bool Import(const std::string& filePath, Scene* scene);
	private:
		bool ImportScene(ImportState& state, FbxScene* fbxscene, const std::string& filePath, Scene* scene);
		bool ImportNode(ImportState& state, FbxScene* fbxscene, FbxNode* node, Scene* scene);
		TriangleMesh* ImportMesh(ImportState& state, FbxScene* fbxScene, FbxMesh* fbxmesh);
		bool ImportMaterial(ImportState& state, TriangleMesh* trimesh, FbxSurfaceMaterial* pFbxMaterial);
	private:
		FbxManager* m_sdkManager;
		FbxGeometryConverter* mpFbxGeometryConverter;
	};
}