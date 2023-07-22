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
		if (Entity_HasComponent(*temp, ComponentType_Script))
		{
			temp->Script->OnCreate(*temp);
		}
	}

	PhysicsWorld2D_Create(out.PhysicsWorld);
}

void Scene_Destroy(Scene& out)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		if (Entity_HasComponent(*temp, ComponentType_Script))
		{
			temp->Script->OnDestroy(*temp);
			free(temp->Script);
		}

		if (Entity_HasComponent(*temp, ComponentType_Camera))
		{
			if (temp->Camera->Camera)
				free(temp->Camera->Camera);
			free(temp->Camera);
		}

		if (Entity_HasComponent(*temp, ComponentType_SpriteRenderer))
		{
			if (temp->SpriteRenderer->Texture)
			{
				Texture2D_Release(*temp->SpriteRenderer->Texture);
				//TODO: Data has been freed by other entity(That using same texture), so we can't free it again
				// Maybe we should make a reference counter feature
				//free(temp->SpriteRenderer->Texture);
			}
			free(temp->SpriteRenderer);
		}

		if (Entity_HasComponent(*temp, ComponentType_CircleRenderer))
		{
			free(temp->CircleRenderer);
		}

		if (Entity_HasComponent(*temp, ComponentType_Rigidbody2D))
		{
			free(temp->Rigidbody2D);
		}
		if (Entity_HasComponent(*temp, ComponentType_BoxCollider2D))
		{
			free(temp->BoxCollider2D);
		}

		if (Entity_HasComponent(*temp, ComponentType_CircleCollider2D))
		{
			free(temp->CircleCollider2D);
		}

		if (Entity_HasComponent(*temp, ComponentType_Text))
		{
			free(temp->Text);
		}
	}

	List_Free(out.Entities, true);

	PhysicsWorld2D_Destory(out.PhysicsWorld);
}

void Scene_AddEntity(Scene& out, Entity& entity)
{
	entity.Scene = &out;
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
		if (Entity_HasComponent(*temp, ComponentType_Camera))
			if (temp->Camera->Primary)
				return temp;
	}
	return nullptr;
}

void Scene_OnUpdate(Scene& out, float timeStep, bool enablePhysicsVisualization)
{
	// Find primary camera
	{
		Entity* mainCamera = Scene_GetPrimaryCamera(out);
		if (!mainCamera)
			return;

		SceneCamera* camera = mainCamera->Camera->Camera;
		TransformComponent* cameraTrans = &mainCamera->Transform;
		Mat viewProjection = DirectX::XMMatrixInverse(nullptr, TransformComponent_GetTransform(*cameraTrans)) * camera->Projection;
		Renderer2D_BeginScene(viewProjection);
	}

	bool isFixedUpdate = PhysicsWorld2D_IsFixedUpdate(timeStep);

	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		TransformComponent* tc = &temp->Transform;

		// Update scripts
		{
			if (Entity_HasComponent(*temp, ComponentType_Script))
			{
				temp->Script->OnUpdate(*temp, timeStep);
			}
		}

		// Update Physics
		{
			if (Entity_HasComponent(*temp, ComponentType_Rigidbody2D))
			{
				//TODO: Implement Rigidbody2D When we need it
			}

			if (Entity_HasComponent(*temp, ComponentType_CircleCollider2D))
			{
				CircleCollider2DComponent* cc2d = temp->CircleCollider2D;

				if (isFixedUpdate)
				{
					CircleCollider2D collider = {
						{ tc->Translation.x + cc2d->Offset.x, tc->Translation.y + cc2d->Offset.y },
						cc2d->Radius * tc->Scale.x,// Notice: CircleCollider2D only support uniform scale
						temp,
					};

					PhysicsWorld2D_AddCircleCollider2D(out.PhysicsWorld, collider);
				}

				if (enablePhysicsVisualization)
				{
					Vec3 translation = Vec3Add(tc->Translation, Vec3(cc2d->Offset.x, cc2d->Offset.y, 0.001f));
					float radius = cc2d->Radius * 2.05f;
					Vec3 scale = Vec3MulVec3(Vec3(radius, radius, radius), tc->Scale);

					Mat transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
						* DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

					Renderer2D_DrawCircle(transform, Vec4(0, 1, 0, 1), 0.01f);
				}
			}

			if (Entity_HasComponent(*temp, ComponentType_BoxCollider2D))
			{
				BoxCollider2DComponent* bc2d = temp->BoxCollider2D;

				if (isFixedUpdate)
				{
					BoxCollider2D collider = {
						{ tc->Translation.x + bc2d->Offset.x, tc->Translation.y + bc2d->Offset.y },
						bc2d->Size.x * tc->Scale.x,
						bc2d->Size.y * tc->Scale.y,
						tc->Rotation.z,
						temp,
					};

					PhysicsWorld2D_AddBoxCollider2D(out.PhysicsWorld, collider);
				}

				if (enablePhysicsVisualization)
				{
					Vec3 bc2dTranslation = Vec3(bc2d->Offset.x, bc2d->Offset.y, 0.001f);
					Vec3 translation = Vec3Add(tc->Translation, bc2dTranslation);
					Vec2 size = Vec2MulFloat(bc2d->Size, 2.05f);
					Vec3 scale = Vec3MulVec3(Vec3(size.x, size.y, 1.0f), tc->Scale);

					Mat transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
						* DirectX::XMMatrixTranslation(bc2dTranslation.x, bc2dTranslation.y, bc2dTranslation.z)
						* DirectX::XMMatrixRotationZ(tc->Rotation.z)
						* DirectX::XMMatrixTranslation(tc->Translation.x, tc->Translation.y, tc->Translation.z);

					Renderer2D_DrawRect(transform, Vec4(0, 1, 0, 1));
				}
			}
		}

		// Rendering
		{
			if (Entity_HasComponent(*temp, ComponentType_SpriteRenderer))
			{
				SpriteRendererComponent* sprite = temp->SpriteRenderer;

				if (sprite->Texture)
				{
					Renderer2D_DrawQuad(
						TransformComponent_GetTransform(*tc),
						*sprite->Texture,
						sprite->UVStart,
						sprite->UVEnd,
						sprite->Color,
						sprite->TilingFactor
					);
				}
				else
				{
					Renderer2D_DrawQuad(TransformComponent_GetTransform(*tc), sprite->Color);
				}
			}

			if (Entity_HasComponent(*temp, ComponentType_CircleRenderer))
			{
				CircleRendererComponent* circle = temp->CircleRenderer;

				Renderer2D_DrawCircle(
					TransformComponent_GetTransform(*tc),
					circle->Color,
					circle->Thickness,
					circle->Fade
				);
			}
		}
	}

	if (isFixedUpdate)
	{
		PhysicsWorld2D_Update(out.PhysicsWorld);
		PhysicsWorld2D_Clear(out.PhysicsWorld);
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
		if (Entity_HasComponent(*temp, ComponentType_Camera))
			if (!temp->Camera->FixedAspectRatio)
				SceneCamera_SetViewportSize(*temp->Camera->Camera, width, height);
	}
}