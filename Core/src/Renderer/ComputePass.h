#pragma once
#include "RendererResource.h"
#include "ComputePipeline.h"

#include <stdint.h>

struct ComputePassSpecification
{
	ComputePipeline Pipeline;
};

struct ComputePass
{
	ComputePassSpecification Specification;

	List Inputs;
	List ShaderReflectionData;
};

void ComputePass_Create(ComputePass& computePass, const ComputePassSpecification& specification);
void ComputePass_Release(ComputePass& computePass);
void ComputePass_SetInput(ComputePass& computePass, const char* name, RendererResourceType type, const void* resource);
void ComputePass_BindInputs(const ComputePass& computePass);
void ComputePass_UnbindInputs(const ComputePass& computePass);
const ComputePipeline& ComputePass_GetPipeline(const ComputePass& computePass);