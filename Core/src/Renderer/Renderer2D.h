#pragma once
#include "../Math/Math.h"
#include "Texture.h"

void Renderer2D_Initialize();
void Renderer2D_Shutdown();
void Renderer2D_BeginScene(const Mat& viewProjection);
void Renderer2D_EndScene();
void DrawQuad(const Mat& transform, const Vec4& color);
void DrawQuad(const Mat& transform, const Texture2D& texture, Vec2 uv0, Vec2 uv1,
	static const Vec4& tintColor = Vec4(1.0f, 1.0f, 1.0f, 1.0f), float tilingFactor = 1.0f);
void DrawCircle(const Mat& transform, const Vec4& color, static float thickness = 1.0f, float fade = 0.005f);
void DrawLine(const Vec3& p0, Vec3& p1, const Vec4& color);
void DrawRect(const Vec3& position, const Vec2& size, const Vec4& color);
void DrawRect(const Mat& transform, const Vec4& color);