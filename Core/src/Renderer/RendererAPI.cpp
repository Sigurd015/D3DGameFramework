#include "pch.h"
#include "RendererAPI.h"
#include "Core/Window.h"

#include <d3d11.h>

void CheckD3DError(HRESULT result)
{
	if (result == S_OK)
		return;

	switch (result)
	{
	case D3D11_ERROR_FILE_NOT_FOUND:
		BV_ASSERT(false, "D3D11_ERROR_FILE_NOT_FOUND");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS:
		BV_ASSERT(false, "D3D11_ERROR_TOO_MANY_UNIQUE_STATE_OBJECTS");
		break;
	case D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS:
		BV_ASSERT(false, "D3D11_ERROR_TOO_MANY_UNIQUE_VIEW_OBJECTS");
		break;
	case D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD:
		BV_ASSERT(false, "D3D11_ERROR_DEFERRED_CONTEXT_MAP_WITHOUT_INITIAL_DISCARD");
		break;
	case DXGI_ERROR_INVALID_CALL:
		BV_ASSERT(false, "DXGI_ERROR_INVALID_CALL");
		break;
	case DXGI_ERROR_WAS_STILL_DRAWING:
		BV_ASSERT(false, "DXGI_ERROR_WAS_STILL_DRAWING");
		break;
	case E_FAIL:
		BV_ASSERT(false, "E_FAIL");
		break;
	case E_INVALIDARG:
		BV_ASSERT(false, "E_INVALIDARG");
		break;
	case E_OUTOFMEMORY:
		BV_ASSERT(false, "E_OUTOFMEMORY");
		break;
	case E_NOTIMPL:
		BV_ASSERT(false, "E_NOTIMPL");
		break;
	case S_FALSE:
		BV_ASSERT(false, "S_FALSE");
		break;
	}
}

#ifdef BV_DEBUG
#define BV_CHECK_RESULT(x) { HRESULT __hr = x;CheckD3DError(__hr);}
#else
#define BV_CHECK_RESULT(x) x
#endif

struct RendererState
{
	HWND* WindowHandle;
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

	Vec4 ClearColor = { 0,0,0,0 };
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
};

static RendererState s_RendererState;

void RendererContext_Initialize(HWND* windowHandle)
{
	s_RendererState.WindowHandle = windowHandle;

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
	swapChainDesc.Flags = 0;

	BV_CHECK_RESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&s_RendererState.SwapChain, &s_RendererState.Device, nullptr, &s_RendererState.DeviceContext));
}

void RendererContext_SwapBuffer(bool VSync)
{
	s_RendererState.SwapChain->Present(VSync, 0);
}

void SetBuffer(uint32_t width, uint32_t height)
{
	ID3D11Texture2D* backBuffer;
	BV_CHECK_RESULT(s_RendererState.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	BV_CHECK_RESULT(s_RendererState.Device->CreateRenderTargetView(backBuffer, nullptr, &s_RendererState.RenderTargetView));

	ID3D11Texture2D* depthTexture;
	D3D11_TEXTURE2D_DESC depthStencilDesc = {};
	depthStencilDesc.Width = width;
	depthStencilDesc.Height = height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	BV_CHECK_RESULT(s_RendererState.Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthTexture));
	BV_CHECK_RESULT(s_RendererState.Device->CreateDepthStencilView(depthTexture, nullptr, &s_RendererState.DepthStencilView));
	s_RendererState.DeviceContext->OMSetRenderTargets(1, &s_RendererState.RenderTargetView, s_RendererState.DepthStencilView);

	D3D11_VIEWPORT viewPort{};
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	s_RendererState.DeviceContext->RSSetViewports(1, &viewPort);
}

void RendererAPI_Initialize()
{
	SetBuffer(Window_GetWidth(), Window_GetHeight());
}

void RendererAPI_SetViewport(uint32_t width, uint32_t height)
{
	s_RendererState.RenderTargetView->Release();
	s_RendererState.DepthStencilView->Release();

	SetBuffer(width, height);
}

void RendererAPI_SetClearColor(const Vec4* color)
{
	s_RendererState.ClearColor = *color;
}

void RendererAPI_Clear()
{
	s_RendererState.DeviceContext->ClearRenderTargetView(s_RendererState.RenderTargetView, &s_RendererState.ClearColor.x);
	s_RendererState.DeviceContext->ClearDepthStencilView(s_RendererState.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	s_RendererState.DeviceContext->OMSetRenderTargets(1, &s_RendererState.RenderTargetView, s_RendererState.DepthStencilView);
}