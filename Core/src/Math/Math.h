#pragma once

#include <DirectXMath.h>

typedef DirectX::XMVECTOR Vec;
typedef DirectX::XMFLOAT2 Vec2;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMFLOAT4X4 Mat4;

Vec3 Vec4ToVec3(const Vec4& vec);

Vec4 Vec4MulFloat(const Vec4& vec, float f);
Vec4 Vec4MulMat(const Vec4& vec, const Mat& mat);

Vec2 Vec2MulFloat(const Vec2& vec, float f);
float Vec2LengthSq(const Vec2& vec);

Vec3 Vec3Add(const Vec3& a, const Vec3& b);
Vec3 Vec3MulFloat(const Vec3& vec, float f);
Vec3 Vec3MulVec3(const Vec3& a, const Vec3& b);
Vec3 Vec3Lerp(const Vec3& a, const Vec3& b, float t);

float FloatMax(float a, float b);
float FloatMin(float a, float b);
float FloatClamp(float value, float min, float max);
float FloatLerp(float a, float b, float t);