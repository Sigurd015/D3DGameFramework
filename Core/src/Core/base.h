#pragma once

#ifdef BV_DEBUG
#define BV_ASSERT(x,m) {if(!x){__debugbreak();CORE_LOG_ERROR(m);}}
#else
#define BV_ASSERT(x,m) x
#endif

#include <DirectXMath.h>

typedef DirectX::XMVECTOR Vec;
typedef DirectX::XMFLOAT2 Vec2;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMFLOAT4X4 Mat4;