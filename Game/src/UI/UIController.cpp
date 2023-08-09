#include "UIController.h"
#include "../Player/PlayerController.h"

#define BACKBAR_FOLLOW_COUNT_TIME 1.0f
#define SHOOTING_ANIMATION_FRAME_SPEED 0.15f
#define HIT_ICON_HOLD_TIME 0.5f

struct UIControllerData
{
	float BarMaxWidth = 500.0f;

	RectTransformComponent* HPBarFrontRectTrans;
	RectTransformComponent* HPBarBackRectTrans;
	float LastHPBarFrontWidth = BarMaxWidth;
	float HPBarBackFollowCountTime = 0.0;

	SpriteRendererComponent* WeaponSpriteRenderer = nullptr;
	SpriteAnimator WeaponSpriteAnimator;
	SpriteTimer WeaponSpriteTimer;
	uint32_t WeaponSpriteMaxAnimationFrames = 6;
	bool IsShooting = false;

	SpriteRendererComponent* SightIconSpriteRenderer = nullptr;
	SpriteAnimator SightIconSpriteAnimator;
	float HitIconHoldTimer = 0;
	uint32_t SightIconMaxAnimationFrames = 2;
	bool IsHit = false;
};
static UIControllerData s_Data;

void UIController_OnCreate(Entity& entity)
{
	s_Data = {};

	//HP Bar
	{
		Entity* Front = Scene_GetEntityByName(*entity.Scene, "HPBarFront");
		Entity* Back = Scene_GetEntityByName(*entity.Scene, "HPBarBack");

		CORE_ASSERT(Front, "Cannot find HPBarFront entity!");
		CORE_ASSERT(Back, "Cannot find HPBarBack entity!");

		s_Data.HPBarFrontRectTrans = (RectTransformComponent*)Entity_GetComponent(*Front, ComponentType_RectTransform);
		s_Data.HPBarBackRectTrans = (RectTransformComponent*)Entity_GetComponent(*Back, ComponentType_RectTransform);

		CORE_ASSERT(s_Data.HPBarFrontRectTrans, "Entity does not have RectTransformComponent!");
		CORE_ASSERT(s_Data.HPBarBackRectTrans, "Entity does not have RectTransformComponent!");
	}

	//Sight Icon
	{
		Entity* sightIcon = Scene_GetEntityByName(*entity.Scene, "SightIcon");
		CORE_ASSERT(sightIcon, "Cannot find Weapon entity!");

		s_Data.SightIconSpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(*sightIcon, ComponentType_SpriteRenderer);
		CORE_ASSERT(s_Data.SightIconSpriteRenderer, "Entity does not have SpriteRendererComponent!");

		SpriteAnimatorSpecification spec;
		spec.TextureWidth = Texture2D_GetWidth(*s_Data.SightIconSpriteRenderer->Texture);
		spec.TextureHeight = Texture2D_GetHeight(*s_Data.SightIconSpriteRenderer->Texture);
		spec.ElementsCount = 2;
		spec.ElementsPerColumn = 2;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 50;
		spec.ElementHeight = 50;
		spec.StartElement = { 0,0 };

		SpriteAnimator_Create(s_Data.SightIconSpriteAnimator, spec);

		SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SightIconSpriteAnimator, 0);
		s_Data.SightIconSpriteRenderer->UVStart = spriteElement->UVStart;
		s_Data.SightIconSpriteRenderer->UVEnd = spriteElement->UVEnd;
	}

	//Player Animation
	{
		Entity* weapon = Scene_GetEntityByName(*entity.Scene, "Weapon");
		CORE_ASSERT(weapon, "Cannot find Weapon entity!");

		s_Data.WeaponSpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(*weapon, ComponentType_SpriteRenderer);
		CORE_ASSERT(s_Data.WeaponSpriteRenderer, "Entity does not have SpriteRendererComponent!");

		SpriteAnimatorSpecification spec;
		spec.TextureWidth = Texture2D_GetWidth(*s_Data.WeaponSpriteRenderer->Texture);
		spec.TextureHeight = Texture2D_GetHeight(*s_Data.WeaponSpriteRenderer->Texture);
		spec.ElementsCount = 6;
		spec.ElementsPerColumn = 6;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 970;
		spec.ElementHeight = 1050;
		spec.StartElement = { 0,0 };

		SpriteAnimator_Create(s_Data.WeaponSpriteAnimator, spec);

		SpriteTimer_Create(s_Data.WeaponSpriteTimer, s_Data.WeaponSpriteMaxAnimationFrames, SHOOTING_ANIMATION_FRAME_SPEED);

		SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.WeaponSpriteAnimator, 0);
		s_Data.WeaponSpriteRenderer->UVStart = spriteElement->UVStart;
		s_Data.WeaponSpriteRenderer->UVEnd = spriteElement->UVEnd;
	}
}

