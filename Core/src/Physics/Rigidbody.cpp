#include "pch.h"
#include "Rigidbody.h"

void Rigidbody2D_CreateBoxCollider(Rigidbody2D& rigidbody2D, const Vec2& offset, const Vec2& size)
{
	rigidbody2D.Shape = Rigidbody2D::ShapeType::Box;
	rigidbody2D.BoxCollider.Size = size;
	rigidbody2D.BoxCollider.Offset = offset;
	rigidbody2D.UpdateRequired = true;

	rigidbody2D.Mass = (size.x * 2) * (size.y * 2) * rigidbody2D.Density;
}

void Rigidbody2D_ReCalculBoxColliderVerticesAndAABB(Rigidbody2D& rigidbody2D)
{
	Vec2 position = Vec2Add(rigidbody2D.Position, rigidbody2D.BoxCollider.Offset);
	Vec2 boxLeftTop = { position.x - rigidbody2D.BoxCollider.Size.x,position.y + rigidbody2D.BoxCollider.Size.y };
	Vec2 boxRightBottom = { position.x + rigidbody2D.BoxCollider.Size.x,position.y - rigidbody2D.BoxCollider.Size.y };

	rigidbody2D.BoxCollider.Vertices[0] = { boxLeftTop.x, boxLeftTop.y };
	rigidbody2D.BoxCollider.Vertices[1] = { boxRightBottom.x, boxLeftTop.y };
	rigidbody2D.BoxCollider.Vertices[2] = { boxRightBottom.x, boxRightBottom.y };
	rigidbody2D.BoxCollider.Vertices[3] = { boxLeftTop.x, boxRightBottom.y };

	// Rotate by pivot point(position)
	Mat translationToPivot = DirectX::XMMatrixTranslation(position.x, position.y, 0.0f);
	Mat rotation = DirectX::XMMatrixRotationZ(rigidbody2D.Rotation);
	Mat translationBack = DirectX::XMMatrixTranslation(-position.x, -position.y, 0.0f);
	Mat finalTransform = translationBack * rotation * translationToPivot;

	for (size_t i = 0; i < 4; i++)
	{
		rigidbody2D.BoxCollider.Vertices[i] = Vec2MulMat(rigidbody2D.BoxCollider.Vertices[i], finalTransform);

		if (rigidbody2D.BoxCollider.Vertices[i].x < rigidbody2D.AABB.Min.x)
		{
			rigidbody2D.AABB.Min.x = rigidbody2D.BoxCollider.Vertices[i].x;
		}

		if (rigidbody2D.BoxCollider.Vertices[i].x > rigidbody2D.AABB.Max.x)
		{
			rigidbody2D.AABB.Max.x = rigidbody2D.BoxCollider.Vertices[i].x;
		}

		if (rigidbody2D.BoxCollider.Vertices[i].y < rigidbody2D.AABB.Min.y)
		{
			rigidbody2D.AABB.Min.y = rigidbody2D.BoxCollider.Vertices[i].y;
		}

		if (rigidbody2D.BoxCollider.Vertices[i].y > rigidbody2D.AABB.Max.y)
		{
			rigidbody2D.AABB.Max.y = rigidbody2D.BoxCollider.Vertices[i].y;
		}
	}

	rigidbody2D.UpdateRequired = false;
}

void Rigidbody2D_CreateCircleCollider(Rigidbody2D& rigidbody2D, const Vec2& offset, float radius)
{
	rigidbody2D.Shape = Rigidbody2D::ShapeType::Circle;
	rigidbody2D.CircleCollider.Radius = radius;
	rigidbody2D.CircleCollider.Offset = offset;

	rigidbody2D.Mass = (radius * radius) * 3.14f * rigidbody2D.Density;
	rigidbody2D.UpdateRequired = true;
}

void Rigidbody2D_ReCalculCircleColliderAABB(Rigidbody2D& rigidbody2D)
{
	Vec2 position = Vec2Add(rigidbody2D.Position, rigidbody2D.CircleCollider.Offset);
	rigidbody2D.AABB.Min.x = position.x - rigidbody2D.CircleCollider.Radius;
	rigidbody2D.AABB.Min.y = position.y - rigidbody2D.CircleCollider.Radius;
	rigidbody2D.AABB.Max.x = position.x + rigidbody2D.CircleCollider.Radius;
	rigidbody2D.AABB.Max.y = position.y + rigidbody2D.CircleCollider.Radius;
}

void Rigidbody2D_ApplyForce(void* rigidbody2D, const Vec2& force)
{
	Rigidbody2D* rigidbody = (Rigidbody2D*)rigidbody2D;
	rigidbody->Force = Vec2Add(rigidbody->Force, force);
	rigidbody->UpdateRequired = true;
}

void Rigidbody2D_ApplyRotation(void* rigidbody2D, float rotation)
{
	Rigidbody2D* rigidbody = (Rigidbody2D*)rigidbody2D;
	rigidbody->Torque += rotation;
	rigidbody->UpdateRequired = true;
}

void Rigidbody2D_MovePosition(void* rigidbody2D, const Vec2& amount)
{
	Rigidbody2D* rigidbody = (Rigidbody2D*)rigidbody2D;
	rigidbody->Position = Vec2Add(rigidbody->Position, amount);
	rigidbody->UpdateRequired = true;
}

void Rigidbody2D_Step(Rigidbody2D& rigidbody2D, float timeStep)
{
	if (rigidbody2D.Type == Rigidbody2D::BodyType::Static)
		return;

	// Integrate velocity

	if (rigidbody2D.Type == Rigidbody2D::BodyType::Dynamic)
	{
		Vec2 gravity = { 0.0f, -9.8f };
		gravity.y *= rigidbody2D.GravityScale;
		gravity = Vec2MulFloat(gravity, timeStep);
		rigidbody2D.Velocity = Vec2Add(rigidbody2D.Velocity, gravity);
	}

	Vec2 acceleration = Vec2DivFloat(rigidbody2D.Force, rigidbody2D.Mass);
	rigidbody2D.Velocity = Vec2Add(rigidbody2D.Velocity, Vec2MulFloat(acceleration, timeStep));

	rigidbody2D.AngularVelocity += rigidbody2D.Torque * timeStep;

	// Integrate position
	rigidbody2D.Position = Vec2Add(rigidbody2D.Position, Vec2MulFloat(rigidbody2D.Velocity, timeStep));
	rigidbody2D.Rotation += rigidbody2D.AngularVelocity * timeStep;

	// Clear forces
	rigidbody2D.Force = Vec2Zero;
	rigidbody2D.Torque = 0.0f;
	rigidbody2D.UpdateRequired = true;
}