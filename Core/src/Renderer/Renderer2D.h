#pragma once
#include "../Math/Math.h"
#include "Texture.h"

void Renderer2D_Init();
void Renderer2D_Shutdown();
void Renderer2D_BeginScene(const Mat& viewProjection);
void Renderer2D_EndScene();
void Renderer2D_DrawQuad(const Mat& transform, const Vec4& color);
void Renderer2D_DrawQuad(const Mat& transform, Texture2D* texture, const Vec2& uv0, const Vec2& uv1,
	const Vec4& tintColor, float tilingFactor = 1.0f);
void Renderer2D_DrawCircle(const Mat& transform, const Vec4& color, float thickness = 1.0f, float fade = 0.005f);
void Renderer2D_DrawLine(const Vec3& p0, Vec3& p1, const Vec4& color);
void Renderer2D_DrawRect(const Vec3& position, const Vec2& size, const Vec4& color);
void Renderer2D_DrawRect(const Mat& transform, const Vec4& color);
void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, float rotation, const Vec4& color);
void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, float rotation, Texture2D* texture, const Vec2& uv0, const Vec2& uv1,
	const Vec4& tintColor, float tilingFactor = 1.0f);
// Notice: Make sure the str is long lived
void Renderer2D_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize);

struct Renderer2DStatistics
{
	uint32_t DrawCalls = 0;
	uint32_t QuadCount = 0;
	uint32_t LineCount = 0;

	uint32_t GetTotalVertexCount() const { return QuadCount * 4 + LineCount * 2; }
	uint32_t GetTotalIndexCount() const { return QuadCount * 6 + LineCount * 2; }
};

void Renderer2D_ResetStats();
Renderer2DStatistics Renderer2D_GetStats();