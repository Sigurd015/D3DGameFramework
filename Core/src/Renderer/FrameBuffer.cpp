#include "pch.h"
#include "FrameBuffer.h"
#include "RendererResource.h"

void Invalidate(Framebuffer* frameBuffer)
{
	Framebuffer_Release(frameBuffer, false);

	uint32_t attachmentIndex = 0;
	for (size_t i = 0; i < frameBuffer->Specification.Attachments.ElementCount; i++)
	{
		FramebufferTextureSpecification spec = frameBuffer->Specification.Attachments.Elements[i];

		if (!Image2D_IsDepthFormat(spec.TextureFormat))
		{
			Image2D* image = nullptr;
			bool foundExistingImage = false;
			for (size_t j = 0; j < frameBuffer->Specification.ExistingImages.ElementCount; j++)
			{
				if (frameBuffer->Specification.ExistingImages.Elements[j].AttachmentIndex == i)
				{
					image = frameBuffer->Specification.ExistingImages.Elements[j].Image;
					foundExistingImage = true;
					break;
				}
			}
			if (!foundExistingImage)
			{
				ImageSpecification imageSpec = {};
				imageSpec.Width = frameBuffer->Specification.Width;
				imageSpec.Height = frameBuffer->Specification.Height;
				imageSpec.Format = spec.TextureFormat;
				imageSpec.Layers = spec.Layers;
				imageSpec.Usage = ImageUsage_Attachment;

				ImageCreationOptionalData data = {};
				data.Spec = imageSpec;
				data.ImageData = { nullptr, 0 };

				image = (Image2D*)RendererResourcePool_GetResource(RendererResourceType_Image, &data);
			}

			ID3D11RenderTargetView* targetView = nullptr;
			D3D11_RENDER_TARGET_VIEW_DESC targetViewDesc = {};
			targetViewDesc.Format = Image2D_GetDXGIFormat(image);
			if (spec.Layers > 1)
			{
				targetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
				targetViewDesc.Texture2DArray.ArraySize = spec.Layers;
				targetViewDesc.Texture2DArray.MipSlice = 0;
			}
			else
			{
				targetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				targetViewDesc.Texture2D.MipSlice = 0;
			}

			CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateRenderTargetView(Image2D_GetTexture(image), &targetViewDesc, &targetView));

			List_Add(frameBuffer->ColorAttachments, &image);
			List_Add(frameBuffer->ColorAttachmentRTV, &targetView);
		}
		else
		{
			bool foundExistingImage = false;
			for (size_t j = 0; j < frameBuffer->Specification.ExistingImages.ElementCount; j++)
			{
				if (frameBuffer->Specification.ExistingImages.Elements[j].AttachmentIndex == i)
				{
					frameBuffer->DsAttachment = frameBuffer->Specification.ExistingImages.Elements[j].Image;
					foundExistingImage = true;
					break;
				}
			}
			if (!foundExistingImage)
			{

				ImageSpecification depthImageSpec = {};
				depthImageSpec.Width = frameBuffer->Specification.Width;
				depthImageSpec.Height = frameBuffer->Specification.Height;
				depthImageSpec.Format = spec.TextureFormat;
				depthImageSpec.Layers = spec.Layers;
				depthImageSpec.Usage = ImageUsage_Attachment;

				ImageCreationOptionalData data = {};
				data.Spec = depthImageSpec;
				data.ImageData = { nullptr, 0 };

				frameBuffer->DsAttachment = (Image2D*)RendererResourcePool_GetResource(RendererResourceType_Image, &data);
			}

			D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc = {};
			depthStencilViewDesc.Format = Image2D_ImageFormatToDXDepthDSVFormat(spec.TextureFormat);
			depthStencilViewDesc.Flags = 0;
			if (spec.Layers > 1)
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
				depthStencilViewDesc.Texture2DArray.ArraySize = spec.Layers;
				depthStencilViewDesc.Texture2DArray.MipSlice = 0;
			}
			else
			{
				depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				depthStencilViewDesc.Texture2D.MipSlice = 0;
			}

			CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateDepthStencilView(Image2D_GetTexture(frameBuffer->DsAttachment),
				&depthStencilViewDesc, &frameBuffer->DsAttachmentDSV));
		}
		attachmentIndex++;
	}
}

