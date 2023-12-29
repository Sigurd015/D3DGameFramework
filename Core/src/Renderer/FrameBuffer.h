#pragma once

struct FramebufferSpecification
{
	uint32_t Width = 0;
	uint32_t Height = 0;
	bool UseUniqueClearColor = false;
	bool SwapChainTarget = false;
};

struct Framebuffer
{
	FramebufferSpecification Specification;
};

void Framebuffer_Create(Framebuffer& frameBuffer, const  FramebufferSpecification& spec);
void Framebuffer_Bind(const Framebuffer& frameBuffer);
void Framebuffer_Release(Framebuffer& frameBuffer);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer, const Vec4& color);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer);
const FramebufferSpecification& Framebuffer_GetSpecification(const Framebuffer& frameBuffer);