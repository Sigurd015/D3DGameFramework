#pragma once
#include "Entity.h"
#include "Physics/PhysicsWorld.h"

#define MAX_POINT_LIGHT 32
#define MAX_SPOT_LIGHT 32

struct DirectionalLight
{
	Vec3 Radiance = { 1.0f,1.0f,1.0f };
	float Intensity = 0.0f;
	Vec3 Direction = { 0.0f, 0.0f, 0.0f };

	LightComponent::ShadowType ShadowType = LightComponent::ShadowType_None;
};

struct PointLight
{
	Vec3 Position = { 0.0f, 0.0f, 0.0f };
	float Intensity = 0.0f;
	Vec3 Radiance = { 1.0f,1.0f,1.0f };
	float Radius = 0.0f;
	float Falloff = 0.0f;

	LightComponent::ShadowType ShadowType = LightComponent::ShadowType_None;
};

struct SpotLight
{
	Vec3 Position = { 0.0f, 0.0f, 0.0f };
	float Intensity = 0.0f;
	Vec3 Radiance = { 1.0f,1.0f,1.0f };
	float AngleAttenuation;
	Vec3 Direction = { 0.0f, 0.0f, 0.0f };
	float Range = 0.0f;
	float Angle = 0.0f;
	float Falloff = 0.0f;

	LightComponent::ShadowType ShadowType = LightComponent::ShadowType_None;
};

struct Environment
{
	Vec3 CameraPosition;
	float CameraNearClip;
	float CameraFarClip;
	Mat View;
	Mat Projection;
	Mat ViewProjection;

	EnvMap* EnvMap = nullptr;
	float SkyLightIntensity = 0.0f;

	DirectionalLight DirLight;

	List PointLights;
	List SpotLights;
};

struct Scene
{
	List Entities;

	uint32_t ViewportWidth;
	uint32_t ViewportHeight;

	Environment Environment;

	PhysicsWorld2D PhysicsWorld;
};

void Scene_Create(Scene& scene);
void Scene_Ininialize(Scene& scene);
void Scene_Destroy(Scene& scene);
void Scene_AddEntity(Scene& scene, Entity& entity);
Entity* Scene_GetEntityByName(const Scene* scene, const char* name);
Entity* Scene_GetPrimaryCamera(const Scene& scene);
void* Scene_GetListener(const Scene* scene);
void Scene_SetEntityEnabled(Entity* entity, bool enabled);
void Scene_OnUpdate(Scene& scene, float timeStep);
void Scene_OnViewportResize(Scene& scene, uint32_t width, uint32_t height);
bool Scene_Raycast(Scene* scene, Entity* entity, const Vec2& rayDirection, const char* mask, float maxDistance);