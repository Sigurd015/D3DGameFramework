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
		entity->RectTransform = (RectTransformComponent*)malloc(sizeof(RectTransformComponent));
		memcpy(entity->RectTransform, component, sizeof(RectTransformComponent));
		break;
	}
	case ComponentType_Camera:
	{
		CORE_ASSERT(!entity->Camera, "Entity already has component!");
		entity->Camera = (CameraComponent*)malloc(sizeof(CameraComponent));
		memcpy(entity->Camera, component, sizeof(CameraComponent));
		break;
	}
	case ComponentType_SpriteRenderer:
	{
		CORE_ASSERT(!entity->SpriteRenderer, "Entity already has component!");
		entity->SpriteRenderer = (SpriteRendererComponent*)malloc(sizeof(SpriteRendererComponent));
		memcpy(entity->SpriteRenderer, component, sizeof(SpriteRendererComponent));
		break;
	}
	case ComponentType_CircleRenderer:
	{
		CORE_ASSERT(!entity->CircleRenderer, "Entity already has component!");
		entity->CircleRenderer = (CircleRendererComponent*)malloc(sizeof(CircleRendererComponent));
		memcpy(entity->CircleRenderer, component, sizeof(CircleRendererComponent));
		break;
	}
	case ComponentType_Rigidbody2D:
	{
		CORE_ASSERT(!entity->Rigidbody2D, "Entity already has component!");
		entity->Rigidbody2D = (Rigidbody2DComponent*)malloc(sizeof(Rigidbody2DComponent));
		memcpy(entity->Rigidbody2D, component, sizeof(Rigidbody2DComponent));
		break;
	}
	case ComponentType_BoxCollider2D:
	{
		CORE_ASSERT(!entity->BoxCollider2D, "Entity already has component!");
		entity->BoxCollider2D = (BoxCollider2DComponent*)malloc(sizeof(BoxCollider2DComponent));
		memcpy(entity->BoxCollider2D, component, sizeof(BoxCollider2DComponent));
		break;
	}
	case ComponentType_CircleCollider2D:
	{
		CORE_ASSERT(!entity->CircleCollider2D, "Entity already has component!");
		entity->CircleCollider2D = (CircleCollider2DComponent*)malloc(sizeof(CircleCollider2DComponent));
		memcpy(entity->CircleCollider2D, component, sizeof(CircleCollider2DComponent));
		break;
	}
	case ComponentType_Text:
	{
		CORE_ASSERT(!entity->Text, "Entity already has component!");
		entity->Text = (TextComponent*)malloc(sizeof(TextComponent));
		memcpy(entity->Text, component, sizeof(TextComponent));
		break;
	}
	case ComponentType_Script:
	{
		CORE_ASSERT(!entity->Script, "Entity already has component!");
		entity->Script = (ScriptComponent*)malloc(sizeof(ScriptComponent));
		memcpy(entity->Script, component, sizeof(ScriptComponent));
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
		free(entity->RectTransform);
		entity->RectTransform = nullptr;
		return;
	case ComponentType_Camera:
		free(entity->Camera);
		entity->Camera = nullptr;
		return;
	case ComponentType_SpriteRenderer:
		if (entity->SpriteRenderer->Texture)
		{
			RefPtr_Release(entity->SpriteRenderer->Texture, [](void* ptr)
				{
					Texture2D_Release((Texture2D*)ptr);
				});
		}
		free(entity->SpriteRenderer);
		entity->SpriteRenderer = nullptr;
		return;
	case ComponentType_CircleRenderer:
		free(entity->CircleRenderer);
		entity->CircleRenderer = nullptr;
		return;
	case ComponentType_Rigidbody2D:
		free(entity->Rigidbody2D);
		entity->Rigidbody2D = nullptr;
		return;
	case ComponentType_BoxCollider2D:
		free(entity->BoxCollider2D);
		entity->BoxCollider2D = nullptr;
		return;
	case ComponentType_CircleCollider2D:
		free(entity->CircleCollider2D);
		entity->CircleCollider2D = nullptr;
		return;
	case ComponentType_Text:
		free(entity->Text);
		entity->Text = nullptr;
		return;
	case ComponentType_Script:
		if (entity->Script->RuntimeData != nullptr)
		{
			free(entity->Script->RuntimeData);
			entity->Script->RuntimeData = nullptr;
		}
		free(entity->Script);
		entity->Script = nullptr;
		return;
	}
	CORE_ASSERT(false, "Unknown ComponentType!");
}