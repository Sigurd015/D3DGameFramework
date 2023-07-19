#pragma once
#include "Math/Math.h"
#include "SceneCamera.h"
#include "Renderer/Texture.h"

enum ComponentType
{
	ComponentType_Tag = 0,
	ComponentType_Transform,
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

struct CameraComponent
{
	SceneCamera* Camera;
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
	enum class BodyType { Static = 0, Dynamic, Kinematic };
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;

	// Storage for runtime
	void* RuntimeBody = nullptr;
};

struct BoxCollider2DComponent
{
	Vec2 Offset = { 0.0f, 0.0f };
	Vec2 Size = { 0.5f, 0.5f };

	// TODO: move into physics material in the future maybe
	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshold = 0.5f;

	// Storage for runtime
	void* RuntimeFixture = nullptr;
};

struct CircleCollider2DComponent
{
	Vec2 Offset = { 0.0f, 0.0f };
	float Radius = 0.5f;

	// TODO: move into physics material in the future maybe
	float Density = 1.0f;
	float Friction = 0.5f;
	float Restitution = 0.0f;
	float RestitutionThreshold = 0.5f;

	// Storage for runtime
	void* RuntimeFixture = nullptr;
};

struct TextComponent
{
	char* TextString;
	//Ref<Font> FontAsset = Font::GetDefault();
	Vec4 Color{ 1.0f,1.0f,1.0f,1.0f };
	float Kerning = 0.0f;
	float LineSpacing = 0.0f;
};

// Forward declarations
struct Entity;

struct ScriptComponent
{
	void (*OnCreate)(Entity& entity);
	void (*OnUpdate)(Entity& entity, float timeStep);
	void (*OnDestroy)(Entity& entity);
	void (*OnCollision)(Entity& entity, Entity& other);

	Entity* Entity;
};