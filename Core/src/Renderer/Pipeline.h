#pragma once
#include "Shader.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "../Utils/List.h"

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

	Shader Shader;
	VertexBufferLayout Layout;
	ID3D11InputLayout* InputLayout;

	List ConstantBuffers;
};

void Pipeline_Create(Pipeline& out, const PipelineSpecification& spec);
void Pipeline_SetConstantBuffer(Pipeline& out, const ConstantBuffer& constantBuffer);
void Pipeline_Bind(const Pipeline& out);
void Pipeline_Release(Pipeline& out);