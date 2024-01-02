#include "pch.h"
#include "Entity.h"

static const uint32_t s_ComponentSizes[ComponentType_Count] =
{
	sizeof(RectTransformComponent),
	sizeof(CameraComponent),
	sizeof(SpriteRendererComponent),
	sizeof(CircleRendererComponent),
	sizeof(Rigidbody2DComponent),
	sizeof(BoxCollider2DComponent),
	sizeof(CircleCollider2DComponent),
	sizeof(TextComponent),
	sizeof(ScriptComponent),
	sizeof(AudioComponent),
	sizeof(MeshComponent),
	sizeof(SkyLightComponent),
	sizeof(LightComponent),
};

bool Entity_HasComponent(Entity* entity, ComponentType type)
{
	if (type == ComponentType_Tag || type == ComponentType_Transform)
		return true;

	return entity->Components[type] != nullptr;
}

void* Entity_GetComponent(Entity* entity, ComponentType type)
{
	if (type == ComponentType_Tag)
		return &entity->Tag;

	if (type == ComponentType_Transform)
		return &entity->Transform;

	return entity->Components[type];
}

void Entity_AddComponent(Entity* entity, ComponentType type, void* component)
{
	switch (type)
	{
	case ComponentType_Tag:
	{
		TagComponent* tag = (TagComponent*)component;
		//entity->Tag.Name = String_Duplicate(tag->Name);
		entity->Tag.Name = tag->Name;
		return;
	}
	case ComponentType_Transform:
	{
		TransformComponent* transform = (TransformComponent*)component;
		entity->Transform = {
			transform->Translation,
			transform->Rotation,
			transform->Scale
		};
		return;
	}
	}

	CORE_ASSERT(!Entity_HasComponent(entity, type), "Entity already has component!");

	entity->Components[type] = Memory_Allocate(s_ComponentSizes[type], MemoryBlockTag_Component);
	Memory_Copy(entity->Components[type], component, s_ComponentSizes[type]);
}

void Entity_RemoveComponent(Entity* entity, ComponentType type)
{
	if (type == ComponentType_Transform)
		return;

	if (type == ComponentType_Tag)
	{
		//String_Free(entity->Tag.Name);
		return;
	}

	CORE_ASSERT(Entity_HasComponent(entity, type), "Entity does not have component!");

	switch (type)
	{
	case ComponentType_Script:
	{
		ScriptComponent* script = (ScriptComponent*)entity->Components[type];

		if (script->RuntimeData != nullptr)
		{
			Memory_Free(script->RuntimeData, script->RuntimeDataSize, MemoryBlockTag_Component);
			script->RuntimeData = nullptr;
		}
		break;
	}
	case ComponentType_Audio:
	{
		AudioComponent* audio = (AudioComponent*)entity->Components[type];
		if (audio->RuntimeAudio != nullptr)
		{
			delete audio->RuntimeAudio;
			audio->RuntimeAudio = nullptr;
		}
		break;
	}
	}

	Memory_Free(entity->Components[type], s_ComponentSizes[type], MemoryBlockTag_Component);
	entity->Components[type] = nullptr;
}