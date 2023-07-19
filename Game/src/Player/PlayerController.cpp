#include "PlayerController.h"

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity& entity)
{
	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	//s_Data.Transform = (TransformComponent*)Entity_GetComponent(Scene_GetEntityByName(*entity.Scene, "Player"), ComponentType_Transform);
	BV_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");
}

void PlayerController_OnUpdate(Entity& entity, float timeStep)
{
	Vec3 position = s_Data.Transform->Translation;

	static float speed = 0.03f;

	if (Input_IsKeyPressed(KeyCode::W))
		position.y -= speed;
	if (Input_IsKeyPressed(KeyCode::S))
		position.y += speed;
	if (Input_IsKeyPressed(KeyCode::A))
		position.x += speed;
	if (Input_IsKeyPressed(KeyCode::D))
		position.x -= speed;

	s_Data.Transform->Translation = position;
}

void PlayerController_OnDestroy(Entity& entity)
{}

void PlayerController_OnCollision(Entity& entity, Entity& other)
{}
