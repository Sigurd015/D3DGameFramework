#include "pch.h"
#include "MeshLoader.h"

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

static const uint32_t s_MeshImportFlags =
aiProcess_GenNormals |              // Make sure we have legit normals
aiProcess_GenUVCoords |             // Convert UVs if required 
aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
aiProcess_SortByPType |             // Split meshes by primitive type
aiProcess_JoinIdenticalVertices |   // Join identical vertices/ optimize indexing
aiProcess_Triangulate;            // Make sure we're triangles
//aiProcess_OptimizeGraph |
//aiProcess_OptimizeMeshes |          // Batch draws where possible
//aiProcess_LimitBoneWeights |        // If more than N (=4) bone weights, discard least influencing bones and renormalise sum to 1
//aiProcess_ValidateDataStructure   // Validation
//aiProcess_GlobalScale              // e.g. convert cm to m for fbx import (and other formats where cm is native)

MeshSource MeshLoader_Load(const char* path)
{
	MeshSource meshSource;

	List_Create(meshSource.Vertices, sizeof(Vertex));
	List_Create(meshSource.Indices, sizeof(Index));

	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);

	const aiScene* scene = importer.ReadFile(path, s_MeshImportFlags);

	if (!scene)
	{
		CORE_LOG_ERROR("Failed to load mesh source: %s", importer.GetErrorString());
		return meshSource;
	}

	// TODO: Support multiple meshes
	if (scene->HasMeshes())
	{
		aiMesh* mesh = scene->mMeshes[0];
		// Vertices
		for (uint32_t i = 0; i < mesh->mNumVertices; i++)
		{
			CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
			CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

			Vertex vertex;
			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

			if (mesh->HasTangentsAndBitangents())
			{
				vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
				vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
			}

			if (mesh->HasTextureCoords(0))
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

			List_Add(meshSource.Vertices, &vertex);
		}

		// Indices
		for (size_t i = 0; i < mesh->mNumFaces; i++)
		{
			CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
			Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
			List_Add(meshSource.Indices, &index);
		}
	}

	// TODO: May Support Materials, But don't need it now
	return meshSource;
}