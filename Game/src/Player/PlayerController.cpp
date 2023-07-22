#include "PlayerController.h"

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity& entity)
{
	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	BV_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");
}

void PlayerController_OnUpdate(Entity& entity, float timeStep)
{
	Vec3 position = s_Data.Transform->Translation;

	static float speed = 5.0f;

	if (Input_GetKey(KeyCode::W))
		position.y += speed * timeStep;
	if (Input_GetKey(KeyCode::S))
		position.y -= speed * timeStep;
	if (Input_GetKey(KeyCode::A))
		position.x -= speed * timeStep;
	if (Input_GetKey(KeyCode::D))
		position.x += speed * timeStep;

	s_Data.Transform->Translation = position;
}

void PlayerController_OnDestroy(Entity& entity)
{}

void PlayerController_OnCollision(Entity& entity, Entity& other)
{
	APP_LOG_INFO("OnCollision - ");
	APP_LOG_INFO(other.Tag.Name);
}
