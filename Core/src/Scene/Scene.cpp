#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"

void Scene_Create(Scene& out)
{
	List_Create(out.Entities);
}

void Scene_Ininialize(Scene& out)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Script))
		{
			temp->Script->OnCreate(*temp);
			continue;
		}
	}
}

void Scene_Destroy(Scene& out)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Script))
		{
			temp->Script->OnDestroy(*temp);
			continue;
		}
	}
}

void Scene_AddEntity(Scene& out, const Entity& entity)
{
	List_Add(out.Entities, (void*)&entity);
}

Entity* Scene_GetEntityByName(const Scene& out, const char* name)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (!strcmp(temp->Tag.Name, name))
			return temp;
	}
	return nullptr;
}

Entity* Scene_GetPrimaryCamera(const Scene& out)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Camera))
			if (temp->Camera->Primary)
				return temp;
	}
	return nullptr;
}

void Scene_OnUpdate(const Scene& out, float timeStep)
{
	// Find primary camera
	{
		Entity* mainCamera = Scene_GetPrimaryCamera(out);
		if (!mainCamera)
			return;

		SceneCamera* camera = mainCamera->Camera->Camera;
		TransformComponent ts = mainCamera->Transform;

		Mat viewProjection = TransformComponent_GetTransform(ts) * camera->Projection;
		Renderer2D_BeginScene(viewProjection);

	}

	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		TransformComponent ts = temp->Transform;

		// Update scripts
		{
			if (Entity_HasComponent(temp, ComponentType_Script))
			{
				temp->Script->OnUpdate(*temp, timeStep);
			}
		}

		// Update Physics
		{
			if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
			{

			}

			if (Entity_HasComponent(temp, ComponentType_CircleCollider2D))
			{

			}

			if (Entity_HasComponent(temp, ComponentType_BoxCollider2D))
			{

			}
		}

		// Rendering
		{
			if (Entity_HasComponent(temp, ComponentType_SpriteRenderer))
			{
				SpriteRendererComponent* sprite = temp->SpriteRenderer;

				if (sprite->Texture)
				{
					Renderer2D_DrawQuad(
						TransformComponent_GetTransform(ts),
						*sprite->Texture,
						sprite->UVStart,
						sprite->UVEnd,
						sprite->Color,
						sprite->TilingFactor
					);
				}
				else
				{
					Renderer2D_DrawQuad(TransformComponent_GetTransform(ts), sprite->Color);
				}

				continue;
			}

			if (Entity_HasComponent(temp, ComponentType_CircleRenderer))
			{
				CircleRendererComponent* circle = temp->CircleRenderer;

				Renderer2D_DrawCircle(
					TransformComponent_GetTransform(ts),
					circle->Color,
					circle->Thickness,
					circle->Fade
				);

				continue;
			}
		}
	}

	Renderer2D_EndScene();
}

void Scene_OnViewportResize(Scene& out, uint32_t width, uint32_t height)
{
	if (out.ViewportWidth == width && out.ViewportHeight == height)
		return;

	out.ViewportWidth = width;
	out.ViewportHeight = height;

	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Camera))
			if (!temp->Camera->FixedAspectRatio)
				SceneCamera_SetViewportSize(*temp->Camera->Camera, width, height);
	}
}