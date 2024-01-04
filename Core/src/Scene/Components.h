#pragma once
#include "Math/Math.h"
#include "SceneCamera.h"
#include "Renderer/Texture.h"
#include "Physics/Rigidbody.h"
#include "Renderer/EnvMap.h"
#include "Renderer/Mesh.h"
#include "Renderer/Material.h"

enum ComponentType
{
	ComponentType_RectTransform,
	ComponentType_Camera,
	ComponentType_SpriteRenderer,
	ComponentType_CircleRenderer,
	ComponentType_Rigidbody2D,
	ComponentType_BoxCollider2D,
	ComponentType_CircleCollider2D,
	ComponentType_Text,
	ComponentType_Script,
	ComponentType_Audio,
	ComponentType_Mesh,
	ComponentType_SkyLight,
	ComponentType_Light,
	ComponentType_Count,

	// Any entity must have these two components
	ComponentType_Tag,
	ComponentType_Transform,
};

struct TagComponent
{
	char* Name;
};

struct TransformComponent
{
	Vec3 Translation = { 0.0f, 0.0f, 0.0f };
	Vec3 Rotation = { 0.0f, 0.0f, 0.0f };
	Vec3 Scale = { 1.0f, 1.0f, 1.0f };
};

Mat TransformComponent_GetTransform(const TransformComponent& transform);
Mat TransformComponent_GetRotation(const TransformComponent& transform);
Vec3 TransformComponent_GetUp(const TransformComponent& transform);
Vec3 TransformComponent_GetForward(const TransformComponent& transform);
Vec3 TransformComponent_GetRight(const TransformComponent& transform);

struct RectTransformComponent
{
	//Notice: [0,0] is left bottom, [1,1] is right top
	//but for Text, [0,0] is left top, [1,1] is right bottom (Text handled by D2D)
	Vec2 Position = { 0.0f, 0.0f };
	float Rotation = 0.0f;
	Vec2 Size = { 1.0f, 1.0f };
	Vec2 Scale = { 1.0f, 1.0f };
};

void RectTransformComponent_GetPositionAndSize(const RectTransformComponent& rectTransform, const Vec2& currentViewPortSize,
	Vec2* ndcPos, Vec2* pos, Vec2* size);

struct CameraComponent
{
	SceneCamera Camera;
	bool Primary = true;
	bool FixedAspectRatio = false;
};

struct MeshComponent
{
	Mesh* Mesh = nullptr;
	Material* Material = nullptr;
};

struct SkyLightComponent
{
	EnvMap* EnvMap = nullptr;
	float Intensity = 1.0f;
};

struct LightComponent
{
	enum LightType {
		LightType_None = 0,
		LightType_Directional,
		LightType_Point,
		LightType_Spot
	};
	LightType Type = LightType_None;
	Vec3 Radiance = { 1.0f,1.0f,1.0f };
	float Intensity = 0.0f;
	float Radius = 0.0f;
	float Falloff = 0.0f;
	float AngleAttenuation = 0.0f;
	float Range = 0.0f;
	float Angle = 0.0f;

	enum ShadowType {
		ShadowType_None = 0,
		ShadowType_Hard,
		ShadowType_Soft
	};
	ShadowType Shadow = ShadowType_None;
};

struct CircleRendererComponent
{
	Vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	float Thickness = 1.0f;
	float Fade = 0.005f;
};

struct SpriteRendererComponent
{
	Vec4 Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	Texture2D* Texture;
	float TilingFactor = 1.0f;
	Vec2 UVStart = { 0.0f, 0.0f };
	Vec2 UVEnd = { 1.0f,1.0f };
};

// Physics
struct Rigidbody2DComponent
{
	Rigidbody2D::BodyType Type = Rigidbody2D::BodyType::Static;
	bool FixedRotation = false;
	float GravityScale = 1.0f;

	void* RuntimeBody = nullptr;
};

void Rigidbody2DComponent_ApplyForce(Rigidbody2DComponent* rigidbody2D, const Vec2& force);
void Rigidbody2DComponent_ApplyRotation(Rigidbody2DComponent* rigidbody2D, float rotation);
// No physics effect
void Rigidbody2DComponent_SetPosition(Rigidbody2DComponent* rigidbody2D, const Vec2& position);
void Rigidbody2DComponent_MovePosition(Rigidbody2DComponent* rigidbody2D, const Vec2& position);
void Rigidbody2DComponent_SetRotation(Rigidbody2DComponent* rigidbody2D, float rotation);
float Rigidbody2DComponent_GetRotation(Rigidbody2DComponent* rigidbody2D);
void Rigidbody2DCmponent_Rotate(Rigidbody2DComponent* rigidbody2D, float amount);

struct BoxCollider2DComponent
{
	Vec2 Offset = { 0.0f, 0.0f };
	Vec2 Size = { 0.5f, 0.5f };

	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.1f;
	float RestitutionThreshold = 0.5f;

	bool IsTrigger = false;

	void* RuntimeShape = nullptr;
};

struct CircleCollider2DComponent
{
	Vec2 Offset = { 0.0f, 0.0f };
	float Radius = 0.5f;

	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshold = 0.5f;

	bool IsTrigger = false;

	void* RuntimeShape = nullptr;
};

struct TextComponent
{
	WCHAR* TextString;
	WCHAR* FontName = L"Arial";
	Vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
	float FontSize = 100.0f;
};

enum AudioComponentType
{
	AudioComponentType_Emitter,
	AudioComponentType_Listener,
};

struct AudioComponent
{
	AudioComponentType Type;
	void* RuntimeAudio = nullptr;
};

// Forward declarations
struct Scene;
void AudioComponent_Play(const Scene* scene, AudioComponent* audioComponent, void* soundEffect);

// Forward declarations
struct Entity;
struct ScriptComponent
{
	void (*OnCreate)(Entity* entity, void* runtimeData);
	void (*OnUpdate)(Entity* entity, float timeStep, void* runtimeData);
	void (*OnDestroy)(Entity* entity, void* runtimeData);
	void (*OnCollision)(Entity* entity, Entity* other, void* runtimeData);
	void (*OnRaycastHit)(Entity* entity, Entity* other, void* runtimeData);
	void (*OnEnable)(Entity* entity, void* runtimeData);
	void (*OnDisable)(Entity* entity, void* runtimeData);

	// For ScriptComponent that used by multiple entities, this is the runtime data for each entity
	void* RuntimeData = nullptr;
	uint32_t RuntimeDataSize = 0;
};