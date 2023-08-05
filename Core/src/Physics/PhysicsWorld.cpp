#include "pch.h"
#include "PhysicsWorld.h"

struct ContactPair
{
	uint32_t Body1 = 0;
	uint32_t Body2 = 0;
};

struct PhysicsWorldData
{
	float Time;
};
static PhysicsWorldData s_Data;

void PhysicsWorld2D_Create(PhysicsWorld2D& world, void(*CollisionCallback)(void* entity1, void* entity2))
{
	world.CollisionCallback = CollisionCallback;

	List_Create(world.Rigidbody2Ds, PHYSICS_POOL_SIZE);
	List_Create(world.ContactPairs, PHYSICS_POOL_SIZE);
	{
		for (size_t i = 0; i < PHYSICS_POOL_SIZE; i++)
		{
			Rigidbody2D* rigidbody2D = (Rigidbody2D*)malloc(sizeof(Rigidbody2D));
			*rigidbody2D = {};
			List_Add(world.Rigidbody2Ds, rigidbody2D);
		}
	}
	{
		for (size_t i = 0; i < PHYSICS_POOL_SIZE; i++)
		{
			ContactPair* contactPair = (ContactPair*)malloc(sizeof(ContactPair));
			*contactPair = {};
			List_Add(world.ContactPairs, contactPair);
		}
	}
	world.Rigidbody2DCount = 0;
	world.ContactPairCount = 0;
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
		if (shape2 == Rigidbody2D::ShapeType::Circle)
		{
			return Collisions_IntersectCircles(body1.Position, body1.CircleCollider.Radius,
				body2.Position, body2.CircleCollider.Radius, normal, depth, contactPoint, contactPointCount);
		}
		else if (shape2 == Rigidbody2D::ShapeType::Box)
		{
			Vec2 box2Center = Vec2Add(body2.Position, body2.BoxCollider.Offset);
			Vec2 circleCenter = Vec2Add(body1.Position, body1.CircleCollider.Offset);
			return Collisions_IntersectCirclePolygon(circleCenter, body1.CircleCollider.Radius,
				body2.BoxCollider.Vertices, 4, box2Center, normal, depth, contactPoint, contactPointCount);
		}
	}

	return false;
}

