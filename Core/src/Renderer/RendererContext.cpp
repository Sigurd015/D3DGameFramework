#include "pch.h"
#include "RendererContext.h"
#include "DXTrace.h"

struct RendererContextState
{
	HWND* WindowHandle;
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

	ID3D11DepthStencilState* NoDepthTestStencilState;
	ID3D11DepthStencilState* DepthTestStencilState;

	ID3D11BlendState* BlendStateDisabled;
	ID3D11BlendState* BlendStateAlpha;
	ID3D11BlendState* BlendStateAdditive;
	ID3D11BlendState* BlendStateSubtractive;
};
static RendererContextState s_RendererContextState;

ID3D11DepthStencilState* CreateDepthStencilState(bool enable)
{
	ID3D11DepthStencilState* depthStencilState = nullptr;

	D3D11_DEPTH_STENCIL_DESC depthTestingDesc = {};

	if (!enable)
	{
		depthTestingDesc.DepthEnable = false;
		depthTestingDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	}
	else
	{
		depthTestingDesc.DepthEnable = true;
		depthTestingDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	}

	depthTestingDesc.DepthFunc = D3D11_COMPARISON_LESS;
	CORE_CHECK_DX_RESULT(s_RendererContextState.Device->CreateDepthStencilState(&depthTestingDesc, &depthStencilState));

	return depthStencilState;
}

ID3D11BlendState* CreateBlendState(BlendMode type)
{
	ID3D11BlendState* blendState = nullptr;

	D3D11_BLEND_DESC blendDesc = {};
	blendDesc.AlphaToCoverageEnable = FALSE;
	blendDesc.IndependentBlendEnable = FALSE;

	switch (type)
	{
	case BlendMode_Disabled:
	{
		blendDesc.RenderTarget[0].BlendEnable = false;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}
	case BlendMode_Alpha:
	{
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}
	case BlendMode_Additive:
	{
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}
	case BlendMode_Subtractive:
	{
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		break;
	}
	}

	CORE_CHECK_DX_RESULT(s_RendererContextState.Device->CreateBlendState(&blendDesc, &blendState));

	return blendState;
}

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
	swapChainDesc.Flags = 0;

	CORE_CHECK_DX_RESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_BGRA_SUPPORT, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&s_RendererContextState.SwapChain, &s_RendererContextState.Device, nullptr, &s_RendererContextState.DeviceContext));

	s_RendererContextState.NoDepthTestStencilState = CreateDepthStencilState(false);
	s_RendererContextState.DepthTestStencilState = CreateDepthStencilState(true);
	s_RendererContextState.BlendStateDisabled = CreateBlendState(BlendMode_Disabled);
	s_RendererContextState.BlendStateAlpha = CreateBlendState(BlendMode_Alpha);
	s_RendererContextState.BlendStateAdditive = CreateBlendState(BlendMode_Additive);
	s_RendererContextState.BlendStateSubtractive = CreateBlendState(BlendMode_Subtractive);
}

void RendererContext_SwapBuffer(bool VSync)
{
	s_RendererContextState.SwapChain->Present(VSync, 0);
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

ID3D11DepthStencilState* RendererContext_GetDepthStencilState(bool enable)
{
	if (!enable)
	{
		return s_RendererContextState.NoDepthTestStencilState;
	}
	else
	{
		return s_RendererContextState.DepthTestStencilState;
	}
}

ID3D11BlendState* RendererContext_GetBlendState(BlendMode type)
{
	switch (type)
	{
	case BlendMode_Disabled:
		return s_RendererContextState.BlendStateDisabled;
	case BlendMode_Alpha:
		return s_RendererContextState.BlendStateAlpha;
	case BlendMode_Additive:
		return s_RendererContextState.BlendStateAdditive;
	case BlendMode_Subtractive:
		return s_RendererContextState.BlendStateSubtractive;
	}

	CORE_ASSERT(false, "Unknown blend mode type");

	return nullptr;
}