#include "pch.h"
#include "Scene.h"
#include "Entity.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/RendererAPI.h"
#include "Core/Application.h"
#include "Renderer/SceneRenderer.h"

#include <Audio.h>

void Scene_Create(Scene& scene)
{
	List_Create(scene.Entities, sizeof(Entity));

	List_Create(scene.Environment.PointLights, sizeof(PointLight), MAX_POINT_LIGHT);
	List_Create(scene.Environment.SpotLights, sizeof(SpotLight), MAX_SPOT_LIGHT);
}

void OnCollide(void* entity1, void* entity2)
{
	Entity* ent1 = (Entity*)entity1;
	Entity* ent2 = (Entity*)entity2;
	if (Entity_HasComponent(ent1, ComponentType_Script))
	{
		ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(ent1, ComponentType_Script);
		sc->OnCollision(ent1, ent2, sc->RuntimeData);
	}

	if (Entity_HasComponent(ent2, ComponentType_Script))
	{
		ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(ent2, ComponentType_Script);
		sc->OnCollision(ent2, ent1, sc->RuntimeData);
	}
}

void Scene_Ininialize(Scene& scene)
{
	uint32_t size = List_Size(scene.Entities);

	// Initialize all the audio
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(scene.Entities, i);
			TransformComponent* tc = &temp->Transform;

			if (Entity_HasComponent(temp, ComponentType_Audio))
			{
				AudioComponent* ac = (AudioComponent*)Entity_GetComponent(temp, ComponentType_Audio);
				switch (ac->Type)
				{
				case AudioComponentType_Listener:
				{
					DirectX::AudioListener* listener = new DirectX::AudioListener();
					listener->SetPosition(tc->Translation);
					ac->RuntimeAudio = listener;
					break;
				}
				case AudioComponentType_Emitter:
				{
					DirectX::AudioEmitter* emitter = new DirectX::AudioEmitter();
					emitter->SetPosition(tc->Translation);
					ac->RuntimeAudio = emitter;
					break;
				}
				}
			}
		}
	}

	// Initialize all the physics
	PhysicsWorld2D_Create(scene.PhysicsWorld, OnCollide);
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(scene.Entities, i);
			TransformComponent* tc = &temp->Transform;

			if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rb2d = (Rigidbody2DComponent*)Entity_GetComponent(temp, ComponentType_Rigidbody2D);

				Rigidbody2D rigidbody2D;
				rigidbody2D.Type = rb2d->Type;
				rigidbody2D.Position = { tc->Translation.x, tc->Translation.y };
				rigidbody2D.Rotation = tc->Rotation.z;
				rigidbody2D.FixedRotation = rb2d->FixedRotation;
				rigidbody2D.Entity = temp;
				rigidbody2D.Enabled = temp->Enabled;

				if (Entity_HasComponent(temp, ComponentType_CircleCollider2D))
				{
					CircleCollider2DComponent* cc2d = (CircleCollider2DComponent*)Entity_GetComponent(temp, ComponentType_CircleCollider2D);

					rigidbody2D.Density = cc2d->Density;
					rigidbody2D.Restitution = cc2d->Restitution;
					rigidbody2D.Friction = cc2d->Friction;
					rigidbody2D.RestitutionThreshold = cc2d->RestitutionThreshold;
					rigidbody2D.IsTrigger = cc2d->IsTrigger;

					// Notice: CircleCollider2D only support uniform scale
					Rigidbody2D_CreateCircleCollider(rigidbody2D, cc2d->Offset, cc2d->Radius * tc->Scale.x);
				}

				if (Entity_HasComponent(temp, ComponentType_BoxCollider2D))
				{
					BoxCollider2DComponent* bc2d = (BoxCollider2DComponent*)Entity_GetComponent(temp, ComponentType_BoxCollider2D);

					rigidbody2D.Density = bc2d->Density;
					rigidbody2D.Restitution = bc2d->Restitution;
					rigidbody2D.Friction = bc2d->Friction;
					rigidbody2D.RestitutionThreshold = bc2d->RestitutionThreshold;
					rigidbody2D.IsTrigger = bc2d->IsTrigger;

					Rigidbody2D_CreateBoxCollider(rigidbody2D, bc2d->Offset, { bc2d->Size.x * tc->Scale.x, bc2d->Size.y * tc->Scale.y });
				}

				rb2d->RuntimeBody = PhysicsWorld2D_AddRigidbody2D(scene.PhysicsWorld, rigidbody2D);
			}
		}
	}

	// Initialize all the scripts
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(scene.Entities, i);
			if (Entity_HasComponent(temp, ComponentType_Script))
			{
				ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(temp, ComponentType_Script);
				sc->OnCreate(temp, sc->RuntimeData);
			}
		}
	}
}

void Scene_Destroy(Scene& scene)
{
	// Destroy all the entities and their components
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);

		//Entity_RemoveComponent(temp, ComponentType_Tag);
		//Entity_RemoveComponent(temp, ComponentType_Transform);

		if (Entity_HasComponent(temp, ComponentType_RectTransform))
		{
			Entity_RemoveComponent(temp, ComponentType_RectTransform);
		}
		if (Entity_HasComponent(temp, ComponentType_Script))
		{
			ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(temp, ComponentType_Script);
			sc->OnDestroy(temp, sc->RuntimeData);
			Entity_RemoveComponent(temp, ComponentType_Script);
		}
		if (Entity_HasComponent(temp, ComponentType_Camera))
		{
			Entity_RemoveComponent(temp, ComponentType_Camera);
		}
		if (Entity_HasComponent(temp, ComponentType_SpriteRenderer))
		{
			Entity_RemoveComponent(temp, ComponentType_SpriteRenderer);
		}
		if (Entity_HasComponent(temp, ComponentType_CircleRenderer))
		{
			Entity_RemoveComponent(temp, ComponentType_CircleRenderer);
		}
		if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
		{
			Entity_RemoveComponent(temp, ComponentType_Rigidbody2D);
		}
		if (Entity_HasComponent(temp, ComponentType_BoxCollider2D))
		{
			Entity_RemoveComponent(temp, ComponentType_BoxCollider2D);
		}
		if (Entity_HasComponent(temp, ComponentType_CircleCollider2D))
		{
			Entity_RemoveComponent(temp, ComponentType_CircleCollider2D);
		}
		if (Entity_HasComponent(temp, ComponentType_Text))
		{
			Entity_RemoveComponent(temp, ComponentType_Text);
		}
		if (Entity_HasComponent(temp, ComponentType_Audio))
		{
			Entity_RemoveComponent(temp, ComponentType_Audio);
		}
	}

	List_Free(scene.Entities);
	List_Free(scene.Environment.PointLights);
	List_Free(scene.Environment.SpotLights);
	PhysicsWorld2D_Destory(scene.PhysicsWorld);
}

void Scene_AddEntity(Scene& scene, Entity& entity)
{
	entity.Scene = &scene;
	List_Add(scene.Entities, &entity);
}

Entity* Scene_GetEntityByName(const Scene* scene, const char* name)
{
	uint32_t size = List_Size(scene->Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene->Entities, i);
		if (String_Compare(temp->Tag.Name, name))
			return temp;
	}
	return nullptr;
}

Entity* Scene_GetPrimaryCamera(const Scene& scene)
{
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Camera))
		{
			CameraComponent* cc = (CameraComponent*)Entity_GetComponent(temp, ComponentType_Camera);
			if (cc->Primary)
				return temp;
		}
	}
	return nullptr;
}

// Notice: Make sure only one listener in the scene, that's player
void* Scene_GetListener(const Scene* scene)
{
	uint32_t size = List_Size(scene->Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene->Entities, i);
		TransformComponent* tc = &temp->Transform;

		if (Entity_HasComponent(temp, ComponentType_Audio))
		{
			AudioComponent* ac = (AudioComponent*)Entity_GetComponent(temp, ComponentType_Audio);
			switch (ac->Type)
			{
			case AudioComponentType_Listener:
			{
				return ac->RuntimeAudio;
			}
			}
		}
	}
	return nullptr;
}

void Scene_SetEntityEnabled(Entity* entity, bool enabled)
{
	entity->Enabled = enabled;
	if (Entity_HasComponent(entity, ComponentType_Rigidbody2D))
	{
		Rigidbody2DComponent* rb2d = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
		Rigidbody2D* rigidbody2D = (Rigidbody2D*)rb2d->RuntimeBody;
		rigidbody2D->Enabled = enabled;
	}

	if (Entity_HasComponent(entity, ComponentType_Script))
	{
		ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(entity, ComponentType_Script);
		if (enabled)
			sc->OnEnable(entity, sc->RuntimeData);
		else
			sc->OnDisable(entity, sc->RuntimeData);
	}
}

