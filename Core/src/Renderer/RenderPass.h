#pragma once
#include "Pipeline.h"
#include "RendererResource.h"

struct RenderPassSpecification
{
	Pipeline Pipeline;
};

struct RenderPass
{
	RenderPassSpecification Specification;

	HashMap Inputs;
	HashMap ShaderReflectionData;
};

void RenderPass_Create(RenderPass& renderPass, const RenderPassSpecification& specification);
void RenderPass_Release(RenderPass& renderPass);
void RenderPass_SetInput(RenderPass& renderPass, const char* name, const RendererResource* resource);
void RnederPass_BindInputs(const RenderPass& renderPass);
const Pipeline& RenderPass_GetPipeline(const RenderPass& renderPass);
const Framebuffer& RenderPass_GetTargetFramebuffer(const RenderPass& renderPass);
const RenderPassSpecification& RenderPass_GetSpecification(const RenderPass& renderPass);