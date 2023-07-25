#include "pch.h"
#include "PhysicsWorld.h"

struct PhysicsWorldData
{
	float Time;
};
static PhysicsWorldData s_Data;

void PhysicsWorld2D_Create(PhysicsWorld2D& world, void(*CollisionCallback)(void* entity1, void* entity2))
{
	world.CollisionCallback = CollisionCallback;

	List_Create(world.Rigidbody2Ds, PHYSICS_POOL_SIZE);
	{
		for (size_t i = 0; i < PHYSICS_POOL_SIZE; i++)
		{
			Rigidbody2D* rigidbody2D = (Rigidbody2D*)malloc(sizeof(Rigidbody2D));
			*rigidbody2D = {};
			List_Add(world.Rigidbody2Ds, rigidbody2D);
		}
	}
	world.Rigidbody2DCount = 0;
}

void PhysicsWorld2D_Clear(PhysicsWorld2D& world)
{
	world.Rigidbody2DCount = 0;
}

void PhysicsWorld2D_Destory(PhysicsWorld2D& world)
{
	List_Free(world.Rigidbody2Ds, true);
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

	BV_ASSERT(world.Rigidbody2DCount < PHYSICS_POOL_SIZE, "PhysicsWorld2D_AddRigidbody2D: Rigidbody2D Pool is full");

	return rigidbody;
}

bool IsFixedUpdate(float timeStep)
{
	s_Data.Time += timeStep;

	if (s_Data.Time < PHYSICS_TIME_STEP)
		return false;
	else
	{
		s_Data.Time = 0.0f;
		return true;
	}
}

bool Collide(Rigidbody2D& body1, Rigidbody2D& body2, Vec2* normal, float* depth)
{
	Rigidbody2D::ShapeType shape1 = body1.Shape;
	Rigidbody2D::ShapeType shape2 = body2.Shape;

	if (shape1 == Rigidbody2D::ShapeType::Box)
	{
		if (body1.BoxCollider.VerticesNeedUpdate)
		{
			Rigidbody2D_ReCalculBoxColliderVertices(body1);
		}

		Vec2 box1Center = Vec2Add(body1.Position, body1.BoxCollider.Offset);

		if (shape2 == Rigidbody2D::ShapeType::Circle)
		{
			Vec2 circleCenter = Vec2Add(body2.Position, body2.CircleCollider.Offset);
			bool result = Collisions_IntersectCirclePolygon(circleCenter, body2.CircleCollider.Radius,
				body1.BoxCollider.Vertices, 4, box1Center, normal, depth);
			*normal = Vec2MulFloat(*normal, -1);
			return result;
		}
		else if (shape2 == Rigidbody2D::ShapeType::Box)
		{
			if (body2.BoxCollider.VerticesNeedUpdate)
			{
				Rigidbody2D_ReCalculBoxColliderVertices(body2);
			}

			Vec2 box2Center = Vec2Add(body2.Position, body2.BoxCollider.Offset);

			return Collisions_IntersectPolygons(body1.BoxCollider.Vertices, 4, box1Center,
				body2.BoxCollider.Vertices, 4, box2Center, normal, depth);
		}
	}
	else if (shape1 == Rigidbody2D::ShapeType::Circle)
	{
		if (shape2 == Rigidbody2D::ShapeType::Circle)
		{
			return Collisions_IntersectCircles(body1.Position, body1.CircleCollider.Radius, body2.Position, body2.CircleCollider.Radius, normal, depth);
		}
		else if (shape2 == Rigidbody2D::ShapeType::Box)
		{
			if (body2.BoxCollider.VerticesNeedUpdate)
			{
				Rigidbody2D_ReCalculBoxColliderVertices(body2);
			}

			Vec2 box2Center = Vec2Add(body2.Position, body2.BoxCollider.Offset);
			Vec2 circleCenter = Vec2Add(body1.Position, body1.CircleCollider.Offset);

			return Collisions_IntersectCirclePolygon(circleCenter, body1.CircleCollider.Radius,
				body2.BoxCollider.Vertices, 4, box2Center, normal, depth);
		}
	}

	return false;
}

void ResolveCollision(Rigidbody2D& body1, Rigidbody2D& body2, Vec2& normal, float depth)
{
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

void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep, uint32_t iterations)
{
	//if (!IsFixedUpdate(timeStep))
	//	return;

	for (size_t i = 0; i < iterations; i++)
	{
		//TODO: Implement Rigidbody2D Update
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);

				Rigidbody2D_Step(*rigidbody, timeStep / (float)iterations);
			}
		}

		// Collide
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				Rigidbody2D* body1 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);
				for (size_t j = i + 1; j < world.Rigidbody2DCount; j++)
				{
					Rigidbody2D* body2 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, j);

					if (body1->Type == Rigidbody2D::BodyType::Static && body2->Type == Rigidbody2D::BodyType::Static)
					{
						continue;
					}

					float depth;
					Vec2 normal;
					if (Collide(*body1, *body2, &normal, &depth))
					{
						world.CollisionCallback(body1->Entity, body2->Entity);

						if (body1->Type == Rigidbody2D::BodyType::Static)
						{
							Rigidbody2D_MovePosition(*body2, Vec2MulFloat(normal, depth / 2.0f));
						}
						else if (body2->Type == Rigidbody2D::BodyType::Static)
						{
							Rigidbody2D_MovePosition(*body1, Vec2MulFloat(normal, -depth));
						}
						else
						{
							Rigidbody2D_MovePosition(*body1, Vec2MulFloat(normal, -depth / 2.0f));
							Rigidbody2D_MovePosition(*body2, Vec2MulFloat(normal, depth / 2.0f));
						}

						ResolveCollision(*body1, *body2, normal, depth);
					}
				}
			}
		}
	}
}