void ResolveCollision(Rigidbody2D& body1, Rigidbody2D& body2, Vec2& normal, float depth,
	Vec2* contactPoint, uint32_t contactPointCount)
{
	//// Temp
	//float 	InvInertia = 0.1f;

	//BV_ASSERT(contactPointCount <= 2, "Max contact point count is 2");

	//float e = FloatMin(body1.Restitution, body2.Restitution);

	//float StaticFriction = 0.4f;
	//float DynamicFriction = 0.6f;

	////float sf = (body1.StaticFriction + body2.StaticFriction) * 0.5f;
	////float df = (body1.DynamicFriction + body2.DynamicFriction) * 0.5f;
	//float sf = (StaticFriction + StaticFriction) * 0.5f;
	//float df = (DynamicFriction + DynamicFriction) * 0.5f;

	//Vec2 impulseList[2] = { Vec2Zero,Vec2Zero };
	//Vec2 raList[2] = { Vec2Zero,Vec2Zero };
	//Vec2 rbList[2] = { Vec2Zero,Vec2Zero };
	//Vec2 raPerpList[2] = { Vec2Zero,Vec2Zero };
	//Vec2 rbPerpList[2] = { Vec2Zero,Vec2Zero };
	//Vec2 frictionImpulseList[2] = { Vec2Zero,Vec2Zero };
	//float jList[2] = { 0.0f, 0.0f };

	//for (size_t i = 0; i < contactPointCount; i++)
	//{
	//	Vec2 ra = Vec2Sub(contactPoint[i], body1.Position);
	//	Vec2 rb = Vec2Sub(contactPoint[i], body2.Position);

	//	raList[i] = ra;
	//	rbList[i] = rb;

	//	Vec2 raPerp = { -ra.y, ra.x };
	//	Vec2 rbPerp = { -rb.y, rb.x };

	//	raPerpList[i] = raPerp;
	//	rbPerpList[i] = rbPerp;

	//	Vec2 angularLinearVelocityA = Vec2MulFloat(raPerp, body1.AngularVelocity);
	//	Vec2 angularLinearVelocityB = Vec2MulFloat(rbPerp, body2.AngularVelocity);

	//	Vec2 relativeVelocity = Vec2Sub(
	//		Vec2Add(body2.Velocity, angularLinearVelocityB),
	//		Vec2Add(body1.Velocity, angularLinearVelocityA));

	//	float contactVelocityMag = Vec2Dot(relativeVelocity, normal);

	//	if (contactVelocityMag > 0.0f)
	//	{
	//		continue;
	//	}

	//	float raPerpDotN = Vec2Dot(raPerp, normal);
	//	float rbPerpDotN = Vec2Dot(rbPerp, normal);

	//	/*float denom = body1.InvMass + body2.InvMass +
	//		(raPerpDotN * raPerpDotN) * body1.InvInertia +
	//		(rbPerpDotN * rbPerpDotN) * body2.InvInertia;*/

	//	float denom = body1.InvMass + body2.InvMass +
	//		(raPerpDotN * raPerpDotN) * InvInertia +
	//		(rbPerpDotN * rbPerpDotN) * InvInertia;

	//	float j = -(1.0f + e) * contactVelocityMag;
	//	j /= denom;
	//	j /= (float)contactPointCount;

	//	jList[i] = j;

	//	Vec2 impulse = Vec2MulFloat(normal, j);
	//	impulseList[i] = impulse;
	//}

	//for (size_t i = 0; i < contactPointCount; i++)
	//{
	//	Vec2 impulse = impulseList[i];
	//	Vec2 ra = raList[i];
	//	Vec2 rb = rbList[i];

	//	body1.Velocity = Vec2Add(body1.Velocity, Vec2MulFloat(impulse, -body1.InvMass));
	//	/*body1.AngularVelocity += Vec2Cross(ra, impulse) * -body1.InvInertia;*/
	//	body1.AngularVelocity += Vec2Cross(ra, impulse) * -InvInertia;
	//	body2.Velocity = Vec2Add(body2.Velocity, Vec2MulFloat(impulse, body2.InvMass));
	//	//body2.AngularVelocity += Vec2Cross(rb, impulse) * body2.InvInertia;
	//	body2.AngularVelocity += Vec2Cross(rb, impulse) * InvInertia;
	//}

	//for (size_t i = 0; i < contactPointCount; i++)
	//{
	//	Vec2 ra = Vec2Sub(contactPoint[i], body1.Position);
	//	Vec2 rb = Vec2Sub(contactPoint[i], body2.Position);

	//	raList[i] = ra;
	//	rbList[i] = rb;

	//	Vec2 raPerp = { -ra.y, ra.x };
	//	Vec2 rbPerp = { -rb.y, rb.x };

	//	Vec2 angularLinearVelocityA = Vec2MulFloat(raPerp, body1.AngularVelocity);
	//	Vec2 angularLinearVelocityB = Vec2MulFloat(rbPerp, body2.AngularVelocity);

	//	Vec2 relativeVelocity = Vec2Sub(
	//		Vec2Add(body2.Velocity, angularLinearVelocityB),
	//		Vec2Add(body1.Velocity, angularLinearVelocityA));

	//	Vec2 tangent = Vec2Sub(relativeVelocity, Vec2MulFloat(normal, Vec2Dot(relativeVelocity, normal)));

	//	if (Vec2NearlyEqual(tangent, Vec2Zero))
	//	{
	//		continue;
	//	}
	//	else
	//	{
	//		tangent = Vec2Normalize(tangent);
	//	}

	//	float raPerpDotT = Vec2Dot(raPerp, tangent);
	//	float rbPerpDotT = Vec2Dot(rbPerp, tangent);

	//	//float denom = body1.InvMass + body2.InvMass +
	//	//	(raPerpDotT * raPerpDotT) * body1.InvInertia +
	//	//	(rbPerpDotT * rbPerpDotT) * body2.InvInertia;

	//	float denom = body1.InvMass + body2.InvMass +
	//		(raPerpDotT * raPerpDotT) * InvInertia +
	//		(rbPerpDotT * rbPerpDotT) * InvInertia;

	//	float jt = -Vec2Dot(relativeVelocity, tangent);
	//	jt /= denom;
	//	jt /= (float)contactPointCount;

	//	Vec2 frictionImpulse;
	//	float j = jList[i];

	//	if (fabs(jt) <= j * sf)
	//	{
	//		frictionImpulse = Vec2MulFloat(tangent, jt);
	//	}
	//	else
	//	{
	//		frictionImpulse = Vec2MulFloat(Vec2MulFloat(tangent, -j), df);
	//	}

	//	frictionImpulseList[i] = frictionImpulse;
	//}

	//for (size_t i = 0; i < contactPointCount; i++)
	//{
	//	Vec2 frictionImpulse = frictionImpulseList[i];
	//	Vec2 ra = raList[i];
	//	Vec2 rb = rbList[i];

	//	body1.Velocity = Vec2Add(body1.Velocity, Vec2MulFloat(frictionImpulse, -body1.InvMass));
	//	//body1.AngularVelocity += Vec2Cross(ra, frictionImpulse) * -body1.InvInertia;
	//	body1.AngularVelocity += Vec2Cross(ra, frictionImpulse) * -InvInertia;
	//	body2.Velocity = Vec2Add(body2.Velocity, Vec2MulFloat(frictionImpulse, body2.InvMass));
	//	//body2.AngularVelocity += Vec2Cross(rb, frictionImpulse) * body2.InvInertia;
	//	body2.AngularVelocity += Vec2Cross(rb, frictionImpulse) * InvInertia;
	//}

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
		for (size_t j = i + 1; j < world.Rigidbody2DCount; j++)
		{
			Rigidbody2D* body2 = (Rigidbody2D*)List_Get(world.Rigidbody2Ds, j);
			if (body1->Type == Rigidbody2D::BodyType::Static && body2->Type == Rigidbody2D::BodyType::Static)
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
			if (body1->IsTrigger || body2->IsTrigger)
			{
				world.CollisionCallback(body1->Entity, body2->Entity);

				continue;
			}

			if (body1->Type == Rigidbody2D::BodyType::Static || body1->Type == Rigidbody2D::BodyType::Kinematic)
			{
				Rigidbody2D_MovePosition(*body2, Vec2MulFloat(normal, depth / 2.0f));
			}
			else if (body2->Type == Rigidbody2D::BodyType::Static || body2->Type == Rigidbody2D::BodyType::Kinematic)
			{
				Rigidbody2D_MovePosition(*body1, Vec2MulFloat(normal, -depth));
			}
			else
			{
				Rigidbody2D_MovePosition(*body1, Vec2MulFloat(normal, -depth / 2.0f));
				Rigidbody2D_MovePosition(*body2, Vec2MulFloat(normal, depth / 2.0f));
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