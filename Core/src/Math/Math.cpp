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

Vec4 Vec4Lerp(const Vec4& a, const Vec4& b, float t)
{
	Vec4 result;
	DirectX::XMStoreFloat4(&result, DirectX::XMVectorLerp(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b), t));
	return result;
}

Vec2 Vec2Rotate(const Vec2& vec, float angle)
{
	return Vec2MulMat(vec, DirectX::XMMatrixRotationZ(angle));
}

Vec2 Vec2RotateByPivot(const Vec2& vec, const Vec2& pivot, float angle)
{
	float cos = cosf(angle);
	float sin = sinf(angle);

	float x = vec.x - pivot.x;
	float y = vec.y - pivot.y;
	float targetX = x * cos - y * sin;
	float targetY = x * sin + y * cos;
	targetX += pivot.x;
	targetY += pivot.y;
	return Vec2(targetX, targetY);
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

Vec3 Vec3Sub(const Vec3& a, const Vec3& b)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
	return result;
}

Vec3 Vec3Normalize(const Vec3& vec)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&vec)));
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

Vec3 Vec3MulMat(const Vec3& vec, const Mat& mat)
{
	Vec3 result;
	DirectX::XMStoreFloat3(&result, DirectX::XMVector3Transform(DirectX::XMLoadFloat3(&vec), mat));
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

float Vec3Distance(const Vec3& a, const Vec3& b)
{
	return Vec3Length({ a.x - b.x, a.y - b.y, a.z - b.z });
}

float Vec3Length(const Vec3& vec)
{
	float result;
	DirectX::XMStoreFloat(&result, DirectX::XMVector3Length(DirectX::XMLoadFloat3(&vec)));
	return result;
}

Mat Mat4ToMat3(const Mat& mat)
{
	Mat mat3;

	// Extract the upper-left 3x3 submatrix from the original 4x4 matrix
	mat3.r[0] = mat.r[0];
	mat3.r[1] = mat.r[1];
	mat3.r[2] = mat.r[2];

	// Set the last row and column to identity values
	mat3.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	mat3.r[0].m128_f32[3] = 0.0f;
	mat3.r[1].m128_f32[3] = 0.0f;
	mat3.r[2].m128_f32[3] = 0.0f;

	return mat3;
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