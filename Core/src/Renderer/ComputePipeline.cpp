#include "pch.h"
#include "ComputePipeline.h"

void ComputePipeline_Create(ComputePipeline& pipeline, const ComputePipelineSpecification& spec)
{
	pipeline.Spec = spec;
}

void ComputePipeline_Bind(const ComputePipeline& pipeline)
{
	Shader_Bind(pipeline.Spec.Shader);
}

const ComputePipelineSpecification& ComputePipeline_GetSpecification(const ComputePipeline& pipeline)
{
	return pipeline.Spec;
}

void ComputePipeline_Release(ComputePipeline& pipeline)
{}