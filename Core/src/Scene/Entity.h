#pragma once
#include "Components.h"

// Forward declarations
struct Scene;

struct Entity
{
	bool Enabled = true;
	Scene* Scene = nullptr;

	TagComponent Tag;
	TransformComponent Transform;
	RectTransformComponent* RectTransform = nullptr;
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