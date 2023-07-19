#include "pch.h"
#include "Math.h"

Vec3 Vec4ToVec3(const Vec4& vec)
{
	Vec3 temp = { vec.x,vec.y,vec.z };
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

Vec2 Vec2MulFloat(const Vec2& vec, float f)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorScale(DirectX::XMLoadFloat2(&vec), f));
	return result;
}

Vec3 Vec3Add(const Vec3& a, const Vec3& b)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
	return result;
}

Vec3 Vec3MulFloat(const Vec3& vec, float f)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&vec), f));
	return result;
}

Vec3 Vec3MulVec3(const Vec3& a, const Vec3& b)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
	return result;
}
