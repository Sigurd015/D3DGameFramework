#pragma once
#include "Core/base.h"
#include "Math/Math.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

void RendererAPI_Initialize();
void RendererAPI_Shutdown();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_SetClearColor(const Vec4* color);
void RendererAPI_Clear();
void RendererAPI_DrawIndexed(const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, uint32_t indexCount);
void RendererAPI_DrawLines(const VertexBuffer* vertexBuffer, uint32_t vertexCount);