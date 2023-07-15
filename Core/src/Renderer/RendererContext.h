#pragma once
#include <Windows.h>
#include <d3d11.h>

void RendererContext_Initialize(HWND* windowHandle);
void RendererContext_SwapBuffer(bool VSync);
IDXGISwapChain* RendererContext_GetSwapChain();
ID3D11Device* RendererContext_GetDevice();
ID3D11DeviceContext* RendererContext_GetDeviceContext();