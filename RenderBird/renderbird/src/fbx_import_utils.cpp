#include "fbx_import_utils.h"
#include <fstream>
#include <iostream>
#include "trianglemesh.h"
//#include "ofbx.h"
#include "material.h"
#include <memory>
#include <stdint.h>
#include <stdio.h>
#include <unordered_map>
#include <vector>
#include <nmmintrin.h>

namespace RenderBird
{
	FBXImporter::FBXImporter()
	{
		m_sdkManager = FbxManager::Create();
		FbxIOSettings *ios = FbxIOSettings::Create(m_sdkManager, IOSROOT);
		m_sdkManager->SetIOSettings(ios);
		FbxString lPath = FbxGetApplicationDirectory();
		FbxString lExtension = "dll";
		m_sdkManager->LoadPluginsDirectory(lPath.Buffer(), lExtension.Buffer());
		mpFbxGeometryConverter = new FbxGeometryConverter(m_sdkManager);

		int SDKMajor = 0;
		int SDKMinor = 0;
		int SDKRevision = 0;
		FbxManager::GetFileFormatVersion(SDKMajor, SDKMinor, SDKRevision);
	}

	bool FBXImporter::Import(const std::string& filePath, Scene* scene)
	{
		FbxScene* pFbxScene = FbxScene::Create(m_sdkManager, "");
		FbxImporter* pFbxImporter = FbxImporter::Create(m_sdkManager, "");
		FbxIOSettings* pSetting = m_sdkManager->GetIOSettings();
		bool importStatus = pFbxImporter->Initialize(filePath.c_str(), -1, pSetting);
		if (!importStatus)
			return false;
		if (pFbxImporter->IsFBX())
		{
			pSetting->SetBoolProp(IMP_FBX_MATERIAL, true);
			pSetting->SetBoolProp(IMP_FBX_TEXTURE, true);
			pSetting->SetBoolProp(IMP_FBX_LINK, true);
			pSetting->SetBoolProp(IMP_FBX_SHAPE, true);
			pSetting->SetBoolProp(IMP_FBX_GOBO, true);
			pSetting->SetBoolProp(IMP_FBX_ANIMATION, true);
			pSetting->SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
		}

		importStatus = pFbxImporter->Import(pFbxScene);
		if (!importStatus)
			return false;
		pFbxImporter->Destroy();

		ImportState state;
		bool bRet = ImportScene(state, pFbxScene, filePath, scene);

		pFbxScene->Destroy();
		return bRet;
	}

	bool FBXImporter::ImportScene(ImportState& state, FbxScene* fbxscene, const std::string& filePath, Scene* scene)
	{
		mpFbxGeometryConverter->Triangulate(fbxscene, true);

		return ImportNode(state, fbxscene, fbxscene->GetRootNode(), scene);
	}


	template <typename KeyType, typename ValueType>
	struct UnorderedMapGenerator {
		struct Hash {
			uint32 operator()(const KeyType& a) const {
				uint32 digest = 0;
				for (size_t i = 0; i < sizeof(a); i++)
					digest = _mm_crc32_u8(digest, ((uint8*)&a)[i]);
				return digest;
			}
		};
		struct CompareEq {
			bool operator()(const KeyType& a, const KeyType& b) const {
				return !memcmp(&a, &b, sizeof(a));
			}
		};
		typedef std::unordered_map<KeyType, ValueType, Hash, CompareEq> Type;
	};

	template <typename TGeometryElement, typename TValue>
	TValue GetVertexElement(TGeometryElement* pElement, int iPoint, int iTriangle, int iVertex, TValue defaultValue) {
		if (!pElement || pElement->GetMappingMode() == FbxGeometryElement::eNone)
			return defaultValue;
		int index = 0;

		if (pElement->GetMappingMode() == FbxGeometryElement::eByControlPoint)
			index = iPoint;
		else if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygon)
			index = iTriangle;
		else if (pElement->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
			index = iTriangle * 3 + iVertex;

		if (pElement->GetReferenceMode() != FbxGeometryElement::eDirect)
			index = pElement->GetIndexArray().GetAt(index);

		return pElement->GetDirectArray().GetAt(index);
	}