void Framebuffer_Create(Framebuffer* frameBuffer, const FramebufferSpecification& spec)
{
	frameBuffer->Specification = spec;

	List_Create(frameBuffer->ColorAttachments, sizeof(Image2D*));
	List_Create(frameBuffer->ColorAttachmentRTV, sizeof(ID3D11RenderTargetView*));

	Invalidate(frameBuffer);
}

void Framebuffer_Bind(const Framebuffer* frameBuffer)
{
	RendererContext_GetDeviceContext()->OMSetRenderTargets(List_Size(frameBuffer->ColorAttachmentRTV),
		(ID3D11RenderTargetView**)List_GetData(frameBuffer->ColorAttachmentRTV), frameBuffer->DsAttachmentDSV);
}

void Framebuffer_Release(Framebuffer* frameBuffer, bool releaseList)
{
	// Image Release handled by RendererResourcePool
	if (List_Size(frameBuffer->ColorAttachmentRTV) != 0)
	{
		List_Foreach(frameBuffer->ColorAttachmentRTV, [](void* data)
			{
				ID3D11RenderTargetView** targetView = (ID3D11RenderTargetView**)data;
				if (*targetView)
				{
					(*targetView)->Release();
					*targetView = nullptr;
				}
			});

		List_Clear(frameBuffer->ColorAttachments);
		List_Clear(frameBuffer->ColorAttachmentRTV);
	}

	if (frameBuffer->DsAttachmentDSV)
	{
		frameBuffer->DsAttachmentDSV->Release();
		frameBuffer->DsAttachmentDSV = nullptr;
	}

	if (releaseList)
	{
		List_Free(frameBuffer->ColorAttachments);
		List_Free(frameBuffer->ColorAttachmentRTV);
	}
}

void Framebuffer_ClearAttachment(const Framebuffer* frameBuffer, uint32_t attachmentIndex)
{
	CORE_ASSERT(attachmentIndex < List_Size(frameBuffer->ColorAttachmentRTV), "Framebuffer_ClearAttachment - Invalid attachment index!");

	ID3D11RenderTargetView** targetView = (ID3D11RenderTargetView**)List_Get(frameBuffer->ColorAttachmentRTV, attachmentIndex);
	RendererContext_GetDeviceContext()->ClearRenderTargetView(*targetView, frameBuffer->Specification.ClearColor);
}

void Framebuffer_ClearAttachment(const Framebuffer* frameBuffer)
{
	for (size_t i = 0; i < List_Size(frameBuffer->ColorAttachmentRTV); i++)
	{
		Framebuffer_ClearAttachment(frameBuffer, i);
	}
	Framebuffer_ClearDepthAttachment(frameBuffer);
}

void Framebuffer_ClearDepthAttachment(const Framebuffer* frameBuffer)
{
	RendererContext_GetDeviceContext()->ClearDepthStencilView(frameBuffer->DsAttachmentDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		frameBuffer->Specification.DepthClearValue, 0);
}

Image2D* Framebuffer_GetImage(const Framebuffer* frameBuffer, uint32_t attachmentIndex)
{
	CORE_ASSERT(attachmentIndex < List_Size(frameBuffer->ColorAttachments), "Framebuffer_GetImage - Invalid attachment index!");
	return *(Image2D**)List_Get(frameBuffer->ColorAttachments, attachmentIndex);
}

Image2D* Framebuffer_GetDepthImage(const Framebuffer* frameBuffer)
{
	CORE_ASSERT(frameBuffer->DsAttachment != nullptr, "Framebuffer_GetDepthImage - No depth attachment!");
	return frameBuffer->DsAttachment;
}

const FramebufferSpecification& Framebuffer_GetSpecification(const Framebuffer* frameBuffer)
{
	return frameBuffer->Specification;
}