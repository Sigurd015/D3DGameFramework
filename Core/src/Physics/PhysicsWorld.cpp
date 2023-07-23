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

bool CollisionBB(Vec2& box1LeftTop, Vec2& box1RightBottom, Vec2& box2LeftTop, Vec2& box2RightBottom)
{
	return box1LeftTop.x < box2RightBottom.x && box1RightBottom.x > box2LeftTop.x &&
		box1LeftTop.y > box2RightBottom.y && box1RightBottom.y < box2LeftTop.y;
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
		return Collisions_CC(body1.Position, body1.CircleCollider.Radius, body2.Position, body2.CircleCollider.Radius, normal, depth);
	}
	else if (body1.Shape == Rigidbody2D::ShapeType::Box && body2.Shape == Rigidbody2D::ShapeType::Box)
	{
		Vec2 box1LeftTop = { body1.Position.x - body1.BoxCollider.Size.x,body1.Position.y + body1.BoxCollider.Size.y };
		Vec2 box1RightBottom = { body1.Position.x + body1.BoxCollider.Size.x,body1.Position.y - body1.BoxCollider.Size.y };

		Vec2 box2LeftTop = { body2.Position.x - body2.BoxCollider.Size.x,body2.Position.y + body2.BoxCollider.Size.y };
		Vec2 box2RightBottom = { body2.Position.x + body2.BoxCollider.Size.x,body2.Position.y - body2.BoxCollider.Size.y };

		//TODO: CollisionBB
		return false;
	}
	else if (body1.Shape == Rigidbody2D::ShapeType::Circle && body2.Shape == Rigidbody2D::ShapeType::Box)
	{
		Vec2 boxLeftTop = { body1.Position.x - body1.BoxCollider.Size.x,body1.Position.y + body1.BoxCollider.Size.y };
		Vec2 boxRightBottom = { body1.Position.x + body1.BoxCollider.Size.x,body1.Position.y - body1.BoxCollider.Size.y };

		//TODO: CollisionBC
		return false;
	}
	return false;
}

void PhysicsWorld2D_Update(PhysicsWorld2D& world, float timeStep)
{
	if (!IsFixedUpdate(timeStep))
		return;

	{
		//TODO: Implement Rigidbody2D Update
		{
			for (size_t i = 0; i < world.Rigidbody2DCount; i++)
			{
				Rigidbody2D* rigidbody = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, i);

				rigidbody->Position = Vec2Add(rigidbody->Position, rigidbody->Velocity);
				rigidbody->Velocity = Vec2(0.0f, 0.0f);
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
						body1->Position = Vec2Add(body1->Position, Vec2MulFloat(normal, -depth / 2.0f));
						body2->Position = Vec2Sub(body2->Position, Vec2MulFloat(normal, depth / 2.0f));
					}
				}
			}
		}
	}
}