	static Matrix4f AsTransform(const FbxAMatrix& mat)
	{
		auto lT = mat.GetT();
		auto lS = mat.GetS();
		auto lQ = mat.GetQ();

		return MathUtils::TRS(
			Vector3f(Float(lT[0]), Float(lT[1]), Float(lT[2])),
			Quaternion(Float(lQ[0]), Float(lQ[1]), Float(lQ[2]), Float(lQ[3])),
			C_One_v3f
		);
	}

	Matrix4f GetGeometryTransform(FbxNode* pNode)
	{
		auto lT = pNode->GetGeometricTranslation(FbxNode::eSourcePivot);
		auto lR = pNode->GetGeometricRotation(FbxNode::eSourcePivot);
		auto lS = pNode->GetGeometricScaling(FbxNode::eSourcePivot);

		auto lM = FbxAMatrix(lT, lR, lS);
		auto lQ = lM.GetQ();
		return MathUtils::TRS(
			Vector3f(Float(lT[0]), Float(lT[1]), Float(lT[2])),
			Quaternion(Float(lQ[0]), Float(lQ[1]), Float(lQ[2]), Float(lQ[3])),
			Vector3f(Float(lS[0]), Float(lS[1]), Float(lS[2]))
		);
	}

	static FbxAMatrix ComputeTotalMatrix(FbxNode* Node, FbxScene* scene)
	{
		FbxAMatrix Geometry;
		FbxVector4 Translation, Rotation, Scaling;
		Translation = Node->GetGeometricTranslation(FbxNode::EPivotSet::eSourcePivot);
		Rotation = Node->GetGeometricRotation(FbxNode::EPivotSet::eSourcePivot);
		Scaling = Node->GetGeometricScaling(FbxNode::EPivotSet::eSourcePivot);
		Geometry.SetTRS(Translation, Rotation, Scaling);

		//For Single Matrix situation, obtain transfrom matrix from eDESTINATION_SET, which include pivot offsets and pre/post rotations.
		FbxAMatrix GlobalTransform = scene->GetAnimationEvaluator()->GetNodeGlobalTransform(Node);
		FbxAMatrix TotalMatrix;
		TotalMatrix = GlobalTransform * Geometry;

		return TotalMatrix;
	}

