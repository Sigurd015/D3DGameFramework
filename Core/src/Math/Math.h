#pragma once

#include <DirectXMath.h>

typedef DirectX::XMVECTOR Vec;
typedef DirectX::XMFLOAT2 Vec2;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMFLOAT4X4 Mat4;

Vec3 Vec4ToVec3(const Vec4& vec4);

Vec4 Vec4MulFloat(const Vec4& vec, float f);
Vec4 Vec4MulMat(const Vec4& vec, const Mat& mat);