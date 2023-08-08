#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererAPI.h"

void Scene_Create(Scene& out)
{
	List_Create(out.Entities);
}

void OnCollide(void* entity1, void* entity2)
{
	Entity* ent1 = (Entity*)entity1;
	Entity* ent2 = (Entity*)entity2;
	if (Entity_HasComponent(*ent1, ComponentType_Script))
	{
		ent1->Script->OnCollision(*ent1, *ent2);
	}

	if (Entity_HasComponent(*ent2, ComponentType_Script))
	{
		ent2->Script->OnCollision(*ent2, *ent1);
	}
}

void Scene_Ininialize(Scene& out)
{
	// Initialize all the scripts
	uint32_t size = List_Size(out.Entities);
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(out.Entities, i);
			if (Entity_HasComponent(*temp, ComponentType_Script))
			{
				temp->Script->OnCreate(*temp);
			}
		}
	}

	// Initialize all the physics
	PhysicsWorld2D_Create(out.PhysicsWorld, OnCollide);
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(out.Entities, i);
			TransformComponent* tc = &temp->Transform;

			if (Entity_HasComponent(*temp, ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rb2d = temp->Rigidbody2D;

				Rigidbody2D rigidbody2D;
				rigidbody2D.Type = rb2d->Type;
				rigidbody2D.Position = { tc->Translation.x, tc->Translation.y };
				rigidbody2D.Rotation = tc->Rotation.z;
				rigidbody2D.FixedRotation = rb2d->FixedRotation;
				rigidbody2D.Entity = temp;
				rigidbody2D.Enabled = temp->Enabled;

				if (Entity_HasComponent(*temp, ComponentType_CircleCollider2D))
				{
					CircleCollider2DComponent* cc2d = temp->CircleCollider2D;

					rigidbody2D.Density = cc2d->Density;
					rigidbody2D.Restitution = cc2d->Restitution;
					rigidbody2D.Friction = cc2d->Friction;
					rigidbody2D.RestitutionThreshold = cc2d->RestitutionThreshold;
					rigidbody2D.IsTrigger = cc2d->IsTrigger;

					// Notice: CircleCollider2D only support uniform scale
					Rigidbody2D_CreateCircleCollider(rigidbody2D, cc2d->Offset, cc2d->Radius * tc->Scale.x);
				}

				if (Entity_HasComponent(*temp, ComponentType_BoxCollider2D))
				{
					BoxCollider2DComponent* bc2d = temp->BoxCollider2D;

					rigidbody2D.Density = bc2d->Density;
					rigidbody2D.Restitution = bc2d->Restitution;
					rigidbody2D.Friction = bc2d->Friction;
					rigidbody2D.RestitutionThreshold = bc2d->RestitutionThreshold;
					rigidbody2D.IsTrigger = bc2d->IsTrigger;

					Rigidbody2D_CreateBoxCollider(rigidbody2D, bc2d->Offset, { bc2d->Size.x * tc->Scale.x, bc2d->Size.y * tc->Scale.y });
				}

				rb2d->RuntimeBody = PhysicsWorld2D_AddRigidbody2D(out.PhysicsWorld, rigidbody2D);
			}
		}
	}
}

