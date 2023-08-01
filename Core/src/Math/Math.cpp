#include "pch.h"
#include "Math.h"

Vec3 Vec4ToVec3(const Vec4& vec)
{
	return { vec.x,vec.y,vec.z };
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

float Vec2Dot(const Vec2& a, const Vec2& b)
{
	DirectX::XMVECTOR dotProduct = DirectX::XMVector2Dot(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b));
	float result;
	DirectX::XMStoreFloat(&result, dotProduct);
	return result;
}

float Vec2Cross(const Vec2& a, const Vec2& b)
{
	DirectX::XMVECTOR crossProduct = DirectX::XMVector2Cross(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b));
	float result;
	DirectX::XMStoreFloat(&result, crossProduct);
	return result;
}

bool Vec2Equal(const Vec2& a, const Vec2& b)
{
	return a.x == b.x && a.y == b.y;
}

Vec2 Vec2MulFloat(const Vec2& vec, float f)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorScale(DirectX::XMLoadFloat2(&vec), f));
	return result;
}

Vec2 Vec2MulMat(const Vec2& vec, const Mat& mat)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVector2Transform(DirectX::XMLoadFloat2(&vec), mat));
	return result;
}

Vec2 Vec2MulVec2(const Vec2& a, const Vec2& b)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorMultiply(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b)));
	return result;
}

Vec2 Vec2DivFloat(const Vec2& vec, float f)
{
	if (f != 0.0f)
	{
		Vec2 result;
		DirectX::XMStoreFloat2(&result, DirectX::XMVectorScale(DirectX::XMLoadFloat2(&vec), 1.0f / f));
		return result;
	}
	return Vec2Zero;
}

Vec2 Vec2Normalize(const Vec2& vec)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVector2Normalize(DirectX::XMLoadFloat2(&vec)));
	return result;
}

float Vec2LengthSq(const Vec2& vec)
{
	float result;
	DirectX::XMStoreFloat(&result, DirectX::XMVector2LengthSq(DirectX::XMLoadFloat2(&vec)));
	return result;
}

float Vec2Length(const Vec2& vec)
{
	float result;
	DirectX::XMStoreFloat(&result, DirectX::XMVector2Length(DirectX::XMLoadFloat2(&vec)));
	return result;
}

float Vec2Distance(const Vec2& a, const Vec2& b)
{
	return Vec2Length({ a.x - b.x, a.y - b.y });
}

float Vec2DistanceSq(const Vec2& a, const Vec2& b)
{
	return Vec2LengthSq({ a.x - b.x, a.y - b.y });
}

Vec2 Vec2Add(const Vec2& a, const Vec2& b)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b)));
	return result;
}

Vec2 Vec2Sub(const Vec2& a, const Vec2& b)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b)));
	return result;
}

Vec2 Vec2Lerp(const Vec2& a, const Vec2& b, float t)
{
	Vec2 result;
	DirectX::XMStoreFloat2(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b), t));
	return result;
}

bool Vec2NearlyEqual(const Vec2& a, const Vec2& b, float smallAmount)
{
	return Vec2DistanceSq(a, b) < smallAmount * smallAmount;
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

Vec3 Vec3Lerp(const Vec3& a, const Vec3& b, float t)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b), t));
	return result;
}

Vec2 Vec3ToVec2(const Vec3& vec)
{
	return { vec.x,vec.y };
}

float FloatMax(float a, float b)
{
	if (a > b)
		return a;
	else
		return b;
}

float FloatMin(float a, float b)
{
	if (a < b)
		return a;
	else
		return b;
}

float FloatClamp(float value, float min, float max)
{
	if (value < min)
		return min;
	else if (value > max)
		return max;
	else
		return value;
}

float FloatLerp(float a, float b, float t)
{
	if (t >= 0.0f && t <= 1.0f)
		return a + t * (b - a);
	else
		return 0.0f;
}

bool FloatNearlyEqual(float a, float b, float smallAmount)
{
	return fabs(a - b) < smallAmount;
}

const char* ToString(const Vec2& vec)
{
	char tempChar[256];
	sprintf_s(tempChar, 256, "{x : %f, y : %f}", vec.x, vec.y);
	return tempChar;
}

const char* ToString(const Vec3& vec)
{
	char tempChar[256];
	sprintf_s(tempChar, 256, "{x : %f, y : %f z : %f}", vec.x, vec.y, vec.z);
	return tempChar;
}

const char* ToString(const Vec4& vec)
{
	char tempChar[256];
	sprintf_s(tempChar, 256, "{x : %f, y : %f z : %f w : %f}", vec.x, vec.y, vec.z, vec.w);
	return tempChar;
}

const char* ToString(float f)
{
	char tempChar[256];
	sprintf_s(tempChar, 256, "{number : %f}", f);
	return tempChar;
}