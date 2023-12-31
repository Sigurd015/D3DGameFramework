#pragma once
#include "RendererContext.h"
#include "DXTrace.h"

struct CommonStates
{
	ID3D11DepthStencilState* DSSNoDepthTest;
	ID3D11DepthStencilState* DSSLess;
	ID3D11DepthStencilState* DSSLessEqual;

	ID3D11RasterizerState* RSNoCull;
	ID3D11RasterizerState* RSCullBack;

	ID3D11BlendState* BSNoBlend;
	ID3D11BlendState* BSAlpha;
	ID3D11BlendState* BSAdditive;
	ID3D11BlendState* BSSubtractive;

	ID3D11SamplerState* SSLinearWrap;
	ID3D11SamplerState* SSLinearClamp;
	ID3D11SamplerState* SSPointClamp;
	ID3D11SamplerState* SSAnisotropicWrap;
};
// Keep this global, make it easier to use
extern CommonStates s_CommonStates;

void CommonStates_Init();
void CommonStates_Release();