void Scene_Destroy(Scene& out)
{
	// Destroy all the entities and their components
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);

		if (Entity_HasComponent(*temp, ComponentType_RectTransform))
		{
			free(temp->RectTransform);
		}

		if (Entity_HasComponent(*temp, ComponentType_Script))
		{
			temp->Script->OnDestroy(*temp);
			//TODO: same issue as Texture2D
			//free(temp->Script);
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

void Scene_SetEntityEnabled(Entity& entity, bool enabled)
{
	entity.Enabled = enabled;
	//TODO: Maybe call script OnEnable/OnDisable
	//      and then set rigidbody2d enabled
	if (Entity_HasComponent(entity, ComponentType_Rigidbody2D))
	{
		Rigidbody2D* rigidbody2D = (Rigidbody2D*)entity.Rigidbody2D->RuntimeBody;
		rigidbody2D->Enabled = enabled;
	}
}

#ifndef CORE_DIST
void ColliderVisualiztion(const Scene& out)
{
	uint32_t size = List_Size(out.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(out.Entities, i);
		TransformComponent* tc = &temp->Transform;

		if(!temp->Enabled)
			continue;

		if (Entity_HasComponent(*temp, ComponentType_Rigidbody2D))
		{
			Rigidbody2DComponent* rb2d = temp->Rigidbody2D;
			Rigidbody2D* rb = (Rigidbody2D*)rb2d->RuntimeBody;
			if (Entity_HasComponent(*temp, ComponentType_CircleCollider2D))
			{
				CircleCollider2DComponent* cc2d = temp->CircleCollider2D;

				Vec3 translation = Vec3Add(tc->Translation, Vec3(cc2d->Offset.x, cc2d->Offset.y, 0.001f));
				float radius = cc2d->Radius * 2.05f;
				Vec3 scale = Vec3MulVec3(Vec3(radius, radius, radius), tc->Scale);

				Mat transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
					* DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

				Renderer2D_DrawCircle(transform, Vec4(0, 1, 0, 1), 0.01f);
			}

			if (Entity_HasComponent(*temp, ComponentType_BoxCollider2D))
			{
				BoxCollider2DComponent* bc2d = temp->BoxCollider2D;

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
}
#endif

void Scene_OnUpdate(Scene& out, float timeStep)
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

	uint32_t size = List_Size(out.Entities);

	// Update scripts
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(out.Entities, i);

			if (Entity_HasComponent(*temp, ComponentType_Script) && temp->Enabled)
			{
				temp->Script->OnUpdate(*temp, timeStep);
			}
		}
	}

	// Update Physics
	// Notice: Not support changing collider size and offset at runtime
	{
		PhysicsWorld2D_Update(out.PhysicsWorld, timeStep, 10);
		{
			for (size_t i = 0; i < size; i++)
			{
				Entity* temp = (Entity*)List_Get(out.Entities, i);

				if (!temp->Enabled)
					continue;

				TransformComponent* tc = &temp->Transform;

				if (Entity_HasComponent(*temp, ComponentType_Rigidbody2D))
				{
					Rigidbody2DComponent* rb2d = temp->Rigidbody2D;
					Rigidbody2D* rb = (Rigidbody2D*)rb2d->RuntimeBody;
					tc->Translation = { rb->Position.x, rb->Position.y, tc->Translation.z };
					tc->Rotation.z = rb->Rotation;
				}
			}
		}
	}

	// Rendering
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(out.Entities, i);

			if (!temp->Enabled)
				continue;

			bool IsUI = Entity_HasComponent(*temp, ComponentType_RectTransform);

			if (!IsUI)
			{
				TransformComponent* tc = &temp->Transform;
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
			else
			{
				RectTransformComponent* rect = temp->RectTransform;
				Vec2 position, size, ndcPos;
				Vec2 viewPortSize = { (float)out.ViewportWidth,(float)out.ViewportHeight };
				RectTransformComponent_GetPositionAndSize(*rect, viewPortSize, &ndcPos, &position, &size);

				if (Entity_HasComponent(*temp, ComponentType_SpriteRenderer))
				{
					SpriteRendererComponent* sprite = temp->SpriteRenderer;

					if (sprite->Texture)
					{
						Renderer2D_DrawUI(
							ndcPos,
							size,
							*sprite->Texture,
							sprite->UVStart,
							sprite->UVEnd,
							sprite->Color,
							sprite->TilingFactor
						);
					}
					else
					{
						Renderer2D_DrawUI(ndcPos, size, sprite->Color);
					}
				}

				if (Entity_HasComponent(*temp, ComponentType_Text))
				{
					TextComponent* text = temp->Text;
					Renderer2D_DrawText(text->TextString, text->FontName, position, text->Color, text->FontSize);
				}
			}
		}
	}

#ifndef CORE_DIST
	ColliderVisualiztion(out);
#endif

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