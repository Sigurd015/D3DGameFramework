#pragma once

void Renderer2D_Initialize();
void Renderer2D_Shutdown();
void Renderer2D_BeginScene();
void Renderer2D_EndScene();
void Renderer2D_DrawQuad(const Vec2* position, const Vec2* size, const Vec4* color);