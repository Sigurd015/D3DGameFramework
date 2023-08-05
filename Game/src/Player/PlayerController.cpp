#include "PlayerController.h"

struct PlayerStats
{
	float MaxHp = 300.0f;
	float Hp = 300.0f;
};

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;

	PlayerStats Stats = {};
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity& entity)
{
	s_Data = {};

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

	s_Data.Rigidbody2D = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(s_Data.Rigidbody2D, "Entity does not have Rigidbody2DComponent!");
}

void PlayerController_OnUpdate(Entity& entity, float timeStep)
{
	if (Input_GetKeyDown(KeyCode::O))
	{
		s_Data.Stats.Hp = FloatClamp(s_Data.Stats.Hp + 30.0f, 0.0f, s_Data.Stats.MaxHp);

	}
	if (Input_GetKeyDown(KeyCode::P))
	{
		s_Data.Stats.Hp = FloatClamp(s_Data.Stats.Hp - 30.0f, 0.0f, s_Data.Stats.MaxHp);
	}

	Vec2 force = Vec2(0, 0);
	static float speed = 5.0f;

	if (Input_GetKey(KeyCode::W))
		force.y += speed;
	if (Input_GetKey(KeyCode::S))
		force.y -= speed;
	if (Input_GetKey(KeyCode::A))
		force.x -= speed;
	if (Input_GetKey(KeyCode::D))
		force.x += speed;

	Rigidbody2DComponent_ApplyForce(*s_Data.Rigidbody2D, Vec2MulFloat(force, timeStep));
}

void PlayerController_OnDestroy(Entity& entity)
{}

void PlayerController_OnCollision(Entity& entity, Entity& other)
{
	APP_LOG_INFO("OnCollision - ");
	APP_LOG_INFO(other.Tag.Name);
}

float PlayerController_GetHpPercent()
{
	return s_Data.Stats.Hp / s_Data.Stats.MaxHp;
}
