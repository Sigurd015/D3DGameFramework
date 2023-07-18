#include "pch.h"
#include "Scene.h"

Entity* Scene_GetEntityByName(const Scene& scene, const char* name)
{
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		if (!strcmp(temp->Tag.Name, name))
			return temp;
	}
	return nullptr;
}

void Scene_OnUpdate(const Scene& scene, float timeStep)
{
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		TransformComponent ts = temp->Transform;

		// Update scripts
		{
			if (Entity_HasComponent(temp, ComponentType_Script))
			{
				temp->Script->OnUpdate(*temp, timeStep);
				continue;
			}
		}

		// Update Physics
		{
			if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
			{

				continue;
			}

			if (Entity_HasComponent(temp, ComponentType_CircleCollider2D))
			{

				continue;
			}

			if (Entity_HasComponent(temp, ComponentType_BoxCollider2D))
			{

				continue;
			}
		}

		// Rendering
		{
			if (Entity_HasComponent(temp, ComponentType_Camera))
			{

				continue;
			}

			if (Entity_HasComponent(temp, ComponentType_SpriteRenderer))
			{

				continue;
			}

			if (Entity_HasComponent(temp, ComponentType_CircleRenderer))
			{

				continue;
			}
		}
	}
}

void Scene_OnViewportResize(const Scene& scene, uint32_t width, uint32_t height)
{
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Camera))
			if (!temp->Camera->FixedAspectRatio)
				SceneCamera_SetViewportSize(temp->Camera->Camera, width, height);
	}
}