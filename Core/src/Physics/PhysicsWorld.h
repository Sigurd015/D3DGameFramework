#pragma once
#include "Math/Math.h"

// Physics
struct Rigidbody2D
{
	enum class BodyType { Static = 0, Dynamic, Kinematic };
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;

	// Storage for runtime
	void* RuntimeBody = nullptr;
};

struct BoxCollider2D
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

struct CircleCollider2D
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