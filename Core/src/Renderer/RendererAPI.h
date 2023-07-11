#pragma once
#include "Core/base.h"

#include <Windows.h>
#include <DirectXMath.h>

void RendererContext_Initialize(HWND* windowHandle);
void RendererContext_SwapBuffer(bool VSync);

void RendererAPI_Initialize();
void RendererAPI_SetViewport(uint32_t width, uint32_t height);
void RendererAPI_SetClearColor(const Vec4* color);
void RendererAPI_Clear();