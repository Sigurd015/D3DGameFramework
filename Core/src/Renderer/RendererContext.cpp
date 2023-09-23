#include "pch.h"
#include "RendererContext.h"
#include "DXTrace.h"

struct RendererContextState
{
	HWND* WindowHandle;
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;
};
static RendererContextState s_RendererContextState;

void RendererContext_Initialize(HWND* windowHandle)
{
	s_RendererContextState.WindowHandle = windowHandle;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.OutputWindow = *windowHandle;
	swapChainDesc.Windowed = true;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	//swapChainDesc.Flags = 0;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	CORE_CHECK_DX_RESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&s_RendererContextState.SwapChain, &s_RendererContextState.Device, nullptr, &s_RendererContextState.DeviceContext));
}

void RendererContext_SwapBuffer(bool VSync)
{
	s_RendererContextState.SwapChain->Present(VSync, 0);
}

void RendererContext_SetFullscreen(bool fullscreen)
{
	s_RendererContextState.SwapChain->SetFullscreenState(fullscreen, nullptr);
}

IDXGISwapChain* RendererContext_GetSwapChain()
{
	return s_RendererContextState.SwapChain;
}

ID3D11Device* RendererContext_GetDevice()
{
	return s_RendererContextState.Device;
}

ID3D11DeviceContext* RendererContext_GetDeviceContext()
{
	return s_RendererContextState.DeviceContext;
}