void UIController_OnUpdate(Entity& entity, float timeStep)
{
	//HP Bar
	{
		float targetHPBarFrontWidth = PlayerController_GetHpPercent() * s_Data.BarMaxWidth;
		s_Data.HPBarFrontRectTrans->Size.x = targetHPBarFrontWidth;
		if (s_Data.LastHPBarFrontWidth > targetHPBarFrontWidth)
		{
			s_Data.HPBarBackFollowCountTime = 0.0f;
		}
		else if (targetHPBarFrontWidth > s_Data.HPBarBackRectTrans->Size.x)
		{
			s_Data.HPBarBackRectTrans->Size.x = targetHPBarFrontWidth;
		}

		if (s_Data.HPBarBackFollowCountTime < BACKBAR_FOLLOW_COUNT_TIME)
		{
			s_Data.HPBarBackFollowCountTime += timeStep;
		}
		else
		{
			s_Data.HPBarBackRectTrans->Size.x = FloatLerp(s_Data.HPBarBackRectTrans->Size.x, s_Data.HPBarFrontRectTrans->Size.x, timeStep);
		}
		s_Data.LastHPBarFrontWidth = targetHPBarFrontWidth;
	}

	//Sight Icon
	{
		if (s_Data.IsHit)
		{
			s_Data.HitIconHoldTimer += timeStep;
			SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SightIconSpriteAnimator, 1);
			s_Data.SightIconSpriteRenderer->UVStart = spriteElement->UVStart;
			s_Data.SightIconSpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (s_Data.HitIconHoldTimer > HIT_ICON_HOLD_TIME)
			{
				s_Data.IsHit = false;
				s_Data.HitIconHoldTimer = 0.0f;

				SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SightIconSpriteAnimator, 0);
				s_Data.SightIconSpriteRenderer->UVStart = spriteElement->UVStart;
				s_Data.SightIconSpriteRenderer->UVEnd = spriteElement->UVEnd;
			}
		}
	}

	//Player Animation
	{
		if (s_Data.IsShooting)
		{
			SpriteTimer_Update(s_Data.WeaponSpriteTimer, timeStep);
			uint32_t currentFrame = SpriteTimer_GetFrame(s_Data.WeaponSpriteTimer);

			SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.WeaponSpriteAnimator, SpriteTimer_GetFrame(s_Data.WeaponSpriteTimer));
			s_Data.WeaponSpriteRenderer->UVStart = spriteElement->UVStart;
			s_Data.WeaponSpriteRenderer->UVEnd = spriteElement->UVEnd;

			if (currentFrame == s_Data.WeaponSpriteMaxAnimationFrames - 1)
			{
				s_Data.IsShooting = false;
				SpriteTimer_Reset(s_Data.WeaponSpriteTimer);

				SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.WeaponSpriteAnimator, 0);
				s_Data.WeaponSpriteRenderer->UVStart = spriteElement->UVStart;
				s_Data.WeaponSpriteRenderer->UVEnd = spriteElement->UVEnd;
			}
		}
	}
}

void UIController_OnDestroy(Entity& entity)
{}

void UIController_OnCollision(Entity& entity, Entity& other)
{}

void UIController_OnRaycastHit(Entity& entity, Entity& other)
{}

void UIController_PlayShootAnimation()
{
	s_Data.IsShooting = true;
}

void UIController_PlayHitIcon()
{
	s_Data.IsHit = true;
}