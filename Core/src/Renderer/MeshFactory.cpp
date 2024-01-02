#include "pch.h"
#include "MeshFactory.h"

MeshSource MeshFactory_CreateBox(const Vec3& size)
{
	MeshSource meshSource;

	List_Create(meshSource.Vertices, sizeof(Vertex), 8);
	List_Create(meshSource.Indices, sizeof(Index), 12);

	Vertex vertices[8];
	vertices[0].Position = { -size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	vertices[1].Position = { size.x / 2.0f, -size.y / 2.0f,  size.z / 2.0f };
	vertices[2].Position = { size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
	vertices[3].Position = { -size.x / 2.0f,  size.y / 2.0f,  size.z / 2.0f };
	vertices[4].Position = { -size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	vertices[5].Position = { size.x / 2.0f, -size.y / 2.0f, -size.z / 2.0f };
	vertices[6].Position = { size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };
	vertices[7].Position = { -size.x / 2.0f,  size.y / 2.0f, -size.z / 2.0f };

	vertices[0].TexCoord = { 0.0f, 0.0f };
	vertices[1].TexCoord = { 1.0f, 0.0f };
	vertices[2].TexCoord = { 1.0f, 1.0f };
	vertices[3].TexCoord = { 0.0f, 1.0f };
	vertices[4].TexCoord = { 0.0f, 0.0f };
	vertices[5].TexCoord = { 1.0f, 0.0f };
	vertices[6].TexCoord = { 1.0f, 1.0f };
	vertices[7].TexCoord = { 0.0f, 1.0f };

	vertices[0].Normal = { -1.0f, -1.0f,  1.0f };
	vertices[1].Normal = { 1.0f, -1.0f,  1.0f };
	vertices[2].Normal = { 1.0f,  1.0f,  1.0f };
	vertices[3].Normal = { -1.0f,  1.0f,  1.0f };
	vertices[4].Normal = { -1.0f, -1.0f, -1.0f };
	vertices[5].Normal = { 1.0f, -1.0f, -1.0f };
	vertices[6].Normal = { 1.0f,  1.0f, -1.0f };
	vertices[7].Normal = { -1.0f,  1.0f, -1.0f };

	vertices[0].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[1].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[2].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[3].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[4].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[5].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[6].Binormal = { 0.0f, 0.0f, 0.0f };
	vertices[7].Binormal = { 0.0f, 0.0f, 0.0f };

	vertices[0].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[1].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[2].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[3].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[4].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[5].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[6].Tangent = { 0.0f, 0.0f, 0.0f };
	vertices[7].Tangent = { 0.0f, 0.0f, 0.0f };

	Index indices[12];
	indices[0] = { 0, 1, 2 };
	indices[1] = { 2, 3, 0 };
	indices[2] = { 1, 5, 6 };
	indices[3] = { 6, 2, 1 };
	indices[4] = { 7, 6, 5 };
	indices[5] = { 5, 4, 7 };
	indices[6] = { 4, 0, 3 };
	indices[7] = { 3, 7, 4 };
	indices[8] = { 4, 5, 1 };
	indices[9] = { 1, 0, 4 };
	indices[10] = { 3, 2, 6 };
	indices[11] = { 6, 7, 3 };

	for (size_t i = 0; i < 8; i++)
	{
		List_Add(meshSource.Vertices, &vertices[i]);
	}
	for (size_t i = 0; i < 12; i++)
	{
		List_Add(meshSource.Indices, &indices[i]);
	}

	return meshSource;
}