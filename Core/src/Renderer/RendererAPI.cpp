#include "pch.h"
#include "RendererAPI.h"
#include "Core/Window.h"

#include <d3dcompiler.h>
#include <atlbase.h>
#include <atlconv.h>

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
#define BV_CHECK_DX_RESULT(x) { HRESULT __hr = x;CheckD3DError(__hr);}
#else
#define BV_CHECK_DX_RESULT(x) x
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

	BV_CHECK_DX_RESULT(D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &swapChainDesc,
		&s_RendererState.SwapChain, &s_RendererState.Device, nullptr, &s_RendererState.DeviceContext));
}

void RendererContext_SwapBuffer(bool VSync)
{
	s_RendererState.SwapChain->Present(VSync, 0);
}

void SetBuffer(uint32_t width, uint32_t height)
{
	ID3D11Texture2D* backBuffer;
	BV_CHECK_DX_RESULT(s_RendererState.SwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBuffer));
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateRenderTargetView(backBuffer, nullptr, &s_RendererState.RenderTargetView));

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
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateTexture2D(&depthStencilDesc, nullptr, &depthTexture));
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateDepthStencilView(depthTexture, nullptr, &s_RendererState.DepthStencilView));
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

void RendererAPI_DrawIndexed(const VertexBuffer* vertexBuffer, const IndexBuffer* indexBuffer, uint32_t indexCount)
{}

void RendererAPI_DrawLines(const VertexBuffer* vertexBuffer, uint32_t vertexCount)
{}

//-----------------------------------------
//------------Vertex Buffer----------------
//-----------------------------------------

void VertexBufferLayout_CalculateOffsetsAndStride(VertexBufferLayout* out)
{
	uint32_t offset = 0;
	out->Stride = 0;
	for (uint32_t i = 0; i < out->ElementCount; i++)
	{
		out->Elements[i].Offset = offset;
		offset += out->Elements[i].Size;
		out->Stride += out->Elements[i].Size;
	}
}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Vertex Buffer----------------
//-----------------------------------------

void VertexBuffer_Create(VertexBuffer* out, uint32_t size)
{
	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = size;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = out->Stride;
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateBuffer(&bufferDesc, nullptr, &out->Buffer));
}

void VertexBuffer_SetData(VertexBuffer* out, void* data, uint32_t size)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	BV_CHECK_DX_RESULT(s_RendererState.DeviceContext->Map(out->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, data, size);
	s_RendererState.DeviceContext->Unmap(out->Buffer, 0);
}

void VertexBuffer_SetLayout(VertexBuffer* out, VertexBufferLayout* layout)
{
	out->Layout = *layout;
	out->Stride = layout->Stride;
}

void VertexBuffer_Bind(VertexBuffer* out)
{
	const UINT offset = 0;
	s_RendererState.DeviceContext->IASetVertexBuffers(0, 1, &out->Buffer, &out->Stride, &offset);
}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Index Buffer-----------------
//-----------------------------------------

void IndexBuffer_Create(IndexBuffer* out, void* indices, uint32_t count)
{
	out->Count = count;

	D3D11_SUBRESOURCE_DATA resourceData = {};
	resourceData.pSysMem = indices;

	D3D11_BUFFER_DESC bufferDesc = { 0 };
	bufferDesc.ByteWidth = count * sizeof(uint32_t);
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateBuffer(&bufferDesc, &resourceData, &out->Buffer));
}

void IndexBuffer_Bind(IndexBuffer* out)
{
	s_RendererState.DeviceContext->IASetIndexBuffer(out->Buffer, DXGI_FORMAT_R32_UINT, 0);
}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Shader-----------------------
//-----------------------------------------

void Shader_Create(Shader* out, const char* name)
{
	char vertexShaderName[256];
	sprintf_s(vertexShaderName, 256, "assets/shaders/cache/%s_v.cso", name);
	char pixelShaderName[256];
	sprintf_s(pixelShaderName, 256, "assets/shaders/cache/%s_v.cso", name);

	BV_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(vertexShaderName), &out->VertexShaderBlob));
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateVertexShader(out->VertexShaderBlob->GetBufferPointer(),
		out->VertexShaderBlob->GetBufferSize(), nullptr, &out->VertexShader));

	ID3DBlob* blob;
	BV_CHECK_DX_RESULT(D3DReadFileToBlob(CA2T(pixelShaderName), &blob));
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreatePixelShader(blob->GetBufferPointer(),
		blob->GetBufferSize(), nullptr, &out->PixelShader));
}