	TriangleMesh* FBXImporter::ImportMesh(ImportState& state, FbxScene* fbxScene, FbxMesh* fbxmesh)
	{
		FbxNode* pNode = fbxmesh->GetNode();
		auto name = pNode->GetName();
		fbxmesh->RemoveBadPolygons();
		if (!fbxmesh->IsTriangleMesh())
		{
			return nullptr;
		}
		auto totalMatrix = ComputeTotalMatrix(pNode, fbxScene);
		auto totalMatrixForNormal = totalMatrix.Inverse();
		totalMatrixForNormal = totalMatrixForNormal.Transpose();

		TriangleMesh* trimesh = new TriangleMesh();
		TriangleMesh::MeshData* meshData = new TriangleMesh::MeshData();
		trimesh->SetMeshData(meshData);

		int materialCount = pNode->GetMaterialCount();
		for (int n = 0; n < materialCount; n++) 
		{
			FbxSurfaceMaterial* material = pNode->GetMaterial(n);
			if (!ImportMaterial(state, trimesh, material)) 
			{
				return nullptr;
			}
		}

		const FbxGeometryElementNormal* pNormals = fbxmesh->GetElementNormal(0);
		if (!pNormals) 
		{
			// Generate normals if we don't have any
			fbxmesh->GenerateNormals();
			pNormals = fbxmesh->GetElementNormal(0);
		}
		const FbxGeometryElementUV* pUVs = fbxmesh->GetElementUV(0);
		const FbxGeometryElementTangent* pTangents = fbxmesh->GetElementTangent(0);
		const FbxGeometryElementVertexColor* pVertexColors = fbxmesh->GetElementVertexColor(0);
		const FbxLayerElementMaterial* pPolygonMaterials = fbxmesh->GetElementMaterial();

		auto getMaterialIndex = [pPolygonMaterials, materialCount](uint32 triangleIndex) 
		{
			int lookupIndex = 0;
			auto mappingMode = pPolygonMaterials->GetMappingMode();
			switch (mappingMode) 
			{
			case FbxGeometryElement::eByPolygon:
				lookupIndex = triangleIndex;
				break;
			case FbxGeometryElement::eAllSame:
				lookupIndex = 0;
				break;
			default:
				break;
			}
			 
			int materialIndex = pPolygonMaterials->mIndexArray->GetAt(lookupIndex);
			assert(materialIndex >= 0 && materialIndex < materialCount);
			return uint32(materialIndex);
		};

		int numTriangles = fbxmesh->GetPolygonCount();
		meshData->m_vertexData.reserve(numTriangles * 3);
		meshData->m_faceData.resize(numTriangles);

		FbxAMatrix LeftToRightMatrix;
		FbxAMatrix LeftToRightMatrixForNormal;
		LeftToRightMatrix.SetS(FbxVector4(1.0, -1.0, 1.0));
		LeftToRightMatrix = LeftToRightMatrix * totalMatrix;
		LeftToRightMatrixForNormal = LeftToRightMatrix.Inverse();
		LeftToRightMatrixForNormal = LeftToRightMatrixForNormal.Transpose();
		LeftToRightMatrixForNormal = LeftToRightMatrixForNormal * totalMatrixForNormal;
		// vertex deduplication
		UnorderedMapGenerator<TriangleMesh::VertexData, uint32>::Type hashMap;
		meshData->m_localBoundingBox = BoundingBox::INVALID;
		for (int t = 0; t < numTriangles; t++)
		{
			uint32 triIndices[3];
			for (uint32 v = 0; v < 3; v++)
			{
				int iPoint = fbxmesh->GetPolygonVertex(t, v);

				//FbxVector4 point = totalMatrix.MultT(fbxmesh->GetControlPointAt(iPoint));
				FbxVector4 point = LeftToRightMatrix.MultT(fbxmesh->GetControlPointAt(iPoint));
				FbxVector4 normal = LeftToRightMatrixForNormal.MultT(GetVertexElement(pNormals, iPoint, t, v, FbxVector4(0, 0, 0, 0)));
				FbxVector2 uv = GetVertexElement(pUVs, iPoint, t, v, FbxVector2(0, 1));
				FbxVector4 tangent = LeftToRightMatrixForNormal.MultT(GetVertexElement(pTangents, iPoint, t, v, FbxVector4(0, 0, 0, 0)));
				FbxColor vertexColor = GetVertexElement(pVertexColors, iPoint, t, v, FbxColor(0, 0, 0, 0));

				TriangleMesh::VertexData vertex = {};
				vertex.m_pos = Vector3f((point[0]), (point[1]), (point[2]));
				vertex.m_n = Vector3f((normal[0]), (normal[1]), (normal[2]));
				vertex.m_tangent = Vector3f((tangent[0]), (tangent[1]), (tangent[2]));
				vertex.m_color = RGBA32((vertexColor[0]), (vertexColor[1]), (vertexColor[2]), (vertexColor[3]));
				vertex.m_uv0 = Vector2f((uv[0]), 1.0f - (uv[1]));

				meshData->m_localBoundingBox += vertex.m_pos;

				auto it = hashMap.find(vertex);
				if (it != hashMap.end()) 
				{
					// it's a duplicate vertex
					triIndices[v] = it->second;
				}
				else 
				{
					// we haven't run into this vertex yet
					uint32 index = uint32(meshData->m_vertexData.size());
					meshData->m_vertexData.emplace_back(vertex);
					hashMap[vertex] = index;
					triIndices[v] = index;
				}
			}

			uint32 materialIndex = 0;
			if (pPolygonMaterials != nullptr)
			{
				materialIndex = getMaterialIndex(t);
			}


			meshData->m_faceData[t].m_v0 = triIndices[0];
			meshData->m_faceData[t].m_v1 = triIndices[1];
			meshData->m_faceData[t].m_v2 = triIndices[2];
			meshData->m_faceData[t].m_materialId = materialIndex;
			meshData->m_name = pNode->GetName();
		}

		meshData->m_flags = TriangleMesh::VCT_Position | TriangleMesh::VCT_Normal | TriangleMesh::VCT_TANGENT | TriangleMesh::VCT_UV0 | TriangleMesh::VCT_COLOR0;

		if (!pUVs || pUVs->GetMappingMode() == FbxGeometryElement::eNone)
			meshData->m_flags &= ~TriangleMesh::VCT_UV0;

		return trimesh;
	}

