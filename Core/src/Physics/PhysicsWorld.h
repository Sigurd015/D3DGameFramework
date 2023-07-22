#pragma once
#include "Math/Math.h"
#include "Utils/List.h"
#include "Scene/Entity.h"

#define PHYSICS_TIME_STEP 1.0f / 30.0f
#define PHYSICS_COLLIDER_POOL_SIZE 1024

struct Rigidbody2D
{
	enum class BodyType { Static = 0, Dynamic, Kinematic };
	BodyType Type = BodyType::Static;
	bool FixedRotation = false;
};

struct BoxCollider2D
{
	Vec2 Pos = { 0.0f, 0.0f };
	float HalfWidth = 1.0f;
	float HalfHeight = 1.0f;
	float Angle = 0.0f;

	Entity* Entity = nullptr;
};

struct CircleCollider2D
{
	Vec2 Pos = { 0.0f, 0.0f };
	float Radius = 1.0f;

	Entity* Entity = nullptr;
};

struct PhysicsWorld2D
{
	List BoxCollider2Ds;
	List CircleCollider2Ds;

	uint32_t BoxCollider2DCount = 0;
	uint32_t CircleCollider2DCount = 0;
};

void PhysicsWorld2D_Create(PhysicsWorld2D& world);
void PhysicsWorld2D_Clear(PhysicsWorld2D& world);
void PhysicsWorld2D_Destory(PhysicsWorld2D& world);
bool PhysicsWorld2D_IsFixedUpdate(float timeStep);
void PhysicsWorld2D_AddBoxCollider2D(PhysicsWorld2D& world, BoxCollider2D& boxCollider2D);
void PhysicsWorld2D_AddCircleCollider2D(PhysicsWorld2D& world, CircleCollider2D& circleCollider2D);
void PhysicsWorld2D_Update(PhysicsWorld2D& world);