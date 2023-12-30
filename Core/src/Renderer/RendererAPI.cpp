#include "pch.h"
#include "RendererAPI.h"
#include "Core/Window.h"
#include "DXTrace.h"
#include "Renderer2D.h"
#include "RendererContext.h"
#include "FrameBuffer.h"
#include "RenderStates.h"
#include "SceneRenderer.h"

#include <d2d1.h>
#include <dwrite.h>

struct RendererAPIState
{
	IDXGISwapChain* SwapChain;
	ID3D11Device* Device;
	ID3D11DeviceContext* DeviceContext;

	ID2D1Factory* D2DFactory;
	IDWriteFactory* DWriteFactory;
	ID2D1RenderTarget* D2DRenderTarget;

	uint32_t Width, Height;
	Vec4 ClearColor = { 0,0,0,0 };
	ID3D11RenderTargetView* RenderTargetView;
	ID3D11DepthStencilView* DepthStencilView;
};
static RendererAPIState s_RendererAPIState;

void SetBuffer(uint32_t width, uint32_t height)
{
	ID3D11Texture2D* backBuffer;
	CORE_CHECK_DX_RESULT(s_RendererAPIState.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	CORE_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateRenderTargetView(backBuffer, nullptr, &s_RendererAPIState.RenderTargetView));

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
	CORE_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthTexture));
	CORE_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateDepthStencilView(depthTexture, nullptr, &s_RendererAPIState.DepthStencilView));
	s_RendererAPIState.DeviceContext->OMSetRenderTargets(1, &s_RendererAPIState.RenderTargetView, s_RendererAPIState.DepthStencilView);

	s_RendererAPIState.Width = width;
	s_RendererAPIState.Height = height;

	// D2D
	{
		IDXGISurface* backBuffer;
		s_RendererAPIState.SwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

		FLOAT dpiX;
		FLOAT dpiY;
		s_RendererAPIState.D2DFactory->GetDesktopDpi(&dpiX, &dpiY);
		D2D1_RENDER_TARGET_PROPERTIES props = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT,
			D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), dpiX, dpiY);
		s_RendererAPIState.D2DFactory->CreateDxgiSurfaceRenderTarget(backBuffer, &props, &s_RendererAPIState.D2DRenderTarget);
	}
}

void RendererAPI_Initialize()
{
	s_RendererAPIState.SwapChain = RendererContext_GetSwapChain();
	s_RendererAPIState.Device = RendererContext_GetDevice();
	s_RendererAPIState.DeviceContext = RendererContext_GetDeviceContext();

	CORE_CHECK_DX_RESULT(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &s_RendererAPIState.D2DFactory));
	CORE_CHECK_DX_RESULT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&s_RendererAPIState.DWriteFactory)));

	SetBuffer(Window_GetWidth(), Window_GetHeight());
	CommonStates_Init();
	Renderer2D_Init();
	SceneRenderer_Init();
}

void RendererAPI_Shutdown()
{
	SceneRenderer_Shutdown();
	Renderer2D_Shutdown();
	CommonStates_Release();

	s_RendererAPIState.RenderTargetView->Release();
	s_RendererAPIState.DepthStencilView->Release();
	s_RendererAPIState.SwapChain->Release();
	s_RendererAPIState.DeviceContext->Release();
	s_RendererAPIState.Device->Release();

	s_RendererAPIState.D2DFactory->Release();
	s_RendererAPIState.DWriteFactory->Release();
	s_RendererAPIState.D2DRenderTarget->Release();
}

void RendererAPI_SetViewport(uint32_t width, uint32_t height)
{
	s_RendererAPIState.RenderTargetView->Release();
	s_RendererAPIState.DepthStencilView->Release();

	SetBuffer(width, height);
}

void RendererAPI_SetClearColor(const Vec4& color)
{
	s_RendererAPIState.ClearColor = color;
}

void RendererAPI_Clear()
{
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = s_RendererAPIState.Width;
	viewPort.Height = s_RendererAPIState.Height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	s_RendererAPIState.DeviceContext->RSSetViewports(1, &viewPort);

	s_RendererAPIState.DeviceContext->ClearRenderTargetView(s_RendererAPIState.RenderTargetView, &s_RendererAPIState.ClearColor.x);
	s_RendererAPIState.DeviceContext->ClearDepthStencilView(s_RendererAPIState.DepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1, 0);
	s_RendererAPIState.DeviceContext->OMSetRenderTargets(1, &s_RendererAPIState.RenderTargetView, s_RendererAPIState.DepthStencilView);
}

