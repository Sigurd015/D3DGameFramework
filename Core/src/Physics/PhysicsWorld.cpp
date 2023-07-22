#include "pch.h"
#include "PhysicsWorld.h"

struct PhysicsWorldData
{
	float Time;
};
static PhysicsWorldData s_Data;

void PhysicsWorld2D_Create(PhysicsWorld2D& world)
{
	List_Create(world.BoxCollider2Ds, PHYSICS_COLLIDER_POOL_SIZE);
	List_Create(world.CircleCollider2Ds, PHYSICS_COLLIDER_POOL_SIZE);
	{
		for (size_t i = 0; i < PHYSICS_COLLIDER_POOL_SIZE; i++)
		{
			BoxCollider2D* collider = (BoxCollider2D*)malloc(sizeof(BoxCollider2D));
			*collider = {};
			List_Add(world.BoxCollider2Ds, collider);
		}
	}

	{
		for (size_t i = 0; i < PHYSICS_COLLIDER_POOL_SIZE; i++)
		{
			CircleCollider2D* collider = (CircleCollider2D*)malloc(sizeof(CircleCollider2D));
			*collider = {};
			List_Add(world.CircleCollider2Ds, collider);
		}
	}
	world.BoxCollider2DCount = 0;
	world.CircleCollider2DCount = 0;
}

void PhysicsWorld2D_Clear(PhysicsWorld2D& world)
{
	world.BoxCollider2DCount = 0;
	world.CircleCollider2DCount = 0;
}

void PhysicsWorld2D_Destory(PhysicsWorld2D& world)
{
	List_Free(world.BoxCollider2Ds, true);
	List_Free(world.CircleCollider2Ds, true);
}

bool PhysicsWorld2D_IsFixedUpdate(float timeStep)
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

void PhysicsWorld2D_AddBoxCollider2D(PhysicsWorld2D& world, BoxCollider2D& boxCollider2D)
{
	BoxCollider2D* collider = (BoxCollider2D*)List_Get(world.BoxCollider2Ds, world.BoxCollider2DCount);
	collider->Pos = boxCollider2D.Pos;
	//TODO: Implement Rotation support for BoxCollider2D When we need it
	collider->Angle = boxCollider2D.Angle;
	collider->HalfWidth = boxCollider2D.HalfWidth;
	collider->HalfHeight = boxCollider2D.HalfHeight;
	collider->Entity = boxCollider2D.Entity;

	world.BoxCollider2DCount++;

	BV_ASSERT(world.BoxCollider2DCount < PHYSICS_COLLIDER_POOL_SIZE, "PhysicsWorld2D_AddBoxCollider2D: BoxCollider2D Pool is full");
}

void PhysicsWorld2D_AddCircleCollider2D(PhysicsWorld2D& world, CircleCollider2D& circleCollider2D)
{
	CircleCollider2D* collider = (CircleCollider2D*)List_Get(world.CircleCollider2Ds, world.CircleCollider2DCount);
	collider->Pos = circleCollider2D.Pos;
	collider->Radius = circleCollider2D.Radius;
	collider->Entity = circleCollider2D.Entity;

	world.CircleCollider2DCount++;

	BV_ASSERT(world.CircleCollider2DCount < PHYSICS_COLLIDER_POOL_SIZE, "PhysicsWorld2D_AddCircleCollider2D: CircleCollider2D Pool is full");
}

bool CollisionBB(Vec2& box1LeftTop, Vec2& box1RightBottom, Vec2& box2LeftTop, Vec2& box2RightBottom)
{
	return box1LeftTop.x < box2RightBottom.x && box1RightBottom.x > box2LeftTop.x &&
		box1LeftTop.y > box2RightBottom.y && box1RightBottom.y < box2LeftTop.y;
}

bool CollisionCC(float radius1, float radius2, Vec2& distance)
{
	float len = (radius1 + radius2);
	len *= len;
	return len > Vec2LengthSq(distance);
}

