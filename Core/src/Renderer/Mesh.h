#pragma once
#include "Utils/List.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Math/Math.h"

struct Vertex
{
	Vec3 Position;
	Vec3 Normal;
	Vec3 Tangent;
	Vec3 Binormal;
	Vec2 TexCoord;
};

struct Index
{
	uint32_t V1, V2, V3;
};

struct MeshSource
{
	List Vertices;
	List Indices;
};

struct Mesh
{
	MeshSource Source;
	VertexBuffer VertexBuffer;
	IndexBuffer IndexBuffer;
};

void Mesh_Create(Mesh* mesh, const MeshSource* source);
const VertexBuffer& Mesh_GetVertexBuffers(const Mesh* mesh);
const IndexBuffer& Mesh_GetIndexBuffer(const Mesh* mesh);
void Mesh_Release(Mesh* mesh);