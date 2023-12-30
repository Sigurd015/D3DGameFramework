#pragma once
#include "Image.h"

struct FramebufferTextureSpecification
{
	ImageFormat TextureFormat;
	uint32_t Layers = 1;
};

// Used for defining dynamic size attachments, works with FramebufferTextureSpecification
struct FramebufferAttachmentSpecification
{
	FramebufferTextureSpecification* Elements;
	uint32_t ElementCount;
};

struct ExistingImage
{
	const Image2D* Image;
	uint32_t AttachmentIndex;
};

// Used for defining dynamic size existing images, works with ExistingImage
struct ExistingImages
{
	ExistingImage* Elements;
	uint32_t ElementCount = 0;
};

struct FramebufferSpecification
{
	uint32_t Width = 0, Height = 0;
	float DepthClearValue = 1.0f;
	float ClearColor[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
	bool SwapChainTarget = false;

	FramebufferAttachmentSpecification Attachments;

	ExistingImages ExistingImages;
};

struct RenderTargetView
{
	ID3D11RenderTargetView* RTV = nullptr;
};

struct Framebuffer
{
	FramebufferSpecification Specification;

	List ColorAttachments;
	List ColorAttachmentRTV;

	Image2D DsAttachment;
	ID3D11DepthStencilView* DsAttachmentDSV = nullptr;
};

void Framebuffer_Create(Framebuffer& frameBuffer, const  FramebufferSpecification& spec);
void Framebuffer_Bind(const Framebuffer& frameBuffer);
void Framebuffer_Release(Framebuffer& frameBuffer, bool releaseList = true);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer, uint32_t attachmentIndex);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer);
void Framebuffer_ClearDepthAttachment(const Framebuffer& frameBuffer);

const Image2D* Framebuffer_GetImage(const Framebuffer& frameBuffer, uint32_t attachmentIndex = 0);
const Image2D* Framebuffer_GetDepthImage(const Framebuffer& frameBuffer);

const FramebufferSpecification& Framebuffer_GetSpecification(const Framebuffer& frameBuffer);