#include "pch.h"
#include "FrameBuffer.h"

void Framebuffer_Create(Framebuffer& frameBuffer, const FramebufferSpecification& spec)
{}

void Framebuffer_Bind(const Framebuffer& frameBuffer)
{}

void Framebuffer_Release(Framebuffer& frameBuffer)
{}

void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer, const Vec4& color)
{}

void Framebuffer_ClearAttachment(const Framebuffer& frameBuffer)
{}

const FramebufferSpecification& Framebuffer_GetSpecification(const Framebuffer& frameBuffer)
{
	// TODO: insert return statement here
	return FramebufferSpecification();
}
