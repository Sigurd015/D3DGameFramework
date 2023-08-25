#include "pch.h"
#include "PhysicsWorld.h"

struct ContactPair
{
	uint32_t Body1 = 0;
	uint32_t Body2 = 0;
};

void PhysicsWorld2D_Create(PhysicsWorld2D& world, void(*CollisionCallback)(void* entity1, void* entity2))
{
	world.CollisionCallback = CollisionCallback;

	List_Create(world.Rigidbody2Ds, PHYSICS_POOL_SIZE);
	List_Create(world.ContactPairs, PHYSICS_POOL_SIZE);
	{
		Rigidbody2D rigidbody2D;
		for (size_t i = 0; i < PHYSICS_POOL_SIZE; i++)
		{
			List_Add(world.Rigidbody2Ds, sizeof(Rigidbody2D), &rigidbody2D);
		}
	}
	{
		ContactPair contactPair;
		for (size_t i = 0; i < PHYSICS_POOL_SIZE; i++)
		{
			List_Add(world.ContactPairs, sizeof(ContactPair), &contactPair);
		}
	}
	world.Rigidbody2DCount = 0;
	world.ContactPairCount = 0;
}

void PhysicsWorld2D_Destory(PhysicsWorld2D& world)
{
	List_Free(world.Rigidbody2Ds, true);
	List_Free(world.ContactPairs, true);
}

void* PhysicsWorld2D_AddRigidbody2D(PhysicsWorld2D& world, Rigidbody2D& rigidbody2D)
{
	Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, world.Rigidbody2DCount);
	*rigidbody = rigidbody2D;

	if (rigidbody->Type == Rigidbody2D::BodyType::Static)
		rigidbody->InvMass = 0.0f;
	else
		rigidbody->InvMass = 1.0f / rigidbody->Mass;

	world.Rigidbody2DCount++;

	CORE_ASSERT(world.Rigidbody2DCount < PHYSICS_POOL_SIZE, "PhysicsWorld2D_AddRigidbody2D: Rigidbody2D Pool is full");

	return rigidbody;
}

bool Collide(Rigidbody2D& body1, Rigidbody2D& body2, Vec2* normal, float* depth, Vec2* contactPoint, uint32_t* contactPointCount)
{
	Rigidbody2D::ShapeType shape1 = body1.Shape;
	Rigidbody2D::ShapeType shape2 = body2.Shape;

	if (shape1 == Rigidbody2D::ShapeType::Box)
	{
		Vec2 box1Center = Vec2Add(body1.Position, body1.BoxCollider.Offset);

		if (shape2 == Rigidbody2D::ShapeType::Circle)
		{
			Vec2 circleCenter = Vec2Add(body2.Position, body2.CircleCollider.Offset);

			bool result = Collisions_IntersectCirclePolygon(circleCenter, body2.CircleCollider.Radius,
				body1.BoxCollider.Vertices, 4, box1Center, normal, depth, contactPoint, contactPointCount);
			*normal = Vec2MulFloat(*normal, -1);
			return result;
		}
		else if (shape2 == Rigidbody2D::ShapeType::Box)
		{
			Vec2 box2Center = Vec2Add(body2.Position, body2.BoxCollider.Offset);

			return Collisions_IntersectPolygons(body1.BoxCollider.Vertices, 4, box1Center,
				body2.BoxCollider.Vertices, 4, box2Center, normal, depth, contactPoint, contactPointCount);
		}
	}
	else if (shape1 == Rigidbody2D::ShapeType::Circle)
	{
		Vec2 circle1Center = Vec2Add(body1.Position, body1.CircleCollider.Offset);

		if (shape2 == Rigidbody2D::ShapeType::Circle)
		{
			Vec2 circle2Center = Vec2Add(body2.Position, body2.CircleCollider.Offset);

			return Collisions_IntersectCircles(circle1Center, body1.CircleCollider.Radius,
				circle2Center, body2.CircleCollider.Radius, normal, depth, contactPoint, contactPointCount);
		}
		else if (shape2 == Rigidbody2D::ShapeType::Box)
		{
			Vec2 box2Center = Vec2Add(body2.Position, body2.BoxCollider.Offset);
			
			return Collisions_IntersectCirclePolygon(circle1Center, body1.CircleCollider.Radius,
				body2.BoxCollider.Vertices, 4, box2Center, normal, depth, contactPoint, contactPointCount);
		}
	}

	return false;
}

