#include "pch.h"
#include "RenderPass.h"
#include "Texture.h"
#include "RendererContext.h"
#include "RenderStates.h"

void RenderPass_Create(RenderPass& renderPass, const RenderPassSpecification& specification)
{
	renderPass.Specification = specification;
	HashMap_Create(renderPass.Inputs);
	const auto& pipelineSpecification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	renderPass.ShaderReflectionData = Shader_GetReflectionData(pipelineSpecification.Shader);
}

void RenderPass_Release(RenderPass& renderPass)
{
	HashMap_Free(renderPass.Inputs, false);
}

void RenderPass_SetInput(RenderPass& renderPass, const char* name, const RendererResource* resource)
{
	HashMap_Set(renderPass.Inputs, name, (void*)resource);
}

void RnederPass_BindInputs(const RenderPass& renderPass)
{
	for (size_t i = 0; i < HashMap_GetTableSize(); i++)
	{
		HashNode* currentNode = HashMap_Get(renderPass.Inputs, i);
		if (currentNode == nullptr)
		{
			continue;
		}

		while (currentNode != nullptr)
		{
			HashNode* it = HashMap_Find(renderPass.ShaderReflectionData, currentNode->Key);
			if (it != HashMapEnd)
			{
				const RendererResource* resource = (const RendererResource*)currentNode->Value;
				switch (resource->Type)
				{
				case RendererResourceType_ConstantBuffer:
				{
					uint32_t bindingSlot = *(uint32_t*)it->Value;
					ConstantBuffer_Bind((ConstantBuffer*)resource->Data, bindingSlot);
					break;
				}
				case RendererResourceType_Texture2D:
				{
					uint32_t bindingSlot = *(uint32_t*)it->Value;
					Texture2D_Bind((Texture2D*)resource->Data, bindingSlot);
					break;
				}
				case RendererResourceType_TextureCube:
				{}
				}
			}
			currentNode = currentNode->Next;
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