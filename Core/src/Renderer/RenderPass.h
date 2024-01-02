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

	List Inputs;
	List ShaderReflectionData;
};

void RenderPass_Create(RenderPass& renderPass, const RenderPassSpecification& specification);
void RenderPass_Release(RenderPass& renderPass);
Image2D* RenderPass_GetOutput(const RenderPass& renderPass, uint32_t index = 0);
Image2D* RenderPass_GetDepthOutput(const RenderPass& renderPass);
void RenderPass_SetInput(RenderPass& renderPass, const char* name, RendererResourceType type, const void* resource);
void RnederPass_BindInputs(const RenderPass& renderPass);
const Pipeline& RenderPass_GetPipeline(const RenderPass& renderPass);
void RenderPass_SetTargetFramebuffer(RenderPass& renderPass, Framebuffer* framebuffer);
Framebuffer* RenderPass_GetTargetFramebuffer(const RenderPass& renderPass);
const RenderPassSpecification& RenderPass_GetSpecification(const RenderPass& renderPass);