#include "pch.h"
#include "Rigidbody.h"

void Rigidbody2D_CreateShape(Rigidbody2D& rigidbody2D, void* shape)
{
	if (rigidbody2D.Shape == Rigidbody2D::ShapeType::Box)
	{
		rigidbody2D.BoxCollider = *(BoxCollider2D*)shape;
	}
	else if (rigidbody2D.Shape == Rigidbody2D::ShapeType::Circle)
	{
		rigidbody2D.CircleCollider = *(CircleCollider2D*)shape;
	}
	else
	{
		BV_ASSERT(false, "Rigidbody2D_CreateShape: Unknown ShapeType");
	}
}

void Rigidbody2D_ApplyForce(void* rigidbody2D, const Vec2& force)
{
	Rigidbody2D* rigidbody = (Rigidbody2D*)rigidbody2D;
	rigidbody->Velocity = Vec2Add(rigidbody->Velocity, force);
}