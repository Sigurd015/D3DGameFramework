#pragma once
#include "Shader.h"
#include "VertexBuffer.h"

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

struct PipelineSpecification
{
	Shader* Shader;
	VertexBufferLayout* Layout;
};

struct Pipeline
{
	PipelineSpecification Spec;

	Shader* Shader;
	VertexBufferLayout* Layout;
	ID3D11InputLayout* InputLayout;
};

void Pipeline_Create(Pipeline* out, const PipelineSpecification* spec);
void Pipeline_Bind(Pipeline* out);
void Pipeline_Release(Pipeline* out);