void Shader_Bind(Shader* out)
{
	s_RendererState.DeviceContext->VSSetShader(out->VertexShader, nullptr, 0);
	s_RendererState.DeviceContext->PSSetShader(out->PixelShader, nullptr, 0);
}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Pipeline---------------------
//-----------------------------------------
static DXGI_FORMAT ShaderDataTypeToDX11BaseType(ShaderDataType type)
{
	switch (type)
	{
	case ShaderDataType::Float:    return DXGI_FORMAT_R32_FLOAT;
	case ShaderDataType::Float2:   return DXGI_FORMAT_R32G32_FLOAT;
	case ShaderDataType::Float3:   return DXGI_FORMAT_R32G32B32_FLOAT;
	case ShaderDataType::Float4:   return DXGI_FORMAT_R32G32B32A32_FLOAT;
	case ShaderDataType::Int:      return DXGI_FORMAT_R32_SINT;
	case ShaderDataType::Int2:     return DXGI_FORMAT_R32G32_SINT;
	case ShaderDataType::Int3:     return DXGI_FORMAT_R32G32B32_SINT;
	case ShaderDataType::Int4:     return DXGI_FORMAT_R32G32B32A32_SINT;
	}
	return DXGI_FORMAT_UNKNOWN;
}

void Pipeline_Create(Pipeline* out, Shader* shader, VertexBufferLayout* layout)
{
	out->Layout = layout;
	out->Shader = shader;

	//D3D11_INPUT_ELEMENT_DESC temp;
	//for (const auto& element : layout)
	//{
	//	temp.push_back(D3D11_INPUT_ELEMENT_DESC{
	//		element.Name.c_str(),0,ShaderDataTypeToDX11BaseType(element.Type),
	//		0,(UINT)element.Offset ,D3D11_INPUT_PER_VERTEX_DATA ,0 });
	//}

	//BV_CHECK_DX_RESULT(s_RendererState.Device->CreateInputLayout(
	//	&temp[0], (UINT)temp.size(), shader->VertexShaderBlob->GetVertextBufferPointer(),
	//	shader->VertexShaderBlob->GetVertextBufferSize(), out->InputLayout));
}

void Pipeline_Bind(Pipeline* out)
{}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Constant Buffer--------------
//-----------------------------------------

void ConstantBuffer_Create(ConstantBuffer* out, uint32_t size, uint32_t bindSlot)
{
	out->Size = size;
	out->BindSlot = bindSlot;

	D3D11_BUFFER_DESC buffer = {};
	buffer.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffer.Usage = D3D11_USAGE_DYNAMIC;
	buffer.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffer.MiscFlags = 0;
	buffer.ByteWidth = size;
	buffer.StructureByteStride = 0;
	BV_CHECK_DX_RESULT(s_RendererState.Device->CreateBuffer(&buffer, nullptr, &out->Buffer));
}

void ConstantBuffer_SetData(ConstantBuffer* out, void* data)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	BV_CHECK_DX_RESULT(s_RendererState.DeviceContext->Map(out->Buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy(mappedResource.pData, data, out->Size);
	s_RendererState.DeviceContext->Unmap(out->Buffer, 0);
}

void ConstantBuffer_Bind(ConstantBuffer* out)
{
	s_RendererState.DeviceContext->VSSetConstantBuffers(out->BindSlot, 1, &out->Buffer);
	s_RendererState.DeviceContext->PSSetConstantBuffers(out->BindSlot, 1, &out->Buffer);
}

//-----------------------------------------
//-----------------------------------------
//-----------------------------------------

//-----------------------------------------
//------------Texture----------------------
//-----------------------------------------

void Texture2D_Create(Texture2D* out, const char* path)
{

}

void Texture2D_Bind(Texture2D* out, uint32_t slot)
{}