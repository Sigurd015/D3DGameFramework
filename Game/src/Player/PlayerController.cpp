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
	Vec2 position = Vec2(0, 0);
	float rotation = 0.0f;

	static float speed = 5.0f;

	if (Input_GetKey(KeyCode::W))
		position.y += speed * timeStep;
	if (Input_GetKey(KeyCode::S))
		position.y -= speed * timeStep;
	if (Input_GetKey(KeyCode::A))
		position.x -= speed * timeStep;
	if (Input_GetKey(KeyCode::D))
		position.x += speed * timeStep;

	if (Input_GetKey(KeyCode::Z))
		rotation += DirectX::XMConvertToRadians(90.0f) * timeStep;
	if (Input_GetKey(KeyCode::C))
		rotation -= DirectX::XMConvertToRadians(90.0f) * timeStep;

	Rigidbody2DComponent_ApplyRotation(*s_Data.Rigidbody2D, rotation);
	Rigidbody2DComponent_ApplyForce(*s_Data.Rigidbody2D, position);
}

void PlayerController_OnDestroy(Entity& entity)
{}

void PlayerController_OnCollision(Entity& entity, Entity& other)
{
	APP_LOG_INFO("OnCollision - ");
	APP_LOG_INFO(other.Tag.Name);
}
