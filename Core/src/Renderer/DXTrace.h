#pragma once

#include <Windows.h>

void CheckD3DError(HRESULT result);

#ifdef CORE_DEBUG
#define CORE_CHECK_DX_RESULT(x) { HRESULT __hr = x;CheckD3DError(__hr);}
#else
#define CORE_CHECK_DX_RESULT(x) x
#endif