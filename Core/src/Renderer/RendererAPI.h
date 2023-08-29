#pragma once
#include "Core/Base.h"
#include "Math/Math.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RendererContext.h"

void RendererAPI_Initialize();
void RendererAPI_Shutdown();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_SetClearColor(const Vec4& color);
void RendererAPI_Clear();
void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Pipeline& pipeline, uint32_t indexCount);
void RendererAPI_DrawLines(const VertexBuffer& vertexBuffer, const Pipeline& pipeline, uint32_t vertexCount);
void RendererAPI_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize);
void RendererAPI_SetDepthTest(bool enable);
void RendererAPI_SetBlendingState(BlendMode type);