	bool FBXImporter::ImportNode(ImportState& state, FbxScene* fbxscene, FbxNode* node, Scene* scene)
	{
		FbxNodeAttribute *pAttrib = node->GetNodeAttribute();
		if (pAttrib)
		{
			FbxNodeAttribute::EType type = pAttrib->GetAttributeType();
			if (type == FbxNodeAttribute::EType::eMesh)
			{
				if (std::string(node->GetName()) == "light")
				{
					TriangleMesh* trimesh = ImportMesh(state, fbxscene, node->GetMesh());
					Vector3f center = trimesh->GetMeshData()->m_localBoundingBox.GetCenter();
					auto bounding = trimesh->GetMeshData()->m_localBoundingBox;
					scene->AddTestMeshLight(C_Zero_v3f, trimesh);
				}
				else
				{
					TriangleMesh* trimesh = ImportMesh(state, fbxscene, node->GetMesh());
					scene->AddTriangleMesh(trimesh);
				}
			}
			else if (type == FbxNodeAttribute::EType::eLight)
			{
				type = type;

			}
		}

		for (int i = 0, e = node->GetChildCount(); i < e; i++) {
			if (!ImportNode(state, fbxscene, node->GetChild(i), scene)) {
				return false;
			}
		}
		return true;
	}


	//uint32 importTexture(ImportState& state, FbxFileTexture* pFileTexture, bool sRGB) {
	//	if (!pFileTexture)
	//		return hvvr::SimpleMaterial::badTextureIndex;

	//	const char* path = pFileTexture->GetFileName();
	//	auto it = state.textureLookup.find(path);
	//	if (it != state.textureLookup.end()) {
	//		// texture has already been loaded
	//		return it->second;
	//	}

	//	int channelsOut = 4;
	//	int width, height, channelsIn;
	//	uint8* data = stbi_load(path, &width, &height, &channelsIn, channelsOut);
	//	if (data == nullptr) {
	//		printf("error: stb_image couldn't load %s\n", path);
	//		return hvvr::SimpleMaterial::badTextureIndex;
	//	}

	//	size_t size = width * height * channelsOut;

	//	hvvr::TextureData tex;
	//	tex.data = new uint8[size];
	//	memcpy((void*)tex.data, data, size);
	//	STBI_FREE(data);
	//	tex.format = sRGB ? hvvr::TextureFormat::r8g8b8a8_unorm_srgb : hvvr::TextureFormat::r8g8b8a8_unorm;
	//	tex.width = width;
	//	tex.height = height;
	//	tex.stride = width;

	//	uint32 texIndex = uint32(state.model.textures.size());
	//	state.model.textures.emplace_back(std::move(tex));
	//	state.textureLookup[path] = texIndex;

	//	return texIndex;
	//}

	RGBA32 fbxColorConvert(const FbxDouble3& inColor, const FbxDouble& inFactor) {
		//hvvr::vector3 linearColor = hvvr::sRgbToLinear(hvvr::vector3(Float(inColor[0]), Float(inColor[1]), Float(inColor[2])));
		return RGBA32(inColor[0] * Float(inFactor), inColor[1] * Float(inFactor), inColor[2] * Float(inFactor), Float(inFactor));
	}


	// Import a material
	bool FBXImporter::ImportMaterial(ImportState& state, TriangleMesh* trimesh, FbxSurfaceMaterial* pFbxMaterial)
	{
		if (pFbxMaterial == nullptr) {
			printf("error: Material was null!\n");
			return false;
		}

		FbxSurfacePhong* pPhong = (FbxSurfacePhong*)pFbxMaterial;

		FbxDouble3 Diffuse = pPhong->Diffuse.Get();
		FbxDouble DiffuseFactor = pPhong->DiffuseFactor.Get();
		FbxDouble3 Specular = pPhong->Specular.Get();
		FbxDouble SpecularFactor = pPhong->SpecularFactor.Get();
		FbxDouble3 Emissive = pPhong->Emissive.Get();
		FbxDouble EmissiveFactor = pPhong->EmissiveFactor.Get();
		FbxDouble3 TransColor = pPhong->TransparentColor.Get();
		FbxDouble TransFactor = pPhong->TransparencyFactor.Get();
		FbxDouble Shininess = pPhong->Shininess.Get();

		Float transparency = Float((TransColor[0] + TransColor[1] + TransColor[2]) / 3.0 * TransFactor);
		//// Undo FBX glossiness to specular power mapping (n = 2^(g*10)) to get a 0..1 gloss value
		Float glossiness = Float(log2(fmax(Shininess, 1.0f)) / 10.0);

		//uint32 diffuseID = importTexture(state, GetTextureFromProperty(&pPhong->Diffuse), true);
		//uint32 emissiveID = importTexture(state, GetTextureFromProperty(&pPhong->Emissive), true);
		//uint32 specularID = importTexture(state, GetTextureFromProperty(&pPhong->Specular), true);
		//uint32 glossinessID = importTexture(state, GetTextureFromProperty(&pPhong->Shininess), true);
		Material* material = new Material();
		material->m_emissiveColor = fbxColorConvert(Emissive, EmissiveFactor);
		material->m_diffuseColor = fbxColorConvert(Diffuse, DiffuseFactor);
		material->m_specularColor = fbxColorConvert(Specular, SpecularFactor);
		material->m_glossiness = glossiness;
		material->m_opacity = 1.0f - transparency;
		trimesh->m_materials.push_back(material);
		return true;
	}

	//static void ConvertScene(ofbx::IScene* scene, Scene* destScene)
	//{
		//auto meshCount = scene->getMeshCount();
		//for (int i = 0; i < meshCount; ++i)
		//{
		//	TriangleMesh* trimesh = new TriangleMesh();
		//	TriangleMesh::MeshData* meshData = new TriangleMesh::MeshData();
		//	trimesh->SetMeshData(meshData);

		//	const ofbx::Mesh* mesh = scene->getMesh(i);
		//	const ofbx::Geometry* geometry = mesh->getGeometry();
		//	auto geomVertices = geometry->getVertices();
		//	auto geomNormals = geometry->getNormals();
		//	auto geomUv0 = geometry->getUVs(0);
		//	auto geomColor = geometry->getColors();
		//	int vertexCount = geometry->getVertexCount();
		//	int faceCount = vertexCount / 3;
		//	meshData->m_vertexCount = vertexCount;
		//	meshData->m_faceCount = faceCount;

		//	meshData->m_pos = new Vector3f[vertexCount];
		//	meshData->m_n = new Vector3f[vertexCount];
		//	meshData->m_uv0 = new Vector2f[vertexCount];
		//	meshData->m_faceData = new TriangleMesh::FaceData[faceCount];

		//	for (int i = 0; i < vertexCount; ++i)
		//	{
		//		meshData->m_pos[i] = Vector3f(geomVertices[i].x, geomVertices[i].y, geomVertices[i].z);
		//		meshData->m_n[i] = Vector3f(geomNormals[i].x, geomNormals[i].y, geomNormals[i].z);
		//		if (geomUv0 != nullptr)
		//			meshData->m_uv0[i] = Vector2f(geomUv0[i].x, geomUv0[i].y);
		//	}

		//	for (int i = 0; i < faceCount; ++i)
		//	{
		//		meshData->m_faceData[i].m_v0 = i * 3 + 0;
		//		meshData->m_faceData[i].m_v1 = i * 3 + 1;
		//		meshData->m_faceData[i].m_v2 = i * 3 + 2;
		//	}
		//	destScene->AddTriangleMesh(trimesh);
		//}
	//}

	void FBXImportUtils::LoadFBX(const std::string& filePath, Scene* scene)
	{
		FBXImporter::Instance().Import(filePath, scene);

		//int8* content;
		//std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);

		//if (file.is_open())
		//{
		//	auto size = (int)file.tellg();
		//	content = new int8[size];
		//	file.seekg(0, std::ios::beg);
		//	file.read(content, size);
		//	file.close();

		//	auto fbxscene = ofbx::load((ofbx::u8*)content, size);
		//	ConvertScene(fbxscene, scene);
		//	delete[] content;
		//}
	}
}