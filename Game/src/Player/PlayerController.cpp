#include "PlayerController.h"
#include "../KeyMap/KeyMap.h"
#include "../UI/UIController.h"

struct PlayerStats
{
	float MaxHp = 300.0f;
	float Hp = 300.0f;
};



struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;

	bool CanShoot = true;
	float ShootCooldown = 1.0f;
	float ShootCooldownTimer = 0.0f;

	PlayerStats Stats;
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
	//Debug
	{
		if (Input_GetKeyDown(KeyCode::Numpad0))
		{
			s_Data.Stats.Hp = FloatClamp(s_Data.Stats.Hp + 30.0f, 0.0f, s_Data.Stats.MaxHp);

		}
		if (Input_GetKeyDown(KeyCode::Numpad1))
		{
			s_Data.Stats.Hp = FloatClamp(s_Data.Stats.Hp - 30.0f, 0.0f, s_Data.Stats.MaxHp);
		}
		if (Input_GetKeyDown(KeyCode::Numpad2))
		{
			UIController_PlayHitIcon();
		}
	}

	//Movement
	{
		Vec2 movement = Vec2(0, 0);
		static float speed = 25.0f;

		if (Input_GetKey(KeyMap_GetKey(MOVE_FORWARD)))
			movement.y += speed;
		if (Input_GetKey(KeyMap_GetKey(MOVE_BACKWARD)))
			movement.y -= speed;
		if (Input_GetKey(KeyMap_GetKey(MOVE_LEFT)))
			movement.x -= speed;
		if (Input_GetKey(KeyMap_GetKey(MOVE_RIGHT)))
			movement.x += speed;

		Rigidbody2DComponent_MovePosition(*s_Data.Rigidbody2D, Vec2MulFloat(movement, timeStep));
	}

	//Shooting
	{
		if (Input_GetKeyDown(KeyMap_GetKey(SHOOT)))
		{
			if (s_Data.CanShoot)
			{
				s_Data.CanShoot = false;
				UIController_PlayShootAnimation();
				//TODO: Raycast and damage enemies
			}
		}

		if (!s_Data.CanShoot)
		{
			if (s_Data.ShootCooldownTimer >= s_Data.ShootCooldown)
			{
				s_Data.CanShoot = true;
				s_Data.ShootCooldownTimer = 0.0f;
			}
			else
			{
				s_Data.ShootCooldownTimer += timeStep;
			}
		}		
	}
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
