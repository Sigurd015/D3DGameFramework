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

	void* Components[ComponentType_Count] = { nullptr };
};

bool Entity_HasComponent(Entity* entity, ComponentType type);
void* Entity_GetComponent(Entity* entity, ComponentType type);
void Entity_AddComponent(Entity* entity, ComponentType type, void* component);
void Entity_RemoveComponent(Entity* entity, ComponentType type);