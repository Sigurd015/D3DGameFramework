#include "pch.h"
#include "Math.h"

Vec3 Vec4ToVec3(const Vec4& vec4)
{
	Vec3 temp = { vec4.x,vec4.y,vec4.z };
	return temp;
}

Vec4 Vec4MulFloat(const Vec4& vec, float f)
{
	Vec4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVectorScale(DirectX::XMLoadFloat4(&vec), f));
	return result;
}

Vec4 Vec4MulMat(const Vec4& vec, const Mat& mat)
{
	Vec4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVector4Transform(DirectX::XMLoadFloat4(&vec), mat));
	return result;
}