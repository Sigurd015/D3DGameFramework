#include "EnemyController.h"
#include "FieldController.h"
#include "UI/UIController.h"

#define WALK_ANIMATION_FRAME_SPEED 0.25f
#define ATTACK_ANIMATION_FRAME_SPEED 0.25f
#define DEATH_ANIMATION_FRAME_SPEED 0.25f
#define PAIN_ANIMATION_FRAME_SPEED 0.3f

#define PLAYER_DAMAGE 10.0f

#define CACO_DEMON_SCORE 100
#define CACO_DEMON_WALK_SPEED 3.0f
#define CACO_DEMON_ATTACK_DISTANCE 5.0f
#define CACO_DEMON_ATTACK_RANGE 5.0f
#define CACO_DEMON_MAX_HP 30.0f
#define CACO_DEMON_ATTACK_COOLDOWN_TIME 5.0f

#define CYBER_DEMON_SCORE 300
#define CYBER_DEMON_WALK_SPEED 1.0f
#define CYBER_DEMON_ATTACK_DISTANCE 15.0f
#define CYBER_DEMON_ATTACK_RANGE 20.0f
#define CYBER_DEMON_MAX_HP 50.0f
#define CYBER_DEMON_ATTACK_COOLDOWN_TIME 3.0f

#define SOLDIER_SCORE 200
#define SOLDIER_WALK_SPEED 1.0f
#define SOLDIER_ATTACK_DISTANCE 15.0f
#define SOLDIER_ATTACK_RANGE 20.0f
#define SOLDIER_MAX_HP 50.0f
#define SOLDIER_ATTACK_COOLDOWN_TIME 3.0f

#define ANGLE_OFFSET DirectX::XMConvertToRadians(90.0f)

