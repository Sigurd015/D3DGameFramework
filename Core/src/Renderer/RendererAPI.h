#pragma once
#include "Core/Base.h"
#include "Math/Math.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RendererContext.h"
#include "RenderPass.h"
#include "Material.h"

void RendererAPI_Initialize();
void RendererAPI_Shutdown();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_SetClearColor(const Vec4& color);
void RendererAPI_Clear();
void RendererAPI_BeginRenderPass(const RenderPass& renderPass, bool clear = true);
void RendererAPI_EndRenderPass();
void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Material& material, uint32_t indexCount);
void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, uint32_t indexCount);
void RendererAPI_DrawLines(const VertexBuffer& vertexBuffer, uint32_t vertexCount);
void RendererAPI_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize);