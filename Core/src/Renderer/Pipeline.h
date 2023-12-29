#pragma once
#include "Shader.h"
#include "VertexBuffer.h"
#include "ConstantBuffer.h"
#include "../Utils/List.h"
#include "FrameBuffer.h"

#include <stdint.h>
#include <Windows.h>
#include <d3d11.h>

enum PrimitiveTopology
{
	PrimitiveTopology_None = 0,
	PrimitiveTopology_Points,
	PrimitiveTopology_Lines,
	PrimitiveTopology_Triangles,
};

enum DepthCompareOperator
{
	DepthCompareOperator_None = 0,
	DepthCompareOperator_Less,
	DepthCompareOperator_LessEqual,
};

enum BlendMode
{
	BlendMode_Disabled = 0,
	BlendMode_Alpha,
	BlendMode_Additive,
	BlendMode_Subtractive,
};

struct PipelineSpecification
{
	Shader* Shader;
	VertexBufferLayout Layout;
	Framebuffer TargetFramebuffer;
	PrimitiveTopology Topology = PrimitiveTopology::PrimitiveTopology_Triangles;
	DepthCompareOperator DepthOperator = DepthCompareOperator::DepthCompareOperator_Less;
	BlendMode Blend = BlendMode::BlendMode_Disabled;
	bool BackfaceCulling = true;
	bool DepthTest = true;
};

struct Pipeline
{
	PipelineSpecification Spec;

	ID3D11InputLayout* InputLayout;
};

void Pipeline_Create(Pipeline& pipeline, const PipelineSpecification& spec);
void Pipeline_Bind(const Pipeline& pipeline);
const PipelineSpecification& Pipeline_GetSpecification(const Pipeline& pipeline);
void Pipeline_Release(Pipeline& pipeline);