#include "PlayerController.h"

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity& entity)
{
	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	BV_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

	s_Data.Rigidbody2D = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	BV_ASSERT(s_Data.Rigidbody2D, "Entity does not have Rigidbody2DComponent!");
}

void PlayerController_OnUpdate(Entity& entity, float timeStep)
{
	Vec2 force = Vec2(0, 0);
	float rotation = 0.0f;

	static float speed = 20.0f;

	if (Input_GetKey(KeyCode::W))
		force.y += speed;
	if (Input_GetKey(KeyCode::S))
		force.y -= speed;
	if (Input_GetKey(KeyCode::A))
		force.x -= speed;
	if (Input_GetKey(KeyCode::D))
		force.x += speed;

	if (Input_GetKey(KeyCode::Space))
		force.y += speed * 20.0f;

	if (Input_GetKey(KeyCode::Z))
		rotation += 10.0f;
	if (Input_GetKey(KeyCode::C))
		rotation -= 10.0f;

	//Rigidbody2DComponent_ApplyRotation(*s_Data.Rigidbody2D, rotation * timeStep);
	Rigidbody2DComponent_ApplyRotation(*s_Data.Rigidbody2D, rotation);
	//Rigidbody2DComponent_ApplyForce(*s_Data.Rigidbody2D, Vec2MulFloat(force, timeStep));
	Rigidbody2DComponent_ApplyForce(*s_Data.Rigidbody2D, force);
}

void PlayerController_OnDestroy(Entity& entity)
{}

void PlayerController_OnCollision(Entity& entity, Entity& other)
{
	APP_LOG_INFO("OnCollision - ");
	APP_LOG_INFO(other.Tag.Name);
}
