#include "pch.h"
#include "DXTrace.h"

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