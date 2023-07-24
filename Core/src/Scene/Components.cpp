#include "pch.h"
#include "Components.h"

Mat TransformComponent_GetTransform(const TransformComponent& transform)
{
	return DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z)
		* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&transform.Rotation)))
		* DirectX::XMMatrixTranslation(transform.Translation.x, transform.Translation.y, transform.Translation.z);
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