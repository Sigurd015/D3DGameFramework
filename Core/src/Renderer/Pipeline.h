#pragma once
#include "Shader.h"
#include "VertexBuffer.h"

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct PipelineSpecification
{
	Shader* Shader;
	VertexBufferLayout Layout;
};

struct Pipeline
{
	Shader* Shader;
	VertexBufferLayout* Layout;
	ID3D11InputLayout* InputLayout;
};

void Pipeline_Create(Pipeline* out, Shader* shader, VertexBufferLayout* layout);
void Pipeline_Bind(Pipeline* out);