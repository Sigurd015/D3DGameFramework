#pragma once
#include "Shader.h"

struct ComputePipelineSpecification
{
	Shader* Shader;
};

struct ComputePipeline
{
	ComputePipelineSpecification Spec;
};

void ComputePipeline_Create(ComputePipeline& pipeline, const ComputePipelineSpecification& spec);
void ComputePipeline_Bind(const ComputePipeline& pipeline);
const ComputePipelineSpecification& ComputePipeline_GetSpecification(const ComputePipeline& pipeline);
void ComputePipeline_Release(ComputePipeline& pipeline);