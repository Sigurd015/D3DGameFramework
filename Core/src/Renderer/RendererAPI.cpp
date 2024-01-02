#include "pch.h"
#include "RendererAPI.h"
#include "Core/Window.h"
#include "DXTrace.h"
#include "Renderer2D.h"
#include "RendererContext.h"
#include "FrameBuffer.h"
#include "RenderStates.h"
#include "SceneRenderer.h"
#include "Asset/AssetManager.h"
#include "RendererResource.h"

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
	float ClearColor[4] = { 0,0,0,0 };
	ID3D11RenderTargetView* RenderTargetView = nullptr;
};
static RendererAPIState s_RendererAPIState;

struct RendererData
{
	// For Equirectangular To CubemapPass
	ComputePass EquirectangularToCubemapPass;
	ComputePass EnvironmentMipFilterPass;
	ComputePass EnvironmentIrradiancePass;

	// Composite To SwapChain Pass
	RenderPass CompositePass;

	struct CBFilterParam
	{
		float Roughness;

		char padding[12];
	};
	ConstantBuffer FilterParam;
	struct CBSamplesParams
	{
		uint32_t Samples;

		char padding[12];
	};
	ConstantBuffer SamplesParam;
};
static RendererData s_Data;

void RendererAPI_Initialize()
{
	s_Data = { };
	s_RendererAPIState = { };

	s_RendererAPIState.SwapChain = RendererContext_GetSwapChain();
	s_RendererAPIState.Device = RendererContext_GetDevice();
	s_RendererAPIState.DeviceContext = RendererContext_GetDeviceContext();

	CORE_CHECK_DX_RESULT(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &s_RendererAPIState.D2DFactory));
	CORE_CHECK_DX_RESULT(DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory),
		reinterpret_cast<IUnknown**>(&s_RendererAPIState.DWriteFactory)));

	RendererAPI_SetViewport(Window_GetWidth(), Window_GetHeight());
	RendererResourcePool_Init();
	CommonStates_Init();
	Renderer2D_Init();
	SceneRenderer_Init();

	// For Equirectangular To CubemapPass
	{
		ComputePassSpecification spec = {};
		ComputePipelineSpecification pipelineSpec = {};
		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("EquirectangularToCubeMap", AssetType_Shader);
		ComputePipeline_Create(spec.Pipeline, pipelineSpec);
		ComputePass_Create(s_Data.EquirectangularToCubemapPass, spec);
	}

	{
		ComputePassSpecification spec = {};
		ComputePipelineSpecification pipelineSpec = {};
		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("EnvironmentMipFilter", AssetType_Shader);
		ComputePipeline_Create(spec.Pipeline, pipelineSpec);
		ComputePass_Create(s_Data.EnvironmentMipFilterPass, spec);

		ConstantBuffer_Create(s_Data.FilterParam, sizeof(RendererData::CBFilterParam));
		ComputePass_SetInput(s_Data.EnvironmentMipFilterPass, "CBFilterParam", RendererResourceType_ConstantBuffer, &s_Data.FilterParam);
	}

	{
		ComputePassSpecification spec = {};
		ComputePipelineSpecification pipelineSpec = {};
		pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("EnvironmentIrradiance", AssetType_Shader);
		ComputePipeline_Create(spec.Pipeline, pipelineSpec);
		ComputePass_Create(s_Data.EnvironmentIrradiancePass, spec);

		ConstantBuffer_Create(s_Data.SamplesParam, sizeof(RendererData::CBSamplesParams));
		ComputePass_SetInput(s_Data.EnvironmentIrradiancePass, "CBSamplesParams", RendererResourceType_ConstantBuffer, &s_Data.SamplesParam);
	}

	// Composite To SwapChain Pass
	{
		PipelineSpecification pipelineSpec;
		{
			FramebufferSpecification spec;
			spec.SwapChainTarget = true;
			pipelineSpec.TargetFramebuffer = (Framebuffer*)RendererResourcePool_GetResource(RendererResourceType_FrameBuffer, &spec);
		}
		{
			VertexBufferLayoutEmelent FullScreenQuadLayout[2] = {
				{ ShaderDataType::Float3, "a_Position" },
				{ ShaderDataType::Float2, "a_TexCoord" },
			};

			VertexBufferLayout FullScreenQuadLayoutInfo = { FullScreenQuadLayout, 2 };
			VertexBufferLayout_CalculateOffsetsAndStride(FullScreenQuadLayoutInfo);

			pipelineSpec.Layout = FullScreenQuadLayoutInfo;
			pipelineSpec.Shader = (Shader*)AssetManager_GetAsset("CompositeToSwapChain", AssetType_Shader);
			pipelineSpec.BackfaceCulling = false;
			pipelineSpec.DepthTest = false;
			pipelineSpec.Topology = PrimitiveTopology_Triangles;

			RenderPassSpecification renderPassSpec;
			Pipeline_Create(renderPassSpec.Pipeline, pipelineSpec);
			RenderPass_Create(s_Data.CompositePass, renderPassSpec);
		}
	}
}

