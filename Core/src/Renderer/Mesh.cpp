#include "pch.h"
#include "Mesh.h"

void Mesh_Create(Mesh* mesh, const MeshSource* source)
{
	mesh->Source = *source;
	VertexBuffer_Create(mesh->VertexBuffer, List_GetData(source->Vertices), List_Size(source->Vertices) * sizeof(Vertex));
	IndexBuffer_Create(mesh->IndexBuffer, List_GetData(source->Indices), List_Size(source->Indices) * sizeof(Index));

	static VertexBufferLayoutEmelent GeoLayout[5] = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float3, "a_Tangent" },
		{ ShaderDataType::Float3, "a_Bitangent" },
		{ ShaderDataType::Float2, "a_TexCoord" },
	};

	static VertexBufferLayout GeoLayoutInfo = { GeoLayout, 5 };
	VertexBufferLayout_CalculateOffsetsAndStride(GeoLayoutInfo);

	VertexBuffer_SetLayout(mesh->VertexBuffer, GeoLayoutInfo);
}

const VertexBuffer& Mesh_GetVertexBuffers(const Mesh* mesh)
{
	return mesh->VertexBuffer;
}

const IndexBuffer& Mesh_GetIndexBuffer(const Mesh* mesh)
{
	return mesh->IndexBuffer;
}

void Mesh_Release(Mesh* mesh)
{
	List_Free(mesh->Source.Vertices);
	List_Free(mesh->Source.Indices);

	VertexBuffer_Release(mesh->VertexBuffer);
	IndexBuffer_Release(mesh->IndexBuffer);
}