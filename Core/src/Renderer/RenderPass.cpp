#include "pch.h"
#include "RenderPass.h"
#include "Texture.h"
#include "RendererContext.h"
#include "RenderStates.h"
#include "FrameBuffer.h"

void RenderPass_Create(RenderPass& renderPass, const RenderPassSpecification& specification)
{
	renderPass.Specification = specification;
	List_Create(renderPass.Inputs, sizeof(ResourceElement));
	const auto& pipelineSpecification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	renderPass.ShaderReflectionData = Shader_GetReflectionData(pipelineSpecification.Shader);
}

void RenderPass_Release(RenderPass& renderPass)
{
	// Free string in the data, list can't free it, because it doesn't know the type of the data
	List_Foreach(renderPass.Inputs, [](void* data)
		{
			ResourceElement* element = (ResourceElement*)data;
			String_Free(element->Name);
		});

	List_Free(renderPass.Inputs);
	Pipeline_Release(renderPass.Specification.Pipeline);
}

Image2D* RenderPass_GetOutput(const RenderPass& renderPass, uint32_t index)
{
	return Framebuffer_GetImage(RenderPass_GetTargetFramebuffer(renderPass), index);
}

Image2D* RenderPass_GetDepthOutput(const RenderPass& renderPass)
{
	return Framebuffer_GetDepthImage(RenderPass_GetTargetFramebuffer(renderPass));
}

void RenderPass_SetInput(RenderPass& renderPass, const char* name, RendererResourceType type, const void* resource)
{
	ResourceElement element;
	element.Name = String_Duplicate(name);
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
				if (String_Compare(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (String_Compare(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Pixel:
			{
				if (String_Compare(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (String_Compare(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->PSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Compute:
			{
				if (String_Compare(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (String_Compare(decl->Name, "u_SSLinearClamp"))
				{
					RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
				}
				break;
			}
			case ShaderType_Geometry:
			{
				if (String_Compare(decl->Name, "u_SSLinearWrap"))
				{
					RendererContext_GetDeviceContext()->GSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
				}
				else if (String_Compare(decl->Name, "u_SSLinearClamp"))
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

				if (String_Compare(element->Name, decl->Name))
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
					{
						TextureCube_Bind((TextureCube*)element->Resource, decl);
						break;
					}
					case RendererResourceType_Image:
					{
						Image2D_Bind((Image2D*)element->Resource, decl);
						break;
					}
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

void RenderPass_SetTargetFramebuffer(RenderPass& renderPass, Framebuffer* framebuffer)
{
	Pipeline_SetFramebuffer(renderPass.Specification.Pipeline, framebuffer);
}

Framebuffer* RenderPass_GetTargetFramebuffer(const RenderPass& renderPass)
{
	const auto& specification = Pipeline_GetSpecification(renderPass.Specification.Pipeline);
	return specification.TargetFramebuffer;
}

const RenderPassSpecification& RenderPass_GetSpecification(const RenderPass& renderPass)
{
	return renderPass.Specification;
}