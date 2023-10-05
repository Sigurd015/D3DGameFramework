#include "pch.h"
#include "RenderPass.h"
#include "Texture.h"
#include "RendererContext.h"
#include "RenderStates.h"

#define MAX_RESOURCE_SLOT 16

struct ResourceElement
{
	char* Name;
	RendererResourceType Type;
	const void* Resource;
};

void RenderPass_Create(RenderPass& renderPass, const RenderPassSpecification& specification)
{
	renderPass.Specification = specification;
	List_Create(renderPass.Inputs, MAX_RESOURCE_SLOT);
	{
		ResourceElement element;
		for (size_t i = 0; i < MAX_RESOURCE_SLOT; i++)
		{
			element.Name = nullptr;
			element.Resource = nullptr;
			element.Type = RendererResourceType_Unknown;
			List_Add(renderPass.Inputs, &element, sizeof(ResourceElement));
		}
	}
	renderPass.InputCount = 0;
	const auto& pipelineSpecification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	renderPass.ShaderReflectionData = Shader_GetReflectionData(pipelineSpecification.Shader);
}

void RenderPass_Release(RenderPass& renderPass)
{
	List_Free(renderPass.Inputs, true);
	Pipeline_Release(renderPass.Specification.Pipeline);
}

void RenderPass_SetInput(RenderPass& renderPass, const char* name, RendererResourceType type, const void* resource)
{
	ResourceElement* element = (ResourceElement*)List_Get(renderPass.Inputs, renderPass.InputCount);
	element->Name = strdup(name);
	element->Resource = resource;
	element->Type = type;
	renderPass.InputCount++;
}

void RnederPass_BindInputs(const RenderPass& renderPass)
{
	for (size_t i = 0; i < renderPass.InputCount; i++)
	{
		ResourceElement* element = (ResourceElement*)List_Get(renderPass.Inputs, i);

		HashNode* it = HashMap_Find(renderPass.ShaderReflectionData, element->Name);
		if (it != HashMapEnd)
		{
			switch (element->Type)
			{
			case RendererResourceType_ConstantBuffer:
			{
				uint32_t bindingSlot = *(uint32_t*)it->Value;
				ConstantBuffer_Bind((ConstantBuffer*)element->Resource, bindingSlot);
				break;
			}
			case RendererResourceType_Texture2D:
			{
				uint32_t bindingSlot = *(uint32_t*)it->Value;
				Texture2D_Bind((Texture2D*)element->Resource, bindingSlot);
				break;
			}
			case RendererResourceType_TextureCube:
			{}
			}
		}
	}

	// Bind Common States
	{
		HashNode* it = HashMap_Find(renderPass.ShaderReflectionData, "u_SSLinearWrap");
		if (it != HashMapEnd)
		{
			uint32_t bindingSlot = *(uint32_t*)it->Value;
			RendererContext_GetDeviceContext()->PSSetSamplers(bindingSlot, 1, &s_CommonStates.SSLinearWrap);
		}
	}
	{
		HashNode* it = HashMap_Find(renderPass.ShaderReflectionData, "u_SSLinearClamp");
		if (it != HashMapEnd)
		{
			uint32_t bindingSlot = *(uint32_t*)it->Value;
			RendererContext_GetDeviceContext()->PSSetSamplers(bindingSlot, 1, &s_CommonStates.SSLinearClamp);
		}
	}
}

const Pipeline& RenderPass_GetPipeline(const RenderPass& renderPass)
{
	return renderPass.Specification.Pipeline;
}

const Framebuffer& RenderPass_GetTargetFramebuffer(const RenderPass& renderPass)
{
	const auto& specification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	return specification.TargetFrameBuffer;
}

const RenderPassSpecification& RenderPass_GetSpecification(const RenderPass& renderPass)
{
	return renderPass.Specification;
}