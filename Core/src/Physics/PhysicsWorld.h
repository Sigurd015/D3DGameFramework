#pragma once
#include "Math/Math.h"
#include "Utils/List.h"
#include "Collisions.h"
#include "Rigidbody.h"

struct PhysicsWorld2D
{
	List Rigidbody2Ds;
	List ContactPairs;

	void(*CollisionCallback)(void* entity1, void* entity2) = nullptr;
};

void PhysicsWorld2D_Create(PhysicsWorld2D& world, void(*CollisionCallback)(void* entity1, void* entity2));
void PhysicsWorld2D_Destory(PhysicsWorld2D& world);
void* PhysicsWorld2D_AddRigidbody2D(PhysicsWorld2D& world, Rigidbody2D& rigidbody2D);
void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep, uint32_t iterations);
void* PhysicsWorld2D_Raycast(PhysicsWorld2D& world, const Vec2& rayOrigin, const Vec2& rayDirection, float& minDistance);