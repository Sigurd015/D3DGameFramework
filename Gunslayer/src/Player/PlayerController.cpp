#include "PlayerController.h"
#include "KeyMap/KeyMap.h"
#include "UI/UIController.h"
#include "GameMode.h"
#include "Camera/CameraController.h"

#define ENEMY_DAMAGE 30.0f
#define ATTACK_RANGE 100.0f
#define WALK_SPEED 25.0f

struct PlayerStats
{
	float MaxHp = 100.0f;
	float Hp = 100.0f;
};

struct PlayerControllerData
{
	TransformComponent* Transform = nullptr;
	Rigidbody2DComponent* Rigidbody2D = nullptr;

	AudioComponent* Audio = nullptr;
	void* ShotgunSoundEffect = nullptr;
	void* PainSoundEffect = nullptr;
	void* DeathSoundEffect = nullptr;

	bool CanShoot = true;
	float ShootCooldown = 1.0f;
	float ShootCooldownTimer = 0.0f;

	PlayerStats Stats;
};
static PlayerControllerData s_Data;

void PlayerController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

	s_Data.Rigidbody2D = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(s_Data.Rigidbody2D, "Entity does not have Rigidbody2DComponent!");

	s_Data.Audio = (AudioComponent*)Entity_GetComponent(entity, ComponentType_Audio);
	CORE_ASSERT(s_Data.Audio, "Entity does not have AudioComponent!");

	s_Data.Stats = {};
	s_Data.Stats.Hp -= 30.0f;

	s_Data.ShotgunSoundEffect = AssetManager_GetAsset("assets/sounds/player_attack.wav");
	s_Data.PainSoundEffect = AssetManager_GetAsset("assets/sounds/player_pain.wav");
	s_Data.DeathSoundEffect = AssetManager_GetAsset("assets/sounds/player_death.wav");
}

void PlayerController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	if (UIController_IsPaused())
		return;

	//Movement
	{
		Vec2 movementForward = Vec2(0, 0);
		Vec2 movementRight = Vec2(0, 0);

		Vec2 forward = CameraController_GetForward();
		Vec2 right = CameraController_GetRight();

		forward = Vec2MulFloat(forward, WALK_SPEED);
		right = Vec2MulFloat(right, WALK_SPEED);

		switch (KeyMap_GetInputDeviceType())
		{
		case InputDeviceType_Keyboard:
		{
			if (KeyMap_GetKey(MOVE_FORWARD))
				movementForward = forward;
			else if (KeyMap_GetKey(MOVE_BACKWARD))
				movementForward = Vec2MulFloat(forward, -1);
			if (KeyMap_GetKey(MOVE_LEFT))
				movementRight = right;
			else if (KeyMap_GetKey(MOVE_RIGHT))
				movementRight = Vec2MulFloat(right, -1);
			break;
		}
		case InputDeviceType_Controller:
		{
			Vec2 joyStick = KeyMap_GetJoyStick(JOY_MOVE);
			movementForward = Vec2MulFloat(forward, joyStick.y);
			movementRight = Vec2MulFloat(right, -joyStick.x);
			break;
		}
		}
		Rigidbody2DComponent_MovePosition(s_Data.Rigidbody2D, Vec2MulFloat(Vec2Add(movementForward, movementRight), timeStep));
	}

	//Shooting
	{
		if (KeyMap_GetKey(SHOOT))
		{
			if (s_Data.CanShoot)
			{
				AudioComponent_Play(entity->Scene, s_Data.Audio, s_Data.ShotgunSoundEffect);
				s_Data.CanShoot = false;
				UIController_PlayShootAnimation();
				if (Scene_Raycast(entity->Scene, entity, CameraController_GetForward(), "Enemy", ATTACK_RANGE))
				{
					UIController_PlayHitIcon();
				}
			}
		}

		if (!s_Data.CanShoot)
		{
			if (s_Data.ShootCooldownTimer > s_Data.ShootCooldown)
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
	if (String_Contains(other->Tag.Name, "Enemy"))
	{

		#ifndef CORE_DIST
		APP_LOG_INFO("Player hited by : %s", other->Tag.Name);
		#endif 

		s_Data.Stats.Hp -= ENEMY_DAMAGE;

		if (s_Data.Stats.Hp <= 0.0f)
		{
			AudioComponent_Play(entity->Scene, s_Data.Audio, s_Data.DeathSoundEffect);
			UIController_OnPlayerDead();
			return;
		}

		AudioComponent_Play(entity->Scene, s_Data.Audio, s_Data.PainSoundEffect);

		UIController_PlayBloodEffect({ 1.0f,0,0 });
	}
}

void PlayerController_OnEnable(Entity* entity, void* runtimeData)
{}
void PlayerController_OnDisable(Entity* entity, void* runtimeData)
{}

float PlayerController_GetHpPercent()
{
	return s_Data.Stats.Hp / s_Data.Stats.MaxHp;
}

void PlayerController_AddHp(float hp)
{
	s_Data.Stats.Hp = FloatClamp(s_Data.Stats.Hp + hp, 0.0f, s_Data.Stats.MaxHp);
	UIController_PlayBloodEffect({ 0,1.0f,0 });
}