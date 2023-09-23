#include "PlayerController.h"

#define WALK_SPEED 25.0f

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

	s_Data.Rigidbody2D = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(s_Data.Rigidbody2D, "Entity does not have Rigidbody2DComponent!");
}

void PlayerController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	//Movement
	{
		Vec2 movement = Vec2(0, 0);

		if (Input_GetKey(KeyCode_W))
			movement.y += WALK_SPEED;
		if (Input_GetKey(KeyCode_S))
			movement.y -= WALK_SPEED;
		if (Input_GetKey(KeyCode_A))
			movement.x -= WALK_SPEED;
		if (Input_GetKey(KeyCode_D))
			movement.x += WALK_SPEED;

		Rigidbody2DComponent_MovePosition(s_Data.Rigidbody2D, Vec2MulFloat(movement, timeStep));

		float rotation = 0.0f;
		if (Input_GetKeyDown(KeyCode_Q))
			rotation -= DirectX::XMConvertToRadians(45);
		if (Input_GetKeyDown(KeyCode_E))
			rotation += DirectX::XMConvertToRadians(45);

		Rigidbody2DCmponent_Rotate(s_Data.Rigidbody2D, rotation);
	}
}

void PlayerController_OnDestroy(Entity* entity, void* runtimeData)
{}

void PlayerController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{
	#ifndef CORE_DIST
	APP_LOG_INFO("OnCollision: %s", other->Tag.Name);
	#endif 
}

void PlayerController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{
	#ifndef CORE_DIST
	APP_LOG_INFO("Player hited by : %s", other->Tag.Name);
	#endif 
}

void PlayerController_OnEnable(Entity* entity, void* runtimeData)
{}
void PlayerController_OnDisable(Entity* entity, void* runtimeData)
{}