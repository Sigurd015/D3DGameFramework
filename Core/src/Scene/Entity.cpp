#include "pch.h"
#include "Entity.h"

bool Entity_HasComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:		          return true;
	case ComponentType_Transform:	      return true;
	case ComponentType_Camera:	          return entity->Camera != nullptr;
	case ComponentType_SpriteRenderer:	  return entity->SpriteRenderer != nullptr;
	case ComponentType_CircleRenderer:	  return entity->CircleRenderer != nullptr;
	case ComponentType_Rigidbody2D:	      return entity->Rigidbody2D != nullptr;
	case ComponentType_BoxCollider2D:	  return entity->BoxCollider2D != nullptr;
	case ComponentType_CircleCollider2D:  return entity->CircleCollider2D != nullptr;
	case ComponentType_Text:              return entity->Text != nullptr;
	case ComponentType_Script:            return entity->Script != nullptr;
	}
	BV_ASSERT(false, "Unknown ComponentType!");
	return false;
}

void* Entity_GetComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:		          return &entity->Tag;
	case ComponentType_Transform:	      return &entity->Transform;
	case ComponentType_Camera:	          return entity->Camera;
	case ComponentType_SpriteRenderer:	  return entity->SpriteRenderer;
	case ComponentType_CircleRenderer:	  return entity->CircleRenderer;
	case ComponentType_Rigidbody2D:	      return entity->Rigidbody2D;
	case ComponentType_BoxCollider2D:	  return entity->BoxCollider2D;
	case ComponentType_CircleCollider2D:  return entity->CircleCollider2D;
	case ComponentType_Text:              return entity->Text;
	case ComponentType_Script:            return entity->Script;
	}
	BV_ASSERT(false, "Unknown ComponentType!");
	return nullptr;
}

void Entity_AddComponent(Entity* entity, ComponentType type, void* component)
{
	switch (type)
	{
	case ComponentType_Tag:
	{
		entity->Tag = *(TagComponent*)component;
		break;
	}
	case ComponentType_Transform:
	{
		entity->Transform = *(TransformComponent*)component;
		break;
	}
	case ComponentType_Camera:
	{
		BV_ASSERT(!entity->Camera, "Entity already has component!");
		entity->Camera = (CameraComponent*)component;
		break;
	}
	case ComponentType_SpriteRenderer:
	{
		BV_ASSERT(!entity->SpriteRenderer, "Entity already has component!");
		entity->SpriteRenderer = (SpriteRendererComponent*)component;
		break;
	}
	case ComponentType_CircleRenderer:
	{
		BV_ASSERT(!entity->CircleRenderer, "Entity already has component!");
		entity->CircleRenderer = (CircleRendererComponent*)component;
		break;
	}
	case ComponentType_Rigidbody2D:
	{
		BV_ASSERT(!entity->Rigidbody2D, "Entity already has component!");
		entity->Rigidbody2D = (Rigidbody2DComponent*)component;
		break;
	}
	case ComponentType_BoxCollider2D:
	{
		BV_ASSERT(!entity->BoxCollider2D, "Entity already has component!");
		entity->BoxCollider2D = (BoxCollider2DComponent*)component;
		break;
	}
	case ComponentType_CircleCollider2D:
	{
		BV_ASSERT(!entity->CircleCollider2D, "Entity already has component!");
		entity->CircleCollider2D = (CircleCollider2DComponent*)component;
		break;
	}
	case ComponentType_Text:
	{
		BV_ASSERT(!entity->Text, "Entity already has component!");
		entity->Text = (TextComponent*)component;
		break;
	}
	case ComponentType_Script:
	{
		BV_ASSERT(!entity->Script, "Entity already has component!");
		ScriptComponent* script = (ScriptComponent*)component;
		script->Entity = entity;
		entity->Script = script;
		break;
	}
	}
}

void Entity_RemoveComponent(Entity* entity, ComponentType type)
{
	switch (type)
	{
	case ComponentType_Tag:		                                              return;
	case ComponentType_Transform:	                                          return;
	case ComponentType_Camera:	          entity->Camera = nullptr;           return;
	case ComponentType_SpriteRenderer:	  entity->SpriteRenderer = nullptr;   return;
	case ComponentType_CircleRenderer:	  entity->CircleRenderer = nullptr;   return;
	case ComponentType_Rigidbody2D:	      entity->Rigidbody2D = nullptr;      return;
	case ComponentType_BoxCollider2D:	  entity->BoxCollider2D = nullptr;    return;
	case ComponentType_CircleCollider2D:  entity->CircleCollider2D = nullptr; return;
	case ComponentType_Text:              entity->Text = nullptr;             return;
	case ComponentType_Script:            entity->Script = nullptr;           return;
	}
	BV_ASSERT(false, "Unknown ComponentType!");
}