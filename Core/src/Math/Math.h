#pragma once

#include <DirectXMath.h>

typedef DirectX::XMVECTOR Vec;
typedef DirectX::XMFLOAT2 Vec2;
typedef DirectX::XMFLOAT3 Vec3;
typedef DirectX::XMFLOAT4 Vec4;
typedef DirectX::XMMATRIX Mat;
typedef DirectX::XMFLOAT4X4 Mat4;

#define Vec2Zero Vec2(0.0f,0.0f) 
#define Vec3Zero Vec3(0.0f,0.0f,0.0f)
#define Vec4Zero Vec4(0.0f,0.0f,0.0f,0.0f)

Vec3 Vec4ToVec3(const Vec4& vec);

Vec4 Vec4MulFloat(const Vec4& vec, float f);
Vec4 Vec4MulMat(const Vec4& vec, const Mat& mat);
Vec4 Vec4Lerp(const Vec4& a, const Vec4& b, float t);

Vec2 Vec2Rotate(const Vec2& vec, float angle);
Vec2 Vec2RotateByPivot(const Vec2& vec, const Vec2& pivot, float angle);
float Vec2Dot(const Vec2& a, const Vec2& b);
float Vec2Cross(const Vec2& a, const Vec2& b);
bool Vec2Equal(const Vec2& a, const Vec2& b);
Vec2 Vec2MulFloat(const Vec2& vec, float f);
Vec2 Vec2MulMat(const Vec2& vec, const Mat& mat);
Vec2 Vec2MulVec2(const Vec2& a, const Vec2& b);
Vec2 Vec2DivFloat(const Vec2& vec, float f);
Vec2 Vec2Normalize(const Vec2& vec);
float Vec2Length(const Vec2& vec);
float Vec2LengthSq(const Vec2& vec);
float Vec2Distance(const Vec2& a, const Vec2& b);
float Vec2DistanceSq(const Vec2& a, const Vec2& b);
Vec2 Vec2Add(const Vec2& a, const Vec2& b);
Vec2 Vec2Sub(const Vec2& a, const Vec2& b);
Vec2 Vec2Lerp(const Vec2& a, const Vec2& b, float t);
bool Vec2NearlyEqual(const Vec2& a, const Vec2& b, float smallAmount = 0.0005f);

Vec3 Vec3Rotate(const Vec3& vec, const Vec3& axis, float angle);
Vec3 Vec3Sub(const Vec3& a, const Vec3& b);
Vec3 Vec3Normalize(const Vec3& vec);
Vec3 Vec3Add(const Vec3& a, const Vec3& b);
Vec3 Vec3MulFloat(const Vec3& vec, float f);
Vec3 Vec3MulVec3(const Vec3& a, const Vec3& b);
Vec3 Vec3MulMat(const Vec3& vec, const Mat& mat);
Vec3 Vec3Lerp(const Vec3& a, const Vec3& b, float t);
Vec2 Vec3ToVec2(const Vec3& vec);
float Vec3Distance(const Vec3& a, const Vec3& b);
float Vec3Length(const Vec3& vec);

Mat Mat4ToMat3(const Mat& mat);

float FloatMax(float a, float b);
float FloatMin(float a, float b);
float FloatClamp(float value, float min, float max);
float FloatLerp(float a, float b, float t);
bool FloatNearlyEqual(float a, float b, float smallAmount = 0.0005f);