void PhysicsWorld2D_Update(PhysicsWorld2D& world)
{
	// BB Collision
	{
		for (size_t i = 0; i < world.BoxCollider2DCount; i++)
		{
			BoxCollider2D* box1 = (BoxCollider2D*)List_Get(world.BoxCollider2Ds, i);

			Vec2 box1LeftTop = { box1->Pos.x - box1->HalfWidth,box1->Pos.y + box1->HalfHeight };
			Vec2 box1RightBottom = { box1->Pos.x + box1->HalfWidth,box1->Pos.y - box1->HalfHeight };

			for (size_t j = i + 1; j < world.BoxCollider2DCount; j++)
			{
				BoxCollider2D* box2 = (BoxCollider2D*)List_Get(world.BoxCollider2Ds, j);

				Vec2 box2LeftTop = { box2->Pos.x - box2->HalfWidth,box2->Pos.y + box2->HalfHeight };
				Vec2 box2RightBottom = { box2->Pos.x + box2->HalfWidth,box2->Pos.y - box2->HalfHeight };

				if (CollisionBB(box1LeftTop, box1RightBottom, box2LeftTop, box2RightBottom))
				{
					if (Entity_HasComponent(*box1->Entity, ComponentType_Script))
					{
						ScriptComponent* script1 = (ScriptComponent*)Entity_GetComponent(*box1->Entity, ComponentType_Script);
						script1->OnCollision(*box1->Entity, *box2->Entity);
					}

					if (Entity_HasComponent(*box2->Entity, ComponentType_Script))
					{
						ScriptComponent* script2 = (ScriptComponent*)Entity_GetComponent(*box2->Entity, ComponentType_Script);
						script2->OnCollision(*box2->Entity, *box1->Entity);
					}
				}
			}
		}
	}

	// CC Collision
	{
		for (size_t i = 0; i < world.CircleCollider2DCount; i++)
		{
			CircleCollider2D* circle1 = (CircleCollider2D*)List_Get(world.CircleCollider2Ds, i);

			for (size_t j = i + 1; j < world.CircleCollider2DCount; j++)
			{
				CircleCollider2D* circle2 = (CircleCollider2D*)List_Get(world.CircleCollider2Ds, j);

				Vec2 distance = { circle1->Pos.x - circle2->Pos.x,circle1->Pos.y - circle2->Pos.y };

				if (CollisionCC(circle1->Radius, circle2->Radius, distance))
				{
					if (Entity_HasComponent(*circle1->Entity, ComponentType_Script))
					{
						ScriptComponent* script1 = (ScriptComponent*)Entity_GetComponent(*circle1->Entity, ComponentType_Script);
						script1->OnCollision(*circle1->Entity, *circle2->Entity);
					}

					if (Entity_HasComponent(*circle2->Entity, ComponentType_Script))
					{
						ScriptComponent* script2 = (ScriptComponent*)Entity_GetComponent(*circle2->Entity, ComponentType_Script);
						script2->OnCollision(*circle2->Entity, *circle1->Entity);
					}
				}
			}
		}
	}

	// BC Collision
	{
		for (size_t i = 0; i < world.BoxCollider2DCount; i++)
		{
			BoxCollider2D* box = (BoxCollider2D*)List_Get(world.BoxCollider2Ds, i);

			Vec2 boxLeftTop = { box->Pos.x - box->HalfWidth,box->Pos.y + box->HalfHeight };
			Vec2 boxRightBottom = { box->Pos.x + box->HalfWidth,box->Pos.y - box->HalfHeight };

			for (size_t j = 0; j < world.CircleCollider2DCount; j++)
			{
				CircleCollider2D* circle = (CircleCollider2D*)List_Get(world.CircleCollider2Ds, j);

				// Make Circle like a special Box
				Vec2 circleLeftTop = { circle->Pos.x - circle->Radius,circle->Pos.y + circle->Radius };
				Vec2 circleRightBottom = { circle->Pos.x + circle->Radius,circle->Pos.y - circle->Radius };

				if (CollisionBB(boxLeftTop, boxRightBottom, circleLeftTop, circleRightBottom))
				{
					if (Entity_HasComponent(*box->Entity, ComponentType_Script))
					{
						ScriptComponent* script1 = (ScriptComponent*)Entity_GetComponent(*box->Entity, ComponentType_Script);
						script1->OnCollision(*box->Entity, *circle->Entity);
					}

					if (Entity_HasComponent(*circle->Entity, ComponentType_Script))
					{
						ScriptComponent* script2 = (ScriptComponent*)Entity_GetComponent(*circle->Entity, ComponentType_Script);
						script2->OnCollision(*circle->Entity, *box->Entity);
					}
				}
			}
		}
	}
}