void ResolveCollision(Rigidbody2D& body1, Rigidbody2D& body2, Vec2& normal, float depth,
	Vec2* contactPoint, uint32_t contactPointCount)
{
	// No rotation and friction version
	Vec2 relativeVelocity = Vec2Sub(body2.Velocity, body1.Velocity);

	if (Vec2Dot(relativeVelocity, normal) > 0)
	{
		return;
	}

	float e = FloatMin(body1.Restitution, body2.Restitution);

	float j = -(1.0f + e) * Vec2Dot(relativeVelocity, normal);
	j /= body1.InvMass + body2.InvMass;

	Vec2 impulse = Vec2MulFloat(normal, j);

	body1.Velocity = Vec2Sub(body1.Velocity, Vec2MulFloat(impulse, body1.InvMass));
	body2.Velocity = Vec2Add(body2.Velocity, Vec2MulFloat(impulse, body2.InvMass));
}

void BroadPhase(PhysicsWorld2D& world)
{
	for (size_t i = 0; i < world.Rigidbody2DCount; i++)
	{
		Rigidbody2D* body1 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);
		if (!body1->Enabled)
		{
			continue;
		}

		for (size_t j = i + 1; j < world.Rigidbody2DCount; j++)
		{
			Rigidbody2D* body2 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, j);
			if (body1->Type == Rigidbody2D::BodyType::Static && body2->Type == Rigidbody2D::BodyType::Static)
			{
				continue;
			}

			if (!body2->Enabled)
			{
				continue;
			}

			if (body1->UpdateRequired)
			{
				if (body1->Shape == Rigidbody2D::ShapeType::Box)
					Rigidbody2D_ReCalculBoxColliderVerticesAndAABB(*body1);
				else if (body1->Shape == Rigidbody2D::ShapeType::Circle)
					Rigidbody2D_ReCalculCircleColliderAABB(*body1);
			}

			if (body2->UpdateRequired)
			{
				if (body2->Shape == Rigidbody2D::ShapeType::Box)
					Rigidbody2D_ReCalculBoxColliderVerticesAndAABB(*body2);
				else if (body2->Shape == Rigidbody2D::ShapeType::Circle)
					Rigidbody2D_ReCalculCircleColliderAABB(*body2);
			}

			if (!Collisions_IntersectAABB(body1->AABB, body2->AABB))
			{
				continue;
			}

			ContactPair* contactPair = (ContactPair*)List_Get(world.ContactPairs, world.ContactPairCount);
			contactPair->Body1 = i;
			contactPair->Body2 = j;
			world.ContactPairCount++;
		}
	}
}
#ifndef CORE_DIST
#include "Renderer/Renderer2D.h"
void ContactPointVisualiztion(const Vec2* contactPoint, uint32_t contactPointCount)
{
	for (size_t i = 0; i < contactPointCount; i++)
	{
		Vec3 pos = { contactPoint[i].x,contactPoint[i].y,-0.1f };
		Vec3 rot = { 0,0,0 };
		Vec3 scale = { 0.5f,0.5f,1.0f };

		Mat trans = DirectX::XMMatrixScaling(scale.x, scale.y, scale.z)
			* DirectX::XMMatrixRotationQuaternion(DirectX::XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rot)))
			* DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);

		Renderer2D_DrawQuad(trans, { 1.0f,0,0,1.0f });
	}
}

void RayVisualiztion(const Vec2& rayOrigin, const Vec2& rayDirection, float maxDistance = 100.0f)
{
	Vec3 p0 = { rayOrigin.x,rayOrigin.y,0.0f };
	Vec3 p1 = { rayOrigin.x + rayDirection.x * maxDistance ,rayOrigin.y + rayDirection.y * maxDistance,0.0f };
	Renderer2D_DrawLine(p0, p1, { 1.0f,0,0,1.0f });
}
#endif 

void NarrowPhase(PhysicsWorld2D& world)
{
	for (size_t i = 0; i < world.ContactPairCount; i++)
	{
		ContactPair* contactPair = (ContactPair*)List_Get(world.ContactPairs, i);
		Rigidbody2D* body1 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, contactPair->Body1);
		Rigidbody2D* body2 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, contactPair->Body2);

		float depth = FLT_MAX;
		Vec2 normal = Vec2Zero;
		Vec2 contactPoint[2] = { Vec2Zero,Vec2Zero };
		uint32_t contactPointCount = 0;
		if (Collide(*body1, *body2, &normal, &depth, contactPoint, &contactPointCount))
		{

			#ifndef CORE_DIST
			ContactPointVisualiztion(contactPoint, contactPointCount);
			#endif 

			if (body1->IsTrigger || body2->IsTrigger)
			{
				world.CollisionCallback(body1->Entity, body2->Entity);

				continue;
			}

			if (body1->Type == Rigidbody2D::BodyType::Static)
			{
				Rigidbody2D_MovePosition(body2, Vec2MulFloat(normal, depth));
			}
			else if (body2->Type == Rigidbody2D::BodyType::Static)
			{
				Rigidbody2D_MovePosition(body1, Vec2MulFloat(normal, -depth));
			}
			else
			{
				Rigidbody2D_MovePosition(body1, Vec2MulFloat(normal, -depth / 2.0f));
				Rigidbody2D_MovePosition(body2, Vec2MulFloat(normal, depth / 2.0f));
			}

			ResolveCollision(*body1, *body2, normal, depth, contactPoint, contactPointCount);
		}
	}
}

void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep, uint32_t iterations)
{
	for (size_t currentIterations = 0; currentIterations < iterations; currentIterations++)
	{
		// Rigidbody2D Step
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);

				Rigidbody2D_Step(*rigidbody, timeStep / (float)iterations);
			}
		}

		// Collide
		{
			world.ContactPairCount = 0;
			BroadPhase(world);
			NarrowPhase(world);
		}
	}
}

void* PhysicsWorld2D_Raycast(PhysicsWorld2D& world, const Vec2& rayOrigin, const Vec2& rayDirection, float* minDistance)
{
	void* result = nullptr;
	*minDistance = FLT_MAX;

	for (size_t i = 0; i < world.Rigidbody2DCount; i++)
	{
		Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);

		if (!rigidbody->Enabled)
		{
			continue;
		}

		if (rigidbody->UpdateRequired)
		{
			if (rigidbody->Shape == Rigidbody2D::ShapeType::Box)
				Rigidbody2D_ReCalculBoxColliderVerticesAndAABB(*rigidbody);
			else if (rigidbody->Shape == Rigidbody2D::ShapeType::Circle)
				Rigidbody2D_ReCalculCircleColliderAABB(*rigidbody);
		}

		float distance = 0.0f;
		if (rigidbody->Shape == Rigidbody2D::ShapeType::Box)
		{
			Vec2 boxCenter = Vec2Add(rigidbody->Position, rigidbody->BoxCollider.Offset);
			if (Collisions_RayCastPolygons(rayOrigin, rayDirection, rigidbody->BoxCollider.Vertices, 4, boxCenter, &distance))
			{
				if (distance != 0.0f)
				{
					if (distance < *minDistance)
					{
						*minDistance = distance;
						result = rigidbody->Entity;
					}
				}
			}
		}
		else if (rigidbody->Shape == Rigidbody2D::ShapeType::Circle)
		{
			Vec2 circleCenter = Vec2Add(rigidbody->Position, rigidbody->CircleCollider.Offset);
			if (Collisions_RayCastCircles(rayOrigin, rayDirection, circleCenter, rigidbody->CircleCollider.Radius, &distance))
			{
				if (distance != 0.0f)
				{
					if (distance < *minDistance)
					{
						*minDistance = distance;
						result = rigidbody->Entity;
					}
				}
			}
		}
	}

	#ifndef CORE_DIST
	RayVisualiztion(rayOrigin, rayDirection, *minDistance);
	#endif 

	return result;
}