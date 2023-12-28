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
	List_Create(renderPass.Inputs, sizeof(ResourceElement), MAX_RESOURCE_SLOT);
	const auto& pipelineSpecification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	renderPass.ShaderReflectionData = Shader_GetReflectionData(pipelineSpecification.Shader);
}

void RenderPass_Release(RenderPass& renderPass)
{
	List_Free(renderPass.Inputs);
	Pipeline_Release(renderPass.Specification.Pipeline);
}

void RenderPass_SetInput(RenderPass& renderPass, const char* name, RendererResourceType type, const void* resource)
{
	ResourceElement element;
	element.Name = strdup(name);
	element.Resource = resource;
	element.Type = type;
	List_Add(renderPass.Inputs, &element);
}

void RnederPass_BindInputs(const RenderPass& renderPass)
{
	for (size_t i = 0; i < List_Size(renderPass.ShaderReflectionData); i++)
	{
		ShaderResourceDeclaration* decl = (ShaderResourceDeclaration*)List_Get(renderPass.ShaderReflectionData, i);

		// Bind Common States
		if (decl->ResourceType == ShaderResourceType_Sampler)
		{
			switch (decl->Stage)
			{
			case ShaderType_Vertex:
			{
				if (strcmp(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (strcmp(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Pixel:
			{
				if (strcmp(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (strcmp(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Compute:
			{
				if (strcmp(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (strcmp(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Geometry:
			{
				if (strcmp(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->GSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (strcmp(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->GSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			}
		}
		else
		{
			for (size_t i = 0; i < List_Size(renderPass.Inputs); i++)
			{
				ResourceElement* element = (ResourceElement*)List_Get(renderPass.Inputs, i);

				if (strcmp(element->Name, decl->Name) == 0)
				{
					switch (element->Type)
					{
					case RendererResourceType_ConstantBuffer:
					{
						ConstantBuffer_Bind((ConstantBuffer*)element->Resource, decl);
						break;
					}
					case RendererResourceType_Texture2D:
					{
						Texture2D_Bind((Texture2D*)element->Resource, decl);
						break;
					}
					case RendererResourceType_TextureCube:
					{}
					}
				}
			}
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