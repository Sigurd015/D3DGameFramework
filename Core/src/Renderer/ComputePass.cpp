#include "pch.h"
#include "ComputePass.h"
#include "RendererContext.h"
#include "RenderStates.h"
#include "ConstantBuffer.h"
#include "Texture.h"

void ComputePass_Create(ComputePass& computePass, const ComputePassSpecification& specification)
{
	computePass.Specification = specification;
	List_Create(computePass.Inputs, sizeof(ResourceElement));
	const auto& pipelineSpecification = ComputePipeline_GetSpecification(computePass.Specification.Pipeline);
	computePass.ShaderReflectionData = Shader_GetReflectionData(pipelineSpecification.Shader);
}

void ComputePass_Release(ComputePass& computePass)
{
	// Free string in the data, list can't free it, because it doesn't know the type of the data
	List_Foreach(computePass.Inputs, [](void* data)
		{
			ResourceElement* element = (ResourceElement*)data;
			String_Free(element->Name);
		});

	List_Free(computePass.Inputs);
	ComputePipeline_Release(computePass.Specification.Pipeline);
}

void ComputePass_SetInput(ComputePass& computePass, const char* name, RendererResourceType type, const void* resource)
{
	ResourceElement element;
	element.Name = String_Duplicate(name);
	element.Resource = resource;
	element.Type = type;
	List_Add(computePass.Inputs, &element);
}

void ComputePass_BindInputs(const ComputePass& computePass)
{
	for (size_t i = 0; i < List_Size(computePass.ShaderReflectionData); i++)
	{
		ShaderResourceDeclaration* decl = (ShaderResourceDeclaration*)List_Get(computePass.ShaderReflectionData, i);

		if (decl->Stage != ShaderType_Compute)
			continue;

		// Bind Common States
		if (decl->ResourceType == ShaderResourceType_Sampler)
		{
			if (String_Compare(decl->Name, "u_SSLinearWrap"))
			{
				RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearWrap);
			}
			else if (String_Compare(decl->Name, "u_SSLinearClamp"))
			{
				RendererContext_GetDeviceContext()->CSSetSamplers(decl->Slot, 1, &s_CommonStates.SSLinearClamp);
			}
		}
		else
		{
			for (size_t i = 0; i < List_Size(computePass.Inputs); i++)
			{
				ResourceElement* element = (ResourceElement*)List_Get(computePass.Inputs, i);

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
					}
				}
			}
		}
	}
}

void ComputePass_UnbindInputs(const ComputePass& computePass)
{
	static ID3D11UnorderedAccessView* s_NullUAV[] = { nullptr };

	for (size_t i = 0; i < List_Size(computePass.ShaderReflectionData); i++)
	{
		ShaderResourceDeclaration* decl = (ShaderResourceDeclaration*)List_Get(computePass.ShaderReflectionData, i);

		if (decl->Stage != ShaderType_Compute)
			continue;

		if (decl->ResourceType == ShaderResourceType_UnorderedAccess)
		{
			RendererContext_GetDeviceContext()->CSSetUnorderedAccessViews(decl->Slot, 1, s_NullUAV, nullptr);
		}

	}
}

const ComputePipeline& ComputePass_GetPipeline(const ComputePass& computePass)
{
	return computePass.Specification.Pipeline;
}