void EnemyController_OnCreate(Entity* entity, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;

	Entity* Player = Scene_GetEntityByName(entity->Scene, "Player");
	CORE_ASSERT(Player, "Cannot find Player entity!");

	if (Player)
		data->PlayerTransform = (TransformComponent*)Entity_GetComponent(Player, ComponentType_Transform);
	CORE_ASSERT(data->PlayerTransform, "Entity does not have TransformComponent!");

	data->Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(data->Transform, "Entity does not have TransformComponent!");

	data->SpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);
	CORE_ASSERT(data->SpriteRenderer, "Entity does not have SpriteRendererComponent!");

	data->Rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(data->Rigidbody, "Entity does not have Rigidbody2DComponent!");

	data->Audio = (AudioComponent*)Entity_GetComponent(entity, ComponentType_Audio);
	CORE_ASSERT(data->Audio, "Entity does not have AudioComponent!");


	switch (data->Type)
	{
	case CACO_DEMON:
	{
		data->AttackSoundEffect = AssetManager_GetAsset("assets/sounds/caco_demon/cacodemon_attack.wav");
		data->PainSoundEffect = AssetManager_GetAsset("assets/sounds/caco_demon/cacodemon_pain.wav");
		data->DeathSoundEffect = AssetManager_GetAsset("assets/sounds/caco_demon/cacodemon_death.wav");

		data->Score = CACO_DEMON_SCORE;
		data->Stats.MaxHP = CACO_DEMON_MAX_HP;
		data->Stats.HP = data->Stats.MaxHP;
		data->AttackCooldown = CACO_DEMON_ATTACK_COOLDOWN_TIME;
		data->AttackDistance = CACO_DEMON_ATTACK_DISTANCE;
		data->WalkSpeed = CACO_DEMON_WALK_SPEED;
		data->AttackRange = CACO_DEMON_ATTACK_RANGE;

		data->WalkSpriteMaxAnimationFrames = 3;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->WalkSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->WalkSpriteSheet);
			spec.ElementsCount = 3;
			spec.ElementsPerColumn = 3;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 122;
			spec.ElementHeight = 132;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->WalkSpriteAnimator, spec);

			SpriteTimer_Create(data->WalkSpriteTimer, data->WalkSpriteMaxAnimationFrames, WALK_ANIMATION_FRAME_SPEED);
		}
		data->PainSpriteMaxAnimationFrames = 2;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->PainSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->PainSpriteSheet);
			spec.ElementsCount = 2;
			spec.ElementsPerColumn = 2;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 126;
			spec.ElementHeight = 134;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->PainSpriteAnimator, spec);

			SpriteTimer_Create(data->PainSpriteTimer, data->PainSpriteMaxAnimationFrames, PAIN_ANIMATION_FRAME_SPEED);
		}
		data->DeathSpriteMaxAnimationFrames = 6;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->DeathSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->DeathSpriteSheet);
			spec.ElementsCount = 6;
			spec.ElementsPerColumn = 6;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 126;
			spec.ElementHeight = 134;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->DeathSpriteAnimator, spec);

			SpriteTimer_Create(data->DeathSpriteTimer, data->DeathSpriteMaxAnimationFrames, DEATH_ANIMATION_FRAME_SPEED);
		}
		data->AttackSpriteMaxAnimationFrames = 5;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->AttackSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->AttackSpriteSheet);
			spec.ElementsCount = 5;
			spec.ElementsPerColumn = 5;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 126;
			spec.ElementHeight = 130;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->AttackSpriteAnimator, spec);

			SpriteTimer_Create(data->AttackSpriteTimer, data->AttackSpriteMaxAnimationFrames, ATTACK_ANIMATION_FRAME_SPEED);
		}

		//Idle animation
		{
			data->IdleSpriteSheet = data->WalkSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->WalkSpriteAnimator, 1);
			data->IdleUVStart = spriteElement->UVStart;
			data->IdleUVEnd = spriteElement->UVEnd;

		}
		break;
	}
	case CYBER_DEMON:
	{
		data->AttackSoundEffect = AssetManager_GetAsset("assets/sounds/cyber_demon/cyberdemon_attack.wav");
		data->PainSoundEffect = AssetManager_GetAsset("assets/sounds/cyber_demon/cyberdemon_pain.wav");
		data->DeathSoundEffect = AssetManager_GetAsset("assets/sounds/cyber_demon/cyberdemon_death.wav");
		data->WalkSoundEffect = AssetManager_GetAsset("assets/sounds/cyber_demon/cyberdemon_walk.wav");

		data->Score = CYBER_DEMON_SCORE;
		data->Stats.MaxHP = CYBER_DEMON_MAX_HP;
		data->Stats.HP = data->Stats.MaxHP;
		data->AttackCooldown = CYBER_DEMON_ATTACK_COOLDOWN_TIME;
		data->AttackDistance = CYBER_DEMON_ATTACK_DISTANCE;
		data->WalkSpeed = CYBER_DEMON_WALK_SPEED;
		data->AttackRange = CYBER_DEMON_ATTACK_RANGE;

		data->WalkSpriteMaxAnimationFrames = 4;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->WalkSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->WalkSpriteSheet);
			spec.ElementsCount = 4;
			spec.ElementsPerColumn = 4;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 170;
			spec.ElementHeight = 218;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->WalkSpriteAnimator, spec);

			SpriteTimer_Create(data->WalkSpriteTimer, data->WalkSpriteMaxAnimationFrames, WALK_ANIMATION_FRAME_SPEED);
		}
		data->PainSpriteMaxAnimationFrames = 2;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->PainSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->PainSpriteSheet);
			spec.ElementsCount = 2;
			spec.ElementsPerColumn = 2;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 222;
			spec.ElementHeight = 226;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->PainSpriteAnimator, spec);

			SpriteTimer_Create(data->PainSpriteTimer, data->PainSpriteMaxAnimationFrames, PAIN_ANIMATION_FRAME_SPEED);
		}
		data->DeathSpriteMaxAnimationFrames = 9;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->DeathSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->DeathSpriteSheet);
			spec.ElementsCount = 9;
			spec.ElementsPerColumn = 9;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 282;
			spec.ElementHeight = 262;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->DeathSpriteAnimator, spec);

			SpriteTimer_Create(data->DeathSpriteTimer, data->DeathSpriteMaxAnimationFrames, DEATH_ANIMATION_FRAME_SPEED);
		}
		data->AttackSpriteMaxAnimationFrames = 2;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->AttackSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->AttackSpriteSheet);
			spec.ElementsCount = 2;
			spec.ElementsPerColumn = 2;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 198;
			spec.ElementHeight = 220;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->AttackSpriteAnimator, spec);

			SpriteTimer_Create(data->AttackSpriteTimer, data->AttackSpriteMaxAnimationFrames, ATTACK_ANIMATION_FRAME_SPEED);
		}

		//Idle animation
		{
			data->IdleSpriteSheet = data->WalkSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->WalkSpriteAnimator, 0);
			data->IdleUVStart = spriteElement->UVStart;
			data->IdleUVEnd = spriteElement->UVEnd;

		}
		break;
	}
	case SOLDIER:
	{
		data->AttackSoundEffect = AssetManager_GetAsset("assets/sounds/soldier/soldier_attack.wav");
		data->PainSoundEffect = AssetManager_GetAsset("assets/sounds/soldier/soldier_pain.wav");
		data->DeathSoundEffect = AssetManager_GetAsset("assets/sounds/soldier/soldier_death.wav");

		data->Score = SOLDIER_SCORE;
		data->Stats.MaxHP = SOLDIER_MAX_HP;
		data->Stats.HP = data->Stats.MaxHP;
		data->AttackCooldown = SOLDIER_ATTACK_COOLDOWN_TIME;
		data->AttackDistance = SOLDIER_ATTACK_DISTANCE;
		data->WalkSpeed = SOLDIER_WALK_SPEED;
		data->AttackRange = SOLDIER_ATTACK_RANGE;

		data->WalkSpriteMaxAnimationFrames = 4;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->WalkSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->WalkSpriteSheet);
			spec.ElementsCount = 4;
			spec.ElementsPerColumn = 4;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 80;
			spec.ElementHeight = 110;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->WalkSpriteAnimator, spec);

			SpriteTimer_Create(data->WalkSpriteTimer, data->WalkSpriteMaxAnimationFrames, WALK_ANIMATION_FRAME_SPEED);
		}
		data->PainSpriteMaxAnimationFrames = 1;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->PainSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->PainSpriteSheet);
			spec.ElementsCount = 1;
			spec.ElementsPerColumn = 1;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 78;
			spec.ElementHeight = 110;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->PainSpriteAnimator, spec);

			SpriteTimer_Create(data->PainSpriteTimer, data->PainSpriteMaxAnimationFrames, PAIN_ANIMATION_FRAME_SPEED);
		}
		data->DeathSpriteMaxAnimationFrames = 9;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->DeathSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->DeathSpriteSheet);
			spec.ElementsCount = 9;
			spec.ElementsPerColumn = 9;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 114;
			spec.ElementHeight = 118;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->DeathSpriteAnimator, spec);

			SpriteTimer_Create(data->DeathSpriteTimer, data->DeathSpriteMaxAnimationFrames, DEATH_ANIMATION_FRAME_SPEED);
		}
		data->AttackSpriteMaxAnimationFrames = 2;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(data->AttackSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(data->AttackSpriteSheet);
			spec.ElementsCount = 2;
			spec.ElementsPerColumn = 2;
			spec.ElementsPerRow = 1;
			spec.ElementWidth = 54;
			spec.ElementHeight = 110;
			spec.StartElement = { 0,0 };

			SpriteAnimator_Create(data->AttackSpriteAnimator, spec);

			SpriteTimer_Create(data->AttackSpriteTimer, data->AttackSpriteMaxAnimationFrames, ATTACK_ANIMATION_FRAME_SPEED);
		}

		//Idle animation
		{
			data->IdleSpriteSheet = data->WalkSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->WalkSpriteAnimator, 2);
			data->IdleUVStart = spriteElement->UVStart;
			data->IdleUVEnd = spriteElement->UVEnd;

		}
		break;
	}
	}

	data->SpriteRenderer->Texture = data->IdleSpriteSheet;
	data->SpriteRenderer->UVStart = data->IdleUVStart;
	data->SpriteRenderer->UVEnd = data->IdleUVEnd;

	data->State = ENEMY_WALK;
}

void EnemyController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;

	switch (data->State)
	{
	case ENEMY_IDLE:
	{
		break;
	}
	case ENEMY_WALK:
	{
		//Follow player and attack
		{
			Vec2 playerPos = { data->PlayerTransform->Translation.x, data->PlayerTransform->Translation.y };
			Vec2 enemyPos = { data->Transform->Translation.x, data->Transform->Translation.y };
			Vec2 dir = Vec2Sub(playerPos, enemyPos);

			if (Vec2Length(dir) < data->AttackDistance)
			{
				if (data->CanAttack)
				{
					data->State = ENEMY_ATTACK;
					break;
				}
			}

			float angle = atan2f(dir.y, dir.x);
			data->Forward = { cosf(angle), sinf(angle) };
			Vec2 temp = Vec2MulFloat(data->Forward, data->WalkSpeed);

			Rigidbody2DComponent_MovePosition(data->Rigidbody, Vec2MulFloat(temp, timeStep));
			angle += ANGLE_OFFSET;
			Rigidbody2DComponent_SetRotation(data->Rigidbody, angle);

			if (data->WalkSoundEffect)
				AudioComponent_Play(entity->Scene, data->Audio, data->WalkSoundEffect);
		}

		//Walk animation
		{
			SpriteTimer_Update(data->WalkSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(data->WalkSpriteTimer);

			data->SpriteRenderer->Texture = data->WalkSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->WalkSpriteAnimator, SpriteTimer_GetFrame(data->WalkSpriteTimer));
			data->SpriteRenderer->UVStart = spriteElement->UVStart;
			data->SpriteRenderer->UVEnd = spriteElement->UVEnd;
			break;
		}
	}
	case ENEMY_ATTACK:
	{
		//Attack animation
		{
			SpriteTimer_Update(data->AttackSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(data->AttackSpriteTimer);

			data->SpriteRenderer->Texture = data->AttackSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->AttackSpriteAnimator, currentFrame);
			data->SpriteRenderer->UVStart = spriteElement->UVStart;
			data->SpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (currentFrame == data->AttackSpriteMaxAnimationFrames - 1)
			{
				AudioComponent_Play(entity->Scene, data->Audio, data->AttackSoundEffect);
				if (Scene_Raycast(entity->Scene, entity, data->Forward, "Player", data->AttackRange))
				{
					data->CanAttack = false;
				}

				data->State = ENEMY_WALK;
				SpriteTimer_Reset(data->AttackSpriteTimer);
			}
			break;
		}
	}
	case ENEMY_PAIN:
	{
		//Hited animation
		{
			SpriteTimer_Update(data->PainSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(data->PainSpriteTimer);

			data->SpriteRenderer->Texture = data->PainSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->PainSpriteAnimator, currentFrame);
			data->SpriteRenderer->UVStart = spriteElement->UVStart;
			data->SpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (currentFrame == data->PainSpriteMaxAnimationFrames - 1)
			{
				data->State = ENEMY_WALK;
				SpriteTimer_Reset(data->PainSpriteTimer);

				data->SpriteRenderer->Texture = data->IdleSpriteSheet;
				data->SpriteRenderer->UVStart = data->IdleUVStart;
				data->SpriteRenderer->UVEnd = data->IdleUVEnd;
			}
			break;
		}
	}
	case ENEMY_DEATH:
	{
		//Death animation
		{
			SpriteTimer_Update(data->DeathSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(data->DeathSpriteTimer);

			data->SpriteRenderer->Texture = data->DeathSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->DeathSpriteAnimator, currentFrame);
			data->SpriteRenderer->UVStart = spriteElement->UVStart;
			data->SpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (currentFrame == data->DeathSpriteMaxAnimationFrames - 1)
			{
				data->State = ENEMY_IDLE;
				SpriteTimer_Reset(data->DeathSpriteTimer);
				Scene_SetEntityEnabled(entity, false);
				FieldController_OnEnemyDead(data->Transform->Translation);
				UIController_OnEnemyDead(data->Score);
			}
			break;
		}
	}
	}

	if (!data->CanAttack)
	{
		data->AttackTimer += timeStep;

		if (data->AttackTimer >= data->AttackCooldown)
		{
			data->CanAttack = true;
			data->AttackTimer = 0.0f;
		}
	}
}

void EnemyController_OnDestroy(Entity* entity, void* runtimeData)
{}

void EnemyController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{}

void EnemyController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{
	if (String_Compare(other->Tag.Name, "Player"))
	{
		EnemyData* data = (EnemyData*)runtimeData;

		data->Stats.HP -= PLAYER_DAMAGE;

		if (data->Stats.HP <= 0)
		{
			AudioComponent_Play(entity->Scene, data->Audio, data->DeathSoundEffect);
			data->State = ENEMY_DEATH;
		}
		else
		{
			AudioComponent_Play(entity->Scene, data->Audio, data->PainSoundEffect);
			data->State = ENEMY_PAIN;
		}
		//data->State = ENEMY_ATTACK;

		#ifndef CORE_DIST
		APP_LOG_INFO("Enemy hited by player: %s", entity->Tag.Name);
		#endif 
	}
}

void EnemyController_OnEnable(Entity* entity, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;
	data->State = ENEMY_WALK;
	data->Stats.HP = data->Stats.MaxHP;
}

void EnemyController_OnDisable(Entity* entity, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;
	data->State = ENEMY_IDLE;
}