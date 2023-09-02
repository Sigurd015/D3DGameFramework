#include "pch.h"
#include "Components.h"
#include "Scene.h"

#include <Audio.h>

Mat TransformComponent_GetTransform(const TransformComponent& transform)
{
	return DirectX::XMMatrixScaling(transform.Scale.x, transform.Scale.y, transform.Scale.z)
		* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&transform.Rotation)))
		* DirectX::XMMatrixTranslation(transform.Translation.x, transform.Translation.y, transform.Translation.z);
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

void AudioComponent_Play(const Scene* scene, AudioComponent* audioComponent, void* soundEffect)
{
	// Sorry about using std::unique_ptr here, but for DirectXTK audio, it's only return std::unique_ptr
	// RefPtr(Created by Me) is trash compared to std::unique_ptr OvO
	DirectX::SoundEffect* se = (DirectX::SoundEffect*)soundEffect;

	if (audioComponent->Type == AudioComponentType_Listener)
	{
		if (!se->IsInUse())
			se->Play();
	}
	else
	{
		//TODO: Make 3D sound work
		if (!se->IsInUse())
			se->Play();
		//auto effectInstance = se->CreateInstance(DirectX::SoundEffectInstance_Use3D | DirectX::SoundEffectInstance_ReverbUseFilters);
		//DirectX::AudioListener* listener = (DirectX::AudioListener*)Scene_GetListener(scene);
		//DirectX::AudioEmitter* emitter = (DirectX::AudioEmitter*)audioComponent->Audio;
		//effectInstance->Apply3D(*listener, *emitter);
		//effectInstance->Play();
	}
}