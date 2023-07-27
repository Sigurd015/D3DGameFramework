#pragma once
#include <Windows.h>
#include <d3d11.h>

enum BlendMode
{
	BlendMode_Disabled = 0,
	BlendMode_Alpha,
	BlendMode_Additive,
	BlendMode_Subtractive,
};

void RendererContext_Initialize(HWND* windowHandle);
void RendererContext_SwapBuffer(bool VSync);
IDXGISwapChain* RendererContext_GetSwapChain();
ID3D11Device* RendererContext_GetDevice();
ID3D11DeviceContext* RendererContext_GetDeviceContext();
ID3D11DepthStencilState* RendererContext_GetDepthStencilState(bool enable);
ID3D11BlendState* RendererContext_GetBlendState(BlendMode type);