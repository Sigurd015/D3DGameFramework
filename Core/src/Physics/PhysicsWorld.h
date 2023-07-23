#pragma once
#include "Math/Math.h"
#include "Utils/List.h"
#include "Collisions.h"
#include "Rigidbody.h"

#define PHYSICS_TIME_STEP 1.0f / 30.0f
#define PHYSICS_POOL_SIZE 1024

struct PhysicsWorld2D
{
	List Rigidbody2Ds;
	uint32_t Rigidbody2DCount = 0;

	void(*CollisionCallback)(void* entity1, void* entity2) = nullptr;
};

void PhysicsWorld2D_Create(PhysicsWorld2D& world, void(*CollisionCallback)(void* entity1, void* entity2));
void PhysicsWorld2D_Clear(PhysicsWorld2D& world);
void PhysicsWorld2D_Destory(PhysicsWorld2D& world);
void* PhysicsWorld2D_AddRigidbody2D(PhysicsWorld2D& world, Rigidbody2D& rigidbody2D);
void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep);