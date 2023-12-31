﻿#include "pch.h"
#include "Components.h"
#include "Scene.h"

#include <Audio.h>

Mat TransformComponent_GetTransform(const TransformComponent& transform)
{
	return DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z)
		* TransformComponent_GetRotation(transform)
		* DirectX::XMMatrixTranslation(transform.Translation.x, transform.Translation.y, transform.Translation.z);
}

Mat TransformComponent_GetRotation(const TransformComponent& transform)
{
	Mat xRotation = DirectX::XMMatrixRotationX(transform.Rotation.x);
	Mat yRotation = DirectX::XMMatrixRotationY(transform.Rotation.y);
	Mat zRotation = DirectX::XMMatrixRotationZ(transform.Rotation.z);

	// TODO: Why quaternion rotation has gimbal lock problem?
	//return DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&transform.Rotation)));
	return xRotation * yRotation * zRotation;
}

// Right handed coordinate system, -z is forward, +y is up, +x is right
Vec3 TransformComponent_GetUp(const TransformComponent& transform)
{
	Vec3 up = { 0.0f, 1.0f, 0.0f };
	Mat rotation = TransformComponent_GetRotation(transform);
	return Vec3MulMat(up, rotation);
}

Vec3 TransformComponent_GetForward(const TransformComponent& transform)
{
	Vec3 forward = { 0.0f, 0.0f, -1.0f };
	Mat rotation = TransformComponent_GetRotation(transform);
	return Vec3MulMat(forward, rotation);
}

Vec3 TransformComponent_GetRight(const TransformComponent& transform)
{
	Vec3 right = { 1.0f, 0.0f, 0.0f };
	Mat rotation = TransformComponent_GetRotation(transform);
	return Vec3MulMat(right, rotation);
}

void RectTransformComponent_GetPositionAndSize(const RectTransformComponent& rectTransform, const Vec2& currentViewPortSize,
	Vec2* ndcPos, Vec2* pos, Vec2* size)
{
	const static Vec2 standardScreenSize = { 1920.0f,1080.0f };
	Vec2 scale = { currentViewPortSize.x / standardScreenSize.x,currentViewPortSize.y / standardScreenSize.y };

	pos->x = rectTransform.Position.x / standardScreenSize.x;
	pos->y = rectTransform.Position.y / standardScreenSize.y;

	// From [0,1] to ndc [-1,1]
	ndcPos->x = pos->x * 2.0f - 1.0f;
	ndcPos->y = pos->y * 2.0f - 1.0f;
	size->x = (rectTransform.Size.x / standardScreenSize.x) * rectTransform.Scale.x * scale.x * 2.0f;
	size->y = (rectTransform.Size.y / standardScreenSize.y) * rectTransform.Scale.y * scale.y * 2.0f;
}

void Rigidbody2DComponent_ApplyForce(Rigidbody2DComponent* rigidbody2D, const Vec2& force)
{
	if (Vec2Equal(force, Vec2Zero))
		return;
	Rigidbody2D_ApplyForce(rigidbody2D->RuntimeBody, force);
}

void Rigidbody2DComponent_ApplyRotation(Rigidbody2DComponent* rigidbody2D, float rotation)
{
	if (rotation != 0.0f)
		Rigidbody2D_ApplyRotation(rigidbody2D->RuntimeBody, rotation);
}

void Rigidbody2DComponent_SetPosition(Rigidbody2DComponent* rigidbody2D, const Vec2& position)
{
	if (Vec2Equal(position, Vec2Zero))
		return;

	Rigidbody2D_SetPosition(rigidbody2D->RuntimeBody, position);
}

void Rigidbody2DComponent_MovePosition(Rigidbody2DComponent* rigidbody2D, const Vec2& position)
{
	if (Vec2Equal(position, Vec2Zero))
		return;

	Rigidbody2D_MovePosition(rigidbody2D->RuntimeBody, position);
}

void Rigidbody2DComponent_SetRotation(Rigidbody2DComponent* rigidbody2D, float rotation)
{
	if (rotation == 0.0f)
		return;

	Rigidbody2D_SetRotation(rigidbody2D->RuntimeBody, rotation);
}

float Rigidbody2DComponent_GetRotation(Rigidbody2DComponent* rigidbody2D)
{
	return Rigidbody2D_GetRotation(rigidbody2D->RuntimeBody);
}

void Rigidbody2DCmponent_Rotate(Rigidbody2DComponent* rigidbody2D, float amount)
{
	if (amount == 0.0f)
		return;

	Rigidbody2D_Rotate(rigidbody2D->RuntimeBody, amount);
}

void AudioComponent_Play(const Scene* scene, AudioComponent* audioComponent, void* soundEffect)
{
	DirectX::SoundEffect* se = (DirectX::SoundEffect*)soundEffect;

	if (audioComponent->Type == AudioComponentType_Listener)
	{
		if (!se->IsInUse())
			se->Play();
	}
	else
	{

		if (se->IsInUse())
			return;

		se->Play();

		//TODO: Make 3D sound work
		//DirectX::AudioListener* listener = (DirectX::AudioListener*)Scene_GetListener(scene);
		//DirectX::AudioEmitter* emitter = (DirectX::AudioEmitter*)audioComponent->Audio;

		//Vec3 listenerPos = listener->Position;
		//Vec3 emitterPos = emitter->Position;

		//float distance = Vec3Distance(listenerPos, emitterPos);

		//float volume = 1.0f - (distance / 10.0f);
		//if (volume < 0.0f)
		//	volume = 0.0f;

		//auto effectInstance = se->CreateInstance(DirectX::SoundEffectInstance_Use3D);
		//effectInstance->SetVolume(volume);
		//effectInstance->Play(true);

		//auto effectInstance = se->CreateInstance(DirectX::SoundEffectInstance_Use3D
		//	| DirectX::SoundEffectInstance_ReverbUseFilters);
		//DirectX::AudioListener* listener = (DirectX::AudioListener*)Scene_GetListener(scene);
		//DirectX::AudioEmitter* emitter = (DirectX::AudioEmitter*)audioComponent->Audio;
		//effectInstance->Play(false);
		//effectInstance->Apply3D(*listener, *emitter, false);
	}
}