#include "pch.h"
#include "RendererAPI.h"
#include "Core/Window.h"
#include "DXTrace.h"
#include "Renderer2D.h"
#include "RendererContext.h"

struct RendererAPIState
{
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

	Vec4 ClearColor = { 0,0,0,0 };
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
};
static RendererAPIState s_RendererAPIState;

void SetBuffer(uint32_t width, uint32_t height)
{
	ID3D11Texture2D* backBuffer;
	BV_CHECK_DX_RESULT(s_RendererAPIState.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	BV_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateRenderTargetView(backBuffer, nullptr, &s_RendererAPIState.RenderTargetView));

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
	BV_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthTexture));
	BV_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateDepthStencilView(depthTexture, nullptr, &s_RendererAPIState.DepthStencilView));
	s_RendererAPIState.DeviceContext->OMSetRenderTargets(1, &s_RendererAPIState.RenderTargetView, s_RendererAPIState.DepthStencilView);

	D3D11_VIEWPORT viewPort{};
	viewPort.Width = width;
	viewPort.Height = height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	s_RendererAPIState.DeviceContext->RSSetViewports(1, &viewPort);
}

void RendererAPI_Initialize()
{
	s_RendererAPIState.SwapChain = RendererContext_GetSwapChain();
	s_RendererAPIState.Device = RendererContext_GetDevice();
	s_RendererAPIState.DeviceContext = RendererContext_GetDeviceContext();

	SetBuffer(Window_GetWidth(), Window_GetHeight());

	Renderer2D_Initialize();
}

void RendererAPI_Shutdown()
{
	Renderer2D_Shutdown();

	s_RendererAPIState.RenderTargetView->Release();
	s_RendererAPIState.DepthStencilView->Release();
	s_RendererAPIState.SwapChain->Release();
	s_RendererAPIState.DeviceContext->Release();
	s_RendererAPIState.Device->Release();
}

void RendererAPI_SetViewport(uint32_t width, uint32_t height)
{
	s_RendererAPIState.RenderTargetView->Release();
	s_RendererAPIState.DepthStencilView->Release();

	SetBuffer(width, height);
}

void RendererAPI_SetClearColor(const Vec4* color)
{
	s_RendererAPIState.ClearColor = *color;
}

void RendererAPI_Clear()
{
	s_RendererAPIState.DeviceContext->ClearRenderTargetView(s_RendererAPIState.RenderTargetView, &s_RendererAPIState.ClearColor.x);
	s_RendererAPIState.DeviceContext->ClearDepthStencilView(s_RendererAPIState.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	s_RendererAPIState.DeviceContext->OMSetRenderTargets(1, &s_RendererAPIState.RenderTargetView, s_RendererAPIState.DepthStencilView);
}

void RendererAPI_DrawIndexed(const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, uint32_t indexCount)
{

}

void RendererAPI_DrawLines(const VertexBuffer* vertexBuffer, uint32_t vertexCount)
{

}