#pragma once
#include "Math/Math.h"
#include "Collisions.h"

struct Rigidbody2D
{
	enum BodyType { Static = 0, Dynamic, Kinematic };
	BodyType Type = BodyType::Static;
	Vec2 Position = { 0.0f, 0.0f };
	float Rotation = 0.0f;
	Vec2 Velocity = { 0.0f, 0.0f };
	float AngularVelocity = 0.0f;
	bool FixedRotation = false;

	enum ShapeType { Box = 0, Circle };
	ShapeType Shape = ShapeType::Box;

	BoxCollider2D BoxCollider;
	CircleCollider2D CircleCollider;

	void* Entity = nullptr;
};

void Rigidbody2D_CreateShape(Rigidbody2D& rigidbody2D, void* shape);
void Rigidbody2D_ApplyForce(void* rigidbody2D, const Vec2& force);