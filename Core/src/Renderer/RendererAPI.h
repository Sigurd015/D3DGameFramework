#pragma once
#include "Core/Base.h"
#include "Math/Math.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Pipeline.h"
#include "RendererContext.h"
#include "RenderPass.h"
#include "ComputePass.h"
#include "Mesh.h"
#include "Material.h"
#include "EnvMap.h"

void RendererAPI_Initialize();
void RendererAPI_Shutdown();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_Clear();
void RendererAPI_ResetToSwapChain();
void RendererAPI_BeginComputePass(const ComputePass& computePass);
void RendererAPI_EndComputePass(const ComputePass& computePass);
EnvMap RendererAPI_CreateEnvironmentMap(Texture2D* equirectangularMap);
void RendererAPI_BeginRenderPass(const RenderPass& renderPass, bool clear = true);
void RendererAPI_EndRenderPass();
void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Material* material, uint32_t indexCount);
void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, uint32_t indexCount);
void RendererAPI_DrawLines(const VertexBuffer& vertexBuffer, uint32_t vertexCount);
void RendererAPI_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize);
void RendererAPI_DrawMesh(const Mesh* mesh, const Material* material = nullptr);
void RendererAPI_DrawFullScreenQuad();
void RendererAPI_CompositeToSwapChain(const Image2D* finalImage);