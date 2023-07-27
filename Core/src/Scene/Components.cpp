#include "pch.h"
#include "Components.h"

Mat TransformComponent_GetTransform(const TransformComponent& transform)
{
	return DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z)
		* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&transform.Rotation)))
		* DirectX::XMMatrixTranslation(transform.Translation.x, transform.Translation.y, transform.Translation.z);
}

void RectTransformComponent_GetPositionAndSize(const RectTransformComponent& rectTransform, Vec2* pos, Vec2* size)
{
	// From [0,1] to ndc [-1,1]
	pos->x = rectTransform.Position.x / rectTransform.StandardScreenSize.x * 2.0f - 1.0f;
	pos->y = rectTransform.Position.y / rectTransform.StandardScreenSize.y * 2.0f - 1.0f;
	size->x = rectTransform.Size.x / rectTransform.StandardScreenSize.x * rectTransform.Scale.x;
	size->y = rectTransform.Size.y / rectTransform.StandardScreenSize.y * rectTransform.Scale.y;
}

void Rigidbody2DComponent_ApplyForce(Rigidbody2DComponent& rigidbody2D, const Vec2& force)
{
	if (Vec2Equal(force, Vec2Zero))
		return;
	Rigidbody2D_ApplyForce(rigidbody2D.RuntimeBody, force);
}

void Rigidbody2DComponent_ApplyRotation(Rigidbody2DComponent& rigidbody2D, float rotation)
{
	if (rotation != 0.0f)
		Rigidbody2D_ApplyRotation(rigidbody2D.RuntimeBody, rotation);
}