void RendererAPI_BeginRenderPass(const RenderPass& renderPass, bool clear)
{
	const Framebuffer& framebuffer = RenderPass_GetTargetFramebuffer(renderPass);
	const Pipeline& pipeline = RenderPass_GetPipeline(renderPass);
	const PipelineSpecification& pipelineSpec = Pipeline_GetSpecification(pipeline);
	const FramebufferSpecification& framebufferSpec = Framebuffer_GetSpecification(framebuffer);

	//if (clear)
	//{
	//	Framebuffer_ClearAttachment(framebuffer);
	//}
	//Framebuffer_Bind(framebuffer);
	Pipeline_Bind(pipeline);

	if (!pipelineSpec.DepthTest)
		s_RendererAPIState.DeviceContext->OMSetDepthStencilState(s_CommonStates.DSSNoDepthTest, 0);
	else
	{
		switch (pipelineSpec.DepthOperator)
		{
		case DepthCompareOperator_Less:
			s_RendererAPIState.DeviceContext->OMSetDepthStencilState(s_CommonStates.DSSLess, 0);
			break;
		case DepthCompareOperator_LessEqual:
			s_RendererAPIState.DeviceContext->OMSetDepthStencilState(s_CommonStates.DSSLessEqual, 0);
			break;
		}
	}

	if (pipelineSpec.BackfaceCulling)
		s_RendererAPIState.DeviceContext->RSSetState(s_CommonStates.RSCullBack);
	else
		s_RendererAPIState.DeviceContext->RSSetState(s_CommonStates.RSNoCull);

	switch (pipelineSpec.Blend)
	{
	case BlendMode_Alpha:
	{
		static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		s_RendererAPIState.DeviceContext->OMSetBlendState(s_CommonStates.BSAlpha, blendFactor, 0xffffffff);
		break;
	}
	case BlendMode_Additive:
	{
		static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		s_RendererAPIState.DeviceContext->OMSetBlendState(s_CommonStates.BSAdditive, blendFactor, 0xffffffff);
		break;
	}
	case BlendMode_Subtractive:
	{
		static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		s_RendererAPIState.DeviceContext->OMSetBlendState(s_CommonStates.BSSubtractive, blendFactor, 0xffffffff);
		break;
	}
	case BlendMode_Disabled:
	{
		static float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
		s_RendererAPIState.DeviceContext->OMSetBlendState(s_CommonStates.BSNoBlend, blendFactor, 0xffffffff);
		break;
	}
	}

	/*D3D11_VIEWPORT viewPort{};
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;

	if (framebufferSpec.SwapChainTarget)
	{
		viewPort.Width = s_RendererAPIState.Width;
		viewPort.Height = s_RendererAPIState.Height;
	}
	else
	{
		viewPort.Width = framebufferSpec.Width;
		viewPort.Height = framebufferSpec.Height;
	}

	s_RendererAPIState.DeviceContext->RSSetViewports(1, &viewPort);*/
	RnederPass_BindInputs(renderPass);
}

void RendererAPI_EndRenderPass()
{
	//s_RendererAPIState.DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);
	//RendererAPI_Clear();
}

void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, const Material& material, uint32_t indexCount)
{
	Material_Bind(material);

	RendererAPI_DrawIndexed(vertexBuffer, indexBuffer, indexCount);
}

void RendererAPI_DrawIndexed(const VertexBuffer& vertexBuffer, const IndexBuffer& indexBuffer, uint32_t indexCount)
{
	VertexBuffer_Bind(vertexBuffer);
	IndexBuffer_Bind(indexBuffer);

	s_RendererAPIState.DeviceContext->DrawIndexed(indexCount, 0, 0);
}

void RendererAPI_DrawLines(const VertexBuffer& vertexBuffer, uint32_t vertexCount)
{
	VertexBuffer_Bind(vertexBuffer);

	s_RendererAPIState.DeviceContext->Draw(vertexCount, 0);
}

void RendererAPI_DrawText(const WCHAR* str, const WCHAR* fontFamilyName, const Vec2& pos, const Vec4& color, float fontSize)
{
	IDWriteTextFormat* writeTextFormat;
	CORE_CHECK_DX_RESULT(s_RendererAPIState.DWriteFactory->CreateTextFormat(fontFamilyName, nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		fontSize, L"", &writeTextFormat));
	writeTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING);
	writeTextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR);

	D2D1_SIZE_F TargetSize = s_RendererAPIState.D2DRenderTarget->GetSize();

	IDWriteTextLayout* textLayout;
	s_RendererAPIState.DWriteFactory->CreateTextLayout(str, wcslen(str), writeTextFormat, TargetSize.width, TargetSize.height, &textLayout);

	D2D1_POINT_2F pounts;
	pounts.x = pos.x * TargetSize.width;
	pounts.y = pos.y * TargetSize.height;

	ID2D1SolidColorBrush* solidBrush;
	s_RendererAPIState.D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(color.x, color.y, color.z, color.w), &solidBrush);

	s_RendererAPIState.D2DRenderTarget->BeginDraw();
	s_RendererAPIState.D2DRenderTarget->DrawTextLayout(pounts, textLayout, solidBrush);
	s_RendererAPIState.D2DRenderTarget->EndDraw();
}
