#pragma once

struct FrameBufferSpecification
{
	uint32_t Width = 0;
	uint32_t Height = 0;
	bool UseUniqueClearColor = false;
	bool SwapChainTarget = false;
};

struct Framebuffer
{
	FrameBufferSpecification Specification;
};

void Framebuffer_Create(Framebuffer& frameBuffer, const  FrameBufferSpecification& spec);
void Framebuffer_Bind(const Framebuffer& frameBuffer);
void Framebuffer_Release(Framebuffer& frameBuffer);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer, const Vec4& color);
void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer);
const FrameBufferSpecification& Framebuffer_GetSpecification(const Framebuffer& frameBuffer);