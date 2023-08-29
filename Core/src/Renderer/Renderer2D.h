#pragma once
#include "../Math/Math.h"
#include "Texture.h"
#include "Utils/RefPtr.h"

#define COMMAND_BUFFER_SIZE 1024

void Renderer2D_Initialize();
void Renderer2D_Shutdown();
void Renderer2D_BeginScene(const Mat& viewProjection);
void Renderer2D_EndScene();
void Renderer2D_DrawQuad(const Mat& transform, const Vec4& color);
void Renderer2D_DrawQuad(const Mat& transform, RefPtr* texture, const Vec2& uv0, const Vec2& uv1,
	const Vec4& tintColor, float tilingFactor = 1.0f);
void Renderer2D_DrawCircle(const Mat& transform, const Vec4& color, float thickness = 1.0f, float fade = 0.005f);
void Renderer2D_DrawLine(const Vec3& p0, Vec3& p1, const Vec4& color);
void Renderer2D_DrawRect(const Vec3& position, const Vec2& size, const Vec4& color);
void Renderer2D_DrawRect(const Mat& transform, const Vec4& color);
void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, const Vec4& color);
void Renderer2D_DrawUI(const Vec2& pos, const Vec2& size, RefPtr* texture, const Vec2& uv0, const Vec2& uv1,
	const Vec4& tintColor, float tilingFactor = 1.0f);
void Renderer2D_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize);