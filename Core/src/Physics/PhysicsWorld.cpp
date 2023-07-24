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
	if (body1.Shape == Rigidbody2D::ShapeType::Circle && body2.Shape == Rigidbody2D::ShapeType::Circle)
	{
		return IntersectCircles(body1.Position, body1.CircleCollider.Radius, body2.Position, body2.CircleCollider.Radius, normal, depth);
	}
	else if (body1.Shape == Rigidbody2D::ShapeType::Box && body2.Shape == Rigidbody2D::ShapeType::Box)
	{
		if (body1.BoxCollider.VerticesNeedUpdate)
		{
			Rigidbody2D_ReCalculBoxColliderVertices(body1);
		}

		if (body2.BoxCollider.VerticesNeedUpdate)
		{
			Rigidbody2D_ReCalculBoxColliderVertices(body2);
		}

		return IntersectPolygons(body1.BoxCollider.Vertices, 4, body2.BoxCollider.Vertices, 4, normal, depth);
	}
	else if (body1.Shape == Rigidbody2D::ShapeType::Box && body2.Shape == Rigidbody2D::ShapeType::Circle)
	{
		if (body1.BoxCollider.VerticesNeedUpdate)
		{
			Rigidbody2D_ReCalculBoxColliderVertices(body1);
		}

		Vec2 circleCenter = Vec2Add(body2.Position, body2.CircleCollider.Offset);
		return IntersectCirclePolygon(circleCenter, body2.CircleCollider.Radius, body1.BoxCollider.Vertices, 4, normal, depth);
	}
	else if (body1.Shape == Rigidbody2D::ShapeType::Circle && body2.Shape == Rigidbody2D::ShapeType::Box)
	{
		return false;
	}
	return false;
}

void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep)
{
	//if (!IsFixedUpdate(timeStep))
	//	return;

	{
		//TODO: Implement Rigidbody2D Update
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);

				//rigidbody->Position = Vec2Add(rigidbody->Position, rigidbody->Velocity);
				//rigidbody->Velocity = Vec2(0.0f, 0.0f);
			}
		}

		// Collide
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				for (size_t j = i + 1; j < world.Rigidbody2DCount; j++)
				{
					Rigidbody2D* body1 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);
					Rigidbody2D* body2 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, j);

					float depth;
					Vec2 normal;
					if (Collide(*body1, *body2, &normal, &depth))
					{
						world.CollisionCallback(body1->Entity, body2->Entity);

						//Temp
						Rigidbody2D_ApplyForce(body1, Vec2MulFloat(normal, -depth / 2.0f));
						Rigidbody2D_ApplyForce(body2, Vec2MulFloat(normal, depth / 2.0f));
					}
				}
			}
		}
	}
}