#ifndef CORE_DIST
void ColliderVisualiztion(const Scene& scene)
{
	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		TransformComponent* tc = &temp->Transform;

		if (!temp->Enabled)
			continue;

		if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
		{
			if (Entity_HasComponent(temp, ComponentType_CircleCollider2D))
			{
				CircleCollider2DComponent* cc2d = (CircleCollider2DComponent*)Entity_GetComponent(temp, ComponentType_CircleCollider2D);

				Vec3 translation = Vec3Add(tc->Translation, Vec3(cc2d->Offset.x, cc2d->Offset.y, 0.001f));
				float radius = cc2d->Radius * 2.05f;
				Vec3 scale = Vec3MulVec3(Vec3(radius, radius, radius), tc->Scale);

				Mat transform = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
					* DirectX::XMMatrixTranslation(translation.x, translation.y, translation.z);

				Renderer2D_DrawCircle(transform, Vec4(0, 1, 0, 1), 0.01f);
			}

			if (Entity_HasComponent(temp, ComponentType_BoxCollider2D))
			{
				BoxCollider2DComponent* bc2d = (BoxCollider2DComponent*)Entity_GetComponent(temp, ComponentType_BoxCollider2D);

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

void Scene_OnUpdate(Scene& scene, float timeStep)
{
	// Find primary camera
	Mat viewProjection;
	{
		Entity* mainCamera = Scene_GetPrimaryCamera(scene);
		if (!mainCamera)
			return;

		CameraComponent* cc = (CameraComponent*)Entity_GetComponent(mainCamera, ComponentType_Camera);

		viewProjection = DirectX::XMMatrixInverse(nullptr, TransformComponent_GetTransform(mainCamera->Transform))
			* SceneCamera_GetProjectionMatrix(cc->Camera);
	}

	scene.Environment.ViewProjection = viewProjection;

	uint32_t size = List_Size(scene.Entities);

	// Update scripts
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(scene.Entities, i);

			if (Entity_HasComponent(temp, ComponentType_Script) && temp->Enabled)
			{
				ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(temp, ComponentType_Script);
				sc->OnUpdate(temp, timeStep, sc->RuntimeData);
			}
		}
	}

	// Update Physics
	// Notice: Not support changing collider size and offset at runtime
	{
		static const uint32_t iterations = 10;

		PhysicsWorld2D_Update(scene.PhysicsWorld, timeStep, iterations);
		{
			for (size_t i = 0; i < size; i++)
			{
				Entity* temp = (Entity*)List_Get(scene.Entities, i);

				if (!temp->Enabled)
					continue;

				TransformComponent* tc = &temp->Transform;

				if (Entity_HasComponent(temp, ComponentType_Rigidbody2D))
				{
					Rigidbody2DComponent* rb2d = (Rigidbody2DComponent*)Entity_GetComponent(temp, ComponentType_Rigidbody2D);
					Rigidbody2D* rb = (Rigidbody2D*)rb2d->RuntimeBody;
					tc->Translation = { rb->Position.x, rb->Position.y, tc->Translation.z };
					tc->Rotation.z = rb->Rotation;
				}
			}
		}
	}

	// Update audio
	{
		for (size_t i = 0; i < size; i++)
		{
			Entity* temp = (Entity*)List_Get(scene.Entities, i);

			if (!temp->Enabled)
				continue;

			TransformComponent* tc = &temp->Transform;

			if (Entity_HasComponent(temp, ComponentType_Audio))
			{
				AudioComponent* ac = (AudioComponent*)Entity_GetComponent(temp, ComponentType_Audio);
				switch (ac->Type)
				{
				case AudioComponentType_Listener:
				{
					DirectX::AudioListener* listener = (DirectX::AudioListener*)ac->RuntimeAudio;
					listener->Update({ tc->Translation.x,tc->Translation.y,tc->Translation.z }, { 0,0,-1.0f }, timeStep);
					break;
				}
				case AudioComponentType_Emitter:
				{
					DirectX::AudioEmitter* emitter = (DirectX::AudioEmitter*)ac->RuntimeAudio;
					emitter->Update({ tc->Translation.x,tc->Translation.y,tc->Translation.z }, { 0,0,-1.0f }, timeStep);
					break;
				}
				}
			}
		}
	}

	// Rendering
	{
		// 3D rendering
		//----------------- 3D Scene Rendering -----------------//					
		{
			// Lights
			{
				List_Clear(scene.Environment.PointLights);
				List_Clear(scene.Environment.SpotLights);

				for (size_t i = 0; i < size; i++)
				{
					Entity* temp = (Entity*)List_Get(scene.Entities, i);

					if (!temp->Enabled)
						continue;

					TransformComponent* tc = (TransformComponent*)Entity_GetComponent(temp, ComponentType_Transform);

					if (Entity_HasComponent(temp, ComponentType_SkyLight))
					{
						SkyLightComponent* skylight = (SkyLightComponent*)Entity_GetComponent(temp, ComponentType_SkyLight);
						scene.Environment.EnvMap = skylight->EnvMap;
						scene.Environment.SkyLightIntensity = skylight->Intensity;
					}
					else if (Entity_HasComponent(temp, ComponentType_Light))
					{
						LightComponent* light = (LightComponent*)Entity_GetComponent(temp, ComponentType_Light);
						switch (light->Type)
						{
						case LightComponent::LightType_Directional:
						{
							scene.Environment.DirLight = {
								light->Radiance,
								light->Intensity,
								Vec3Normalize(Vec3MulMat(Vec3(1.0f, 1.0f, 1.0f),Mat4ToMat3(TransformComponent_GetTransform(*tc)))),
								light->Shadow,
							};
							break;
						}
						case LightComponent::LightType_Point:
						{
							PointLight pointLight = {
								tc->Translation,
								light->Intensity,
								light->Radiance,
								light->Radius,
								light->Falloff,
								light->Shadow,
							};
							List_Add(scene.Environment.PointLights, &pointLight);
							break;
						}
						case LightComponent::LightType_Spot:
						{
							SpotLight spotLight = {
								tc->Translation,
								light->Intensity,
								light->Radiance,
								light->Radius,
								Vec3Normalize(Vec3MulMat(Vec3(1.0f, 0.0f, 0.0f),Mat4ToMat3(TransformComponent_GetRotation(*tc)))),
								light->Range,
								light->Angle,
								light->Falloff,
								light->Shadow,
							};
							List_Add(scene.Environment.SpotLights, &spotLight);
							break;
						}
						}
					}
				}
			}
			// Draw 3D objects
			{
				SceneRenderer_BeginScene(scene.Environment);

				for (size_t i = 0; i < size; i++)
				{
					Entity* temp = (Entity*)List_Get(scene.Entities, i);

					if (!temp->Enabled)
						continue;

					TransformComponent* tc = (TransformComponent*)Entity_GetComponent(temp, ComponentType_Transform);

					if (Entity_HasComponent(temp, ComponentType_Mesh))
					{
						MeshComponent* mesh = (MeshComponent*)Entity_GetComponent(temp, ComponentType_Mesh);
						//ScnenRenderer_SubmitStaticMesh(TransformComponent_GetTransform(*tc), mesh);
					}
				}
				SceneRenderer_EndScene();
			}
		}
		//----------------- 3D Scene Rendering -----------------//	

		// 2D rendering
		//----------------- 2D Scene Rendering -----------------//
		{
			Renderer2D_SetTargetFrameBuffer(RenderPass_GetTargetFramebuffer(SceneRenderer_GetFinalPass()));
			Renderer2D_BeginScene(viewProjection);

			for (size_t i = 0; i < size; i++)
			{
				Entity* temp = (Entity*)List_Get(scene.Entities, i);

				if (!temp->Enabled)
					continue;

				if (!Entity_HasComponent(temp, ComponentType_RectTransform))
				{
					TransformComponent& tc = temp->Transform;
					if (Entity_HasComponent(temp, ComponentType_SpriteRenderer))
					{
						SpriteRendererComponent* sprite = (SpriteRendererComponent*)Entity_GetComponent(temp, ComponentType_SpriteRenderer);

						if (sprite->Texture)
						{
							Renderer2D_DrawQuad(
								TransformComponent_GetTransform(tc),
								sprite->Texture,
								sprite->UVStart,
								sprite->UVEnd,
								sprite->Color,
								sprite->TilingFactor
							);
						}
						else
						{
							Renderer2D_DrawQuad(TransformComponent_GetTransform(tc), sprite->Color);
						}
					}

					if (Entity_HasComponent(temp, ComponentType_CircleRenderer))
					{
						CircleRendererComponent* circle = (CircleRendererComponent*)Entity_GetComponent(temp, ComponentType_CircleRenderer);

						Renderer2D_DrawCircle(
							TransformComponent_GetTransform(tc),
							circle->Color,
							circle->Thickness,
							circle->Fade
						);
					}
				}
				else
				{
					RectTransformComponent* rect = (RectTransformComponent*)Entity_GetComponent(temp, ComponentType_RectTransform);
					Vec2 position, size, ndcPos;
					Vec2 viewPortSize = { (float)scene.ViewportWidth,(float)scene.ViewportHeight };
					RectTransformComponent_GetPositionAndSize(*rect, viewPortSize, &ndcPos, &position, &size);

					if (Entity_HasComponent(temp, ComponentType_SpriteRenderer))
					{
						SpriteRendererComponent* sprite = (SpriteRendererComponent*)Entity_GetComponent(temp, ComponentType_SpriteRenderer);

						if (sprite->Texture)
						{
							Renderer2D_DrawUI(
								ndcPos,
								size,
								rect->Rotation,
								sprite->Texture,
								sprite->UVStart,
								sprite->UVEnd,
								sprite->Color,
								sprite->TilingFactor
							);
						}
						else
						{
							Renderer2D_DrawUI(ndcPos, size, rect->Rotation, sprite->Color);
						}
					}

					if (Entity_HasComponent(temp, ComponentType_Text))
					{
						TextComponent* text = (TextComponent*)Entity_GetComponent(temp, ComponentType_Text);
						Renderer2D_DrawText(text->TextString, text->FontName, position, text->Color, text->FontSize);
					}
				}
			}
#ifndef CORE_DIST
			ColliderVisualiztion(scene);
#endif
			Renderer2D_EndScene();
		}
		//----------------- 2D Scene Rendering -----------------//
	}
}

void Scene_OnViewportResize(Scene& scene, uint32_t width, uint32_t height)
{
	if (scene.ViewportWidth == width && scene.ViewportHeight == height)
		return;

	scene.ViewportWidth = width;
	scene.ViewportHeight = height;

	uint32_t size = List_Size(scene.Entities);
	for (size_t i = 0; i < size; i++)
	{
		Entity* temp = (Entity*)List_Get(scene.Entities, i);
		if (Entity_HasComponent(temp, ComponentType_Camera))
		{
			CameraComponent* cc = (CameraComponent*)Entity_GetComponent(temp, ComponentType_Camera);
			if (!cc->FixedAspectRatio)
				SceneCamera_SetViewportSize(cc->Camera, width, height);
		}
	}
}

bool LayerMask(void* entity, const char* mask)
{
	Entity* ent = (Entity*)entity;
	return String_Contains(ent->Tag.Name, mask);
}

bool Scene_Raycast(Scene* scene, Entity* entity, const Vec2& rayDirection, const char* mask, float maxDistance)
{
	Vec2 rayOrigin = Vec3ToVec2(entity->Transform.Translation);

	//TODO: Make a better way to do this
	Rigidbody2D* rigidbody2D = nullptr;
	if (Entity_HasComponent(entity, ComponentType_Rigidbody2D))
	{
		Rigidbody2DComponent* rb2d = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
		rigidbody2D = (Rigidbody2D*)rb2d->RuntimeBody;
		rigidbody2D->Enabled = false;
	}
	float distance;
	void* ent = PhysicsWorld2D_Raycast(scene->PhysicsWorld, rayOrigin, rayDirection, distance);
	if (rigidbody2D != nullptr)
		rigidbody2D->Enabled = true;

	if (ent != nullptr && LayerMask(ent, mask) && distance < maxDistance)
	{
		Entity* temp = (Entity*)ent;
		if (Entity_HasComponent(temp, ComponentType_Script))
		{
			ScriptComponent* sc = (ScriptComponent*)Entity_GetComponent(temp, ComponentType_Script);
			sc->OnRaycastHit(temp, entity, sc->RuntimeData);
		}
		return true;
	}
	return false;
}