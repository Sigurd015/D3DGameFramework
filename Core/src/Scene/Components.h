#pragma once
#include "Math/Math.h"
#include "SceneCamera.h"
#include "Renderer/Texture.h"
#include "Physics/Rigidbody.h"

enum ComponentType
{
	ComponentType_Tag = 0,
	ComponentType_Transform,
	ComponentType_RectTransform,
	ComponentType_Camera,
	ComponentType_SpriteRenderer,
	ComponentType_CircleRenderer,
	ComponentType_Rigidbody2D,
	ComponentType_BoxCollider2D,
	ComponentType_CircleCollider2D,
	ComponentType_Text,
	ComponentType_Script,
	ComponentType_Count
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

struct RectTransformComponent
{
	//Notice: Position is left bottom, [0,0] is left bottom, [1,1] is right top
	//but for Text, [0,0] is left top, [1,1] is right bottom (Text handled by D2D)
	Vec2 Position = { 0.0f, 0.0f };
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

void Rigidbody2DComponent_ApplyForce(Rigidbody2DComponent& rigidbody2D, const Vec2& force);
void Rigidbody2DComponent_ApplyRotation(Rigidbody2DComponent& rigidbody2D, float rotation);
void Rigidbody2DComponent_MovePosition(Rigidbody2DComponent& rigidbody2D, const Vec2& position);

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

// Forward declarations
struct Entity;

struct ScriptComponent
{
	void (*OnCreate)(Entity& entity, void* runtimeData);
	void (*OnUpdate)(Entity& entity, float timeStep, void* runtimeData);
	void (*OnDestroy)(Entity& entity, void* runtimeData);
	void (*OnCollision)(Entity& entity, Entity& other, void* runtimeData);
	void (*OnRaycastHit)(Entity& entity, Entity& other, void* runtimeData);
	void (*OnEnable)(Entity& entity, void* runtimeData);
	void (*OnDisable)(Entity& entity, void* runtimeData);

	// For ScriptComponent that used by multiple entities, this is the runtime data for each entity
	void* RuntimeData = nullptr;
};