#pragma once
#include "Pipeline.h"

enum RendererResourceType
{
	RendererResourceType_Unknown = 0,
	RendererResourceType_ConstantBuffer,
	RendererResourceType_Texture2D,
	RendererResourceType_TextureCube,
	RendererResourceType_Count
};

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
void RenderPass_SetInput(RenderPass& renderPass, const char* name, RendererResourceType type, const void* resource);
void RnederPass_BindInputs(const RenderPass& renderPass);
const Pipeline& RenderPass_GetPipeline(const RenderPass& renderPass);
const Framebuffer& RenderPass_GetTargetFramebuffer(const RenderPass& renderPass);
const RenderPassSpecification& RenderPass_GetSpecification(const RenderPass& renderPass);