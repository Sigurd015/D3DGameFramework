#pragma once

#include <Windows.h>

void CheckD3DError(HRESULT result);

#ifdef BV_DEBUG
#define BV_CHECK_DX_RESULT(x) { HRESULT __hr = x;CheckD3DError(__hr);}
#else
#define BV_CHECK_DX_RESULT(x) x
#endif