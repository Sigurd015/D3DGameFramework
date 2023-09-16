#include "pch.h"
#include "RenderStates.h"

CommonStates s_CommonStates;

void CommonStates_Init()
{
	// Depth Stencil States
	{
		D3D11_DEPTH_STENCIL_DESC depthTestingDesc = {};
		depthTestingDesc.DepthEnable = false;
		depthTestingDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthTestingDesc.DepthFunc = D3D11_COMPARISON_LESS;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateDepthStencilState(&depthTestingDesc, &s_CommonStates.DSSNoDepthTest));
	}
	{
		D3D11_DEPTH_STENCIL_DESC depthTestingDesc = {};
		depthTestingDesc.DepthEnable = true;
		depthTestingDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthTestingDesc.DepthFunc = D3D11_COMPARISON_LESS;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateDepthStencilState(&depthTestingDesc, &s_CommonStates.DSSLess));
	}
	{
		D3D11_DEPTH_STENCIL_DESC depthTestingDesc = {};
		depthTestingDesc.DepthEnable = true;
		depthTestingDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
		depthTestingDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
		depthTestingDesc.StencilEnable = false;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateDepthStencilState(&depthTestingDesc, &s_CommonStates.DSSLessEqual));
	}

	// Rasterizer States
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_NONE;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateRasterizerState(&rasterizerDesc, &s_CommonStates.RSNoCull));
	}
	{
		D3D11_RASTERIZER_DESC rasterizerDesc = {};
		rasterizerDesc.FillMode = D3D11_FILL_SOLID;
		rasterizerDesc.CullMode = D3D11_CULL_BACK;
		rasterizerDesc.FrontCounterClockwise = false;
		rasterizerDesc.DepthClipEnable = true;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateRasterizerState(&rasterizerDesc, &s_CommonStates.RSCullBack));
	}

	// Blend States
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = false;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBlendState(&blendDesc, &s_CommonStates.BSNoBlend));
	}
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBlendState(&blendDesc, &s_CommonStates.BSAlpha));
	}
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBlendState(&blendDesc, &s_CommonStates.BSAdditive));
	}
	{
		D3D11_BLEND_DESC blendDesc = {};
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_REV_SUBTRACT;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateBlendState(&blendDesc, &s_CommonStates.BSSubtractive));
	}

	// Sampler State
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateSamplerState(&samplerDesc, &s_CommonStates.SSLinearWrap));
	}
	{
		D3D11_SAMPLER_DESC samplerDesc = {};
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		CORE_CHECK_DX_RESULT(RendererContext_GetDevice()->CreateSamplerState(&samplerDesc, &s_CommonStates.SSLinearClamp));
	}
}

void CommonStates_Release()
{
	s_CommonStates.DSSNoDepthTest->Release();
	s_CommonStates.DSSLess->Release();
	s_CommonStates.DSSLessEqual->Release();

	s_CommonStates.RSNoCull->Release();
	s_CommonStates.RSCullBack->Release();

	s_CommonStates.BSNoBlend->Release();
	s_CommonStates.BSAlpha->Release();
	s_CommonStates.BSAdditive->Release();
	s_CommonStates.BSSubtractive->Release();

	s_CommonStates.SSLinearWrap->Release();
	s_CommonStates.SSLinearClamp->Release();
}