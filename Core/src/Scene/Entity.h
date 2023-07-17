#pragma once
#include "Components.h"

struct Entity
{
	uint32_t ID;

	TagComponent Tag;
	TransformComponent Transform;
	SpriteRendererComponent* SpriteRenderer = nullptr;
	CameraComponent* Camera = nullptr;
	CircleRendererComponent* CircleRenderer = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;
	BoxCollider2DComponent* BoxCollider2D = nullptr;
	CircleCollider2DComponent* CircleCollider2D = nullptr;
	TextComponent* Text = nullptr;
	ScriptComponent* Script = nullptr;
};

bool Entity_HasComponent(Entity* entity, ComponentType type);
void* Entity_GetComponent(Entity* entity, ComponentType type);
void Entity_AddComponent(Entity* entity, ComponentType type, void* component);
void Entity_RemoveComponent(Entity* entity, ComponentType type);