void RendererAPI_Shutdown()
{
	SceneRenderer_Shutdown();
	Renderer2D_Shutdown();
	CommonStates_Release();
	RendererResourcePool_Shutdown();

	s_RendererAPIState.RenderTargetView->Release();
	s_RendererAPIState.SwapChain->Release();
	s_RendererAPIState.DeviceContext->Release();
	s_RendererAPIState.Device->Release();

	s_RendererAPIState.D2DFactory->Release();
	s_RendererAPIState.DWriteFactory->Release();
	s_RendererAPIState.D2DRenderTarget->Release();

	{
		ComputePass_Release(s_Data.EquirectangularToCubemapPass);

		ConstantBuffer_Release(s_Data.FilterParam);
		ComputePass_Release(s_Data.EnvironmentMipFilterPass);

		ConstantBuffer_Release(s_Data.SamplesParam);
		ComputePass_Release(s_Data.EnvironmentIrradiancePass);
	}

	{
		RenderPass_Release(s_Data.CompositePass);
	}
}

void RendererAPI_SetViewport(uint32_t width, uint32_t height)
{
	if (s_RendererAPIState.RenderTargetView)
		s_RendererAPIState.RenderTargetView->Release();

	ID3D11Texture2D* backBuffer;
	CORE_CHECK_DX_RESULT(s_RendererAPIState.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	CORE_CHECK_DX_RESULT(s_RendererAPIState.Device->CreateRenderTargetView(backBuffer, nullptr, &s_RendererAPIState.RenderTargetView));

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

void RendererAPI_Clear()
{
	s_RendererAPIState.DeviceContext->ClearRenderTargetView(s_RendererAPIState.RenderTargetView, s_RendererAPIState.ClearColor);
}

void RendererAPI_ResetToSwapChain()
{
	D3D11_VIEWPORT viewPort{};
	viewPort.Width = s_RendererAPIState.Width;
	viewPort.Height = s_RendererAPIState.Height;
	viewPort.MinDepth = 0;
	viewPort.MaxDepth = 1.0f;
	viewPort.TopLeftX = 0;
	viewPort.TopLeftY = 0;
	s_RendererAPIState.DeviceContext->RSSetViewports(1, &viewPort);

	s_RendererAPIState.DeviceContext->OMSetRenderTargets(1, &s_RendererAPIState.RenderTargetView, nullptr);
}

void RendererAPI_BeginComputePass(const ComputePass& computePass)
{
	const ComputePipeline& pipeline = ComputePass_GetPipeline(computePass);
	ComputePipeline_Bind(pipeline);

	ComputePass_BindInputs(computePass);
}

void RendererAPI_EndComputePass(const ComputePass& computePass)
{
	ComputePass_UnbindInputs(computePass);
}

EnvMap RendererAPI_CreateEnvironmentMap(Texture2D* equirectangularMap)
{
	CORE_ASSERT(Texture2D_GetFormat(equirectangularMap) == ImageFormat_RGBA32F, "Texture is not HDR!");

	EnvMap envMap;

	static const uint32_t cubemapSize = 1024;
	static const uint32_t irradianceMapSize = 32;

	TextureSpecification cubemapSpec = {};
	cubemapSpec.Width = cubemapSize;
	cubemapSpec.Height = cubemapSize;
	cubemapSpec.Format = ImageFormat_RGBA32F;

	TextureCube envUnfiltered;
	TextureCube_Create(&envUnfiltered, cubemapSpec);

	// Radiance map (Equirectangular to cubemap) 
	{
		TextureCube_CreateUAV(&envUnfiltered);

		ComputePass_SetInput(s_Data.EquirectangularToCubemapPass, "u_EquirectangularMap", RendererResourceType_TextureCube, equirectangularMap);
		ComputePass_SetInput(s_Data.EquirectangularToCubemapPass, "o_OutputTex", RendererResourceType_TextureCube, &envUnfiltered);

		RendererAPI_BeginComputePass(s_Data.EquirectangularToCubemapPass);
		RendererContext_GetDeviceContext()->Dispatch(cubemapSize / 32, cubemapSize / 32, 6);
		RendererAPI_EndComputePass(s_Data.EquirectangularToCubemapPass);

		TextureCube_GenerateMips(&envUnfiltered);
	}

	// Radiance map (Filter)	
	TextureCube envFiltered;
	TextureCube_Create(&envFiltered, cubemapSpec);
	{
		// Copy Unfiltered envmap to Filtered envmap (Keep the first mip level)
		RendererContext_GetDeviceContext()->CopyResource(TextureCube_GetTexture(&envFiltered), TextureCube_GetTexture(&envUnfiltered));

		// Cubemap size is 1024, so we need 11 mip levels (1024 -> 512 -> 256 -> 128 -> 64 -> 32 -> 16 -> 8 -> 4 -> 2 -> 1)
		const static uint32_t mipCount = 11;

		ComputePass_SetInput(s_Data.EnvironmentMipFilterPass, "u_InputTex", RendererResourceType_TextureCube, &envUnfiltered);

		const float deltaRoughness = 1.0f / DirectX::XMMax((float)mipCount - 1.0f, 1.0f);
		for (uint32_t i = 1, size = cubemapSize; i < mipCount; i++, size /= 2)
		{
			uint32_t numGroups = DirectX::XMMax(1u, size / 32);
			RendererData::CBFilterParam filterParam = { i * deltaRoughness };
			ConstantBuffer_SetData(s_Data.FilterParam, &filterParam);
			TextureCube_CreateUAV(&envFiltered, i);

			ComputePass_SetInput(s_Data.EnvironmentMipFilterPass, "o_OutputTex", RendererResourceType_TextureCube, &envFiltered);

			RendererAPI_BeginComputePass(s_Data.EnvironmentMipFilterPass);
			RendererContext_GetDeviceContext()->Dispatch(numGroups, numGroups, 6);
			RendererAPI_EndComputePass(s_Data.EnvironmentMipFilterPass);
		}
	}

	// Irradiance map
	cubemapSpec.Width = irradianceMapSize;
	cubemapSpec.Height = irradianceMapSize;
	TextureCube irradianceMap;
	TextureCube_Create(&irradianceMap, cubemapSpec);
	{
		TextureCube_CreateUAV(&irradianceMap);

		ComputePass_SetInput(s_Data.EnvironmentIrradiancePass, "u_RadianceMap", RendererResourceType_TextureCube, &envFiltered);
		ComputePass_SetInput(s_Data.EnvironmentIrradiancePass, "o_IrradianceMap", RendererResourceType_TextureCube, &irradianceMap);

		RendererData::CBSamplesParams samplesParams = { 512 };
		ConstantBuffer_SetData(s_Data.SamplesParam, &samplesParams);

		RendererAPI_BeginComputePass(s_Data.EnvironmentIrradiancePass);
		RendererContext_GetDeviceContext()->Dispatch(irradianceMapSize / 32, irradianceMapSize / 32, 6);
		RendererAPI_EndComputePass(s_Data.EnvironmentIrradiancePass);

		TextureCube_GenerateMips(&irradianceMap);
	}
	envMap.RadianceMap = envFiltered;
	envMap.IrradianceMap = irradianceMap;

	return envMap;
}

void RendererAPI_BeginRenderPass(const RenderPass& renderPass, bool clear)
{
	const Framebuffer* framebuffer = RenderPass_GetTargetFramebuffer(renderPass);
	const Pipeline& pipeline = RenderPass_GetPipeline(renderPass);
	const PipelineSpecification& pipelineSpec = Pipeline_GetSpecification(pipeline);
	const FramebufferSpecification& framebufferSpec = Framebuffer_GetSpecification(framebuffer);

	if (framebufferSpec.SwapChainTarget)
	{
		if (clear)
		{
			RendererAPI_Clear();
		}

		RendererAPI_ResetToSwapChain();
	}
	else
	{
		if (clear)
		{
			Framebuffer_ClearAttachment(framebuffer);
		}

		D3D11_VIEWPORT viewPort{};
		viewPort.MinDepth = 0;
		viewPort.MaxDepth = 1.0f;
		viewPort.TopLeftX = 0;
		viewPort.TopLeftY = 0;
		viewPort.Width = framebufferSpec.Width;
		viewPort.Height = framebufferSpec.Height;

		s_RendererAPIState.DeviceContext->RSSetViewports(1, &viewPort);
		Framebuffer_Bind(framebuffer);
	}

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

	RnederPass_BindInputs(renderPass);
}

void RendererAPI_EndRenderPass()
{
	s_RendererAPIState.DeviceContext->OMSetRenderTargets(0, nullptr, nullptr);

	RendererAPI_ResetToSwapChain();
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


void RendererAPI_DrawMesh(const Mesh* mesh, const Material* material)
{
	if (material)
		Material_Bind(*material);

	const VertexBuffer& vertexBuffer = Mesh_GetVertexBuffers(mesh);
	const IndexBuffer& indexBuffer = Mesh_GetIndexBuffer(mesh);

	VertexBuffer_Bind(vertexBuffer);
	IndexBuffer_Bind(indexBuffer);

	s_RendererAPIState.DeviceContext->DrawIndexed(IndexBuffer_GetCount(indexBuffer), 0, 0);
}

void RendererAPI_DrawFullScreenQuad()
{
	s_RendererAPIState.DeviceContext->Draw(3, 0);
}

void RendererAPI_CompositeToSwapChain(const Image2D* finalImage)
{
	RendererAPI_ResetToSwapChain();

	RenderPass_SetInput(s_Data.CompositePass, "u_FinalImage", RendererResourceType_Image, finalImage);

	RendererAPI_BeginRenderPass(s_Data.CompositePass, true);
	RendererAPI_DrawFullScreenQuad();
	RendererAPI_EndRenderPass();
}