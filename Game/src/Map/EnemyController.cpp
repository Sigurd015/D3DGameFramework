#include "EnemyController.h"

#define WALK_ANIMATION_FRAME_SPEED 0.25f
#define ATTACK_ANIMATION_FRAME_SPEED 0.25f
#define DEATH_ANIMATION_FRAME_SPEED 0.25f
#define PAIN_ANIMATION_FRAME_SPEED 0.3f

void EnemyController_OnCreate(Entity& entity, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;
	data->SpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);
	CORE_ASSERT(data->SpriteRenderer, "Entity does not have SpriteRendererComponent!");

	data->Rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(entity, ComponentType_Rigidbody2D);
	CORE_ASSERT(data->Rigidbody, "Entity does not have Rigidbody2DComponent!");

	switch (data->Type)
	{
	case CACO_DEMON:
	{
		data->WalkSpriteMaxAnimationFrames = 3;
		{
			SpriteAnimatorSpecification spec;
			spec.TextureWidth = Texture2D_GetWidth(*data->WalkSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(*data->WalkSpriteSheet);
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
			spec.TextureWidth = Texture2D_GetWidth(*data->PainSpriteSheet);
			spec.TextureHeight = Texture2D_GetHeight(*data->PainSpriteSheet);
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
		data->AttackSpriteMaxAnimationFrames = 5;

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
		data->WalkSpriteMaxAnimationFrames = 3;
		break;
	}
	case SOLDIER:
	{
		data->WalkSpriteMaxAnimationFrames = 3;
		break;
	}
	}

	data->SpriteRenderer->Texture = data->IdleSpriteSheet;
	data->SpriteRenderer->UVStart = data->IdleUVStart;
	data->SpriteRenderer->UVEnd = data->IdleUVEnd;
}

void EnemyController_OnUpdate(Entity& entity, float timeStep, void* runtimeData)
{
	EnemyData* data = (EnemyData*)runtimeData;
	//Walk animation
	{
		SpriteTimer_Update(data->WalkSpriteTimer, timeStep);
		uint32_t currentFrame = SpriteTimer_GetFrame(data->WalkSpriteTimer);

		data->SpriteRenderer->Texture = data->WalkSpriteSheet;
		SpriteElement* spriteElement = SpriteAnimator_GetElement(data->WalkSpriteAnimator, SpriteTimer_GetFrame(data->WalkSpriteTimer));
		data->SpriteRenderer->UVStart = spriteElement->UVStart;
		data->SpriteRenderer->UVEnd = spriteElement->UVEnd;
	}

	//Attack animation
	{
		if (data->CanAttack)
		{

		}
	}

	//Hited animation
	{
		if (data->IsHited)
		{
			SpriteTimer_Update(data->PainSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(data->PainSpriteTimer);

			data->SpriteRenderer->Texture = data->PainSpriteSheet;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(data->PainSpriteAnimator, currentFrame);
			data->SpriteRenderer->UVStart = spriteElement->UVStart;
			data->SpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (currentFrame == data->PainSpriteMaxAnimationFrames - 1)
			{
				data->IsHited = false;
				SpriteTimer_Reset(data->PainSpriteTimer);

				data->SpriteRenderer->Texture = data->IdleSpriteSheet;
				data->SpriteRenderer->UVStart = data->IdleUVStart;
				data->SpriteRenderer->UVEnd = data->IdleUVEnd;
			}
		}
	}
}

void EnemyController_OnDestroy(Entity& entity, void* runtimeData)
{}

void EnemyController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}

void EnemyController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{
	if (!strcmp(other.Tag.Name, "Player"))
	{
		EnemyData* data = (EnemyData*)runtimeData;
		data->IsHited = true;

		APP_LOG_INFO(entity.Tag.Name);
		APP_LOG_INFO("Enemy hited by player");
	}
}