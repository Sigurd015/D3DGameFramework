#include "pch.h"
#include "Entity.h"

bool Entity_HasComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:		          return true;
	case ComponentType_Transform:	      return true;
	case ComponentType_RectTransform:	  return entity->RectTransform != nullptr;
	case ComponentType_Camera:	          return entity->Camera != nullptr;
	case ComponentType_SpriteRenderer:	  return entity->SpriteRenderer != nullptr;
	case ComponentType_CircleRenderer:	  return entity->CircleRenderer != nullptr;
	case ComponentType_Rigidbody2D:	      return entity->Rigidbody2D != nullptr;
	case ComponentType_BoxCollider2D:	  return entity->BoxCollider2D != nullptr;
	case ComponentType_CircleCollider2D:  return entity->CircleCollider2D != nullptr;
	case ComponentType_Text:              return entity->Text != nullptr;
	case ComponentType_Script:            return entity->Script != nullptr;
	case ComponentType_Audio:             return entity->Audio != nullptr;
	}
	CORE_ASSERT(false, "Unknown ComponentType!");
	return false;
}

void* Entity_GetComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:		          return &entity->Tag;
	case ComponentType_Transform:	      return &entity->Transform;
	case ComponentType_RectTransform:	  return entity->RectTransform;
	case ComponentType_Camera:	          return entity->Camera;
	case ComponentType_SpriteRenderer:	  return entity->SpriteRenderer;
	case ComponentType_CircleRenderer:	  return entity->CircleRenderer;
	case ComponentType_Rigidbody2D:	      return entity->Rigidbody2D;
	case ComponentType_BoxCollider2D:	  return entity->BoxCollider2D;
	case ComponentType_CircleCollider2D:  return entity->CircleCollider2D;
	case ComponentType_Text:              return entity->Text;
	case ComponentType_Script:            return entity->Script;
	case ComponentType_Audio:             return entity->Audio;
	}
	CORE_ASSERT(false, "Unknown ComponentType!");
	return nullptr;
}

void Entity_AddComponent(Entity* entity, ComponentType type, void* component)
{
	switch (type)
	{
	case ComponentType_Tag:
	{
		TagComponent* tag = (TagComponent*)component;
		entity->Tag.Name = tag->Name;
		break;
	}
	case ComponentType_Transform:
	{
		TransformComponent* transform = (TransformComponent*)component;
		entity->Transform = {
			transform->Translation,
			transform->Rotation,
			transform->Scale
		};
		break;
	}
	case ComponentType_RectTransform:
	{
		CORE_ASSERT(!entity->RectTransform, "Entity already has component!");
		entity->RectTransform = (RectTransformComponent*)Memory_Allocate(sizeof(RectTransformComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->RectTransform, component, sizeof(RectTransformComponent));
		break;
	}
	case ComponentType_Camera:
	{
		CORE_ASSERT(!entity->Camera, "Entity already has component!");
		entity->Camera = (CameraComponent*)Memory_Allocate(sizeof(CameraComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->Camera, component, sizeof(CameraComponent));
		break;
	}
	case ComponentType_SpriteRenderer:
	{
		CORE_ASSERT(!entity->SpriteRenderer, "Entity already has component!");
		entity->SpriteRenderer = (SpriteRendererComponent*)Memory_Allocate(sizeof(SpriteRendererComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->SpriteRenderer, component, sizeof(SpriteRendererComponent));
		break;
	}
	case ComponentType_CircleRenderer:
	{
		CORE_ASSERT(!entity->CircleRenderer, "Entity already has component!");
		entity->CircleRenderer = (CircleRendererComponent*)Memory_Allocate(sizeof(CircleRendererComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->CircleRenderer, component, sizeof(CircleRendererComponent));
		break;
	}
	case ComponentType_Rigidbody2D:
	{
		CORE_ASSERT(!entity->Rigidbody2D, "Entity already has component!");
		entity->Rigidbody2D = (Rigidbody2DComponent*)Memory_Allocate(sizeof(Rigidbody2DComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->Rigidbody2D, component, sizeof(Rigidbody2DComponent));
		break;
	}
	case ComponentType_BoxCollider2D:
	{
		CORE_ASSERT(!entity->BoxCollider2D, "Entity already has component!");
		entity->BoxCollider2D = (BoxCollider2DComponent*)Memory_Allocate(sizeof(BoxCollider2DComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->BoxCollider2D, component, sizeof(BoxCollider2DComponent));
		break;
	}
	case ComponentType_CircleCollider2D:
	{
		CORE_ASSERT(!entity->CircleCollider2D, "Entity already has component!");
		entity->CircleCollider2D = (CircleCollider2DComponent*)Memory_Allocate(sizeof(CircleCollider2DComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->CircleCollider2D, component, sizeof(CircleCollider2DComponent));
		break;
	}
	case ComponentType_Text:
	{
		CORE_ASSERT(!entity->Text, "Entity already has component!");
		entity->Text = (TextComponent*)Memory_Allocate(sizeof(TextComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->Text, component, sizeof(TextComponent));
		break;
	}
	case ComponentType_Script:
	{
		CORE_ASSERT(!entity->Script, "Entity already has component!");
		entity->Script = (ScriptComponent*)Memory_Allocate(sizeof(ScriptComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->Script, component, sizeof(ScriptComponent));
		break;
	}
	case ComponentType_Audio:
	{
		CORE_ASSERT(!entity->Script, "Entity already has component!");
		entity->Audio = (AudioComponent*)Memory_Allocate(sizeof(AudioComponent), MemoryBlockTag_Component);
		Memory_Copy(entity->Audio, component, sizeof(AudioComponent));
		break;
	}
	}
}

void Entity_RemoveComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:
		return;
	case ComponentType_Transform:
		return;
	case ComponentType_RectTransform:
		Memory_Free(entity->RectTransform, sizeof(RectTransformComponent), MemoryBlockTag_Component);
		entity->RectTransform = nullptr;
		return;
	case ComponentType_Camera:
		Memory_Free(entity->Camera, sizeof(CameraComponent), MemoryBlockTag_Component);
		entity->Camera = nullptr;
		return;
	case ComponentType_SpriteRenderer:
		Memory_Free(entity->SpriteRenderer, sizeof(SpriteRendererComponent), MemoryBlockTag_Component);
		entity->SpriteRenderer = nullptr;
		return;
	case ComponentType_CircleRenderer:
		Memory_Free(entity->CircleRenderer, sizeof(CircleRendererComponent), MemoryBlockTag_Component);
		entity->CircleRenderer = nullptr;
		return;
	case ComponentType_Rigidbody2D:
		Memory_Free(entity->Rigidbody2D, sizeof(Rigidbody2DComponent), MemoryBlockTag_Component);
		entity->Rigidbody2D = nullptr;
		return;
	case ComponentType_BoxCollider2D:
		Memory_Free(entity->BoxCollider2D, sizeof(BoxCollider2DComponent), MemoryBlockTag_Component);
		entity->BoxCollider2D = nullptr;
		return;
	case ComponentType_CircleCollider2D:
		Memory_Free(entity->CircleCollider2D, sizeof(CircleCollider2DComponent), MemoryBlockTag_Component);
		entity->CircleCollider2D = nullptr;
		return;
	case ComponentType_Text:
		Memory_Free(entity->Text, sizeof(TextComponent), MemoryBlockTag_Component);
		entity->Text = nullptr;
		return;
	case ComponentType_Script:
		if (entity->Script->RuntimeData != nullptr)
		{
			Memory_Free(entity->Script->RuntimeData, entity->Script->RuntimeDataSize, MemoryBlockTag_Component);
			entity->Script->RuntimeData = nullptr;
		}
		Memory_Free(entity->Script, sizeof(ScriptComponent), MemoryBlockTag_Component);
		entity->Script = nullptr;
		return;
	case ComponentType_Audio:
		if (entity->Audio->RuntimeAudio != nullptr)
		{
			delete entity->Audio->RuntimeAudio;
			entity->Audio->RuntimeAudio = nullptr;
		}
		Memory_Free(entity->Audio, sizeof(AudioComponent), MemoryBlockTag_Component);
		entity->Audio = nullptr;
		return;
	}
	CORE_ASSERT(false, "Unknown ComponentType!");
}