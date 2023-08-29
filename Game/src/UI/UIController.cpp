#include "UIController.h"
#include "Player/PlayerController.h"

#define BACKBAR_FOLLOW_COUNT_TIME 1.0f
#define SHOOTING_ANIMATION_FRAME_SPEED 0.15f
#define HIT_ICON_HOLD_TIME 0.5f

#define BLOOD_EFFECT_MAX_TIME 0.2f
#define BLOOD_EFFECT_ALPHA 0.2f

#define PAUSE_MENU_FONT_SIZE 40.0f
#define PAUSE_MENU_FONT_NAME L"Arial"
#define PAUSE_MENU_TEXT L"Exit Game"

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

	RectTransformComponent* RectTrans = nullptr;
	SpriteRendererComponent* SpriteRenderer = nullptr;
	float BloodEffectTimer = 0;
	bool IsBloodEffect = false;

	bool IsPaused = false;
	RectTransformComponent PauseMenuCanvas = {
		{ 840.0f,465.0f },
		{ 150.0f, 150.0f },
	};
};
static UIControllerData s_Data;

void UIController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};

	//HP Bar
	{
		Entity* Front = Scene_GetEntityByName(entity->Scene, "HPBarFront");
		Entity* Back = Scene_GetEntityByName(entity->Scene, "HPBarBack");

		CORE_ASSERT(Front, "Cannot find HPBarFront entity!");
		CORE_ASSERT(Back, "Cannot find HPBarBack entity!");

		s_Data.HPBarFrontRectTrans = (RectTransformComponent*)Entity_GetComponent(Front, ComponentType_RectTransform);
		s_Data.HPBarBackRectTrans = (RectTransformComponent*)Entity_GetComponent(Back, ComponentType_RectTransform);

		CORE_ASSERT(s_Data.HPBarFrontRectTrans, "Entity does not have RectTransformComponent!");
		CORE_ASSERT(s_Data.HPBarBackRectTrans, "Entity does not have RectTransformComponent!");
	}

	//Sight Icon
	{
		Entity* sightIcon = Scene_GetEntityByName(entity->Scene, "SightIcon");
		CORE_ASSERT(sightIcon, "Cannot find Weapon entity!");

		s_Data.SightIconSpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(sightIcon, ComponentType_SpriteRenderer);
		CORE_ASSERT(s_Data.SightIconSpriteRenderer, "Entity does not have SpriteRendererComponent!");

		SpriteAnimatorSpecification spec;
		Texture2D* texture = (Texture2D*)RefPtr_Get(s_Data.SightIconSpriteRenderer->Texture);
		spec.TextureWidth = Texture2D_GetWidth(texture);
		spec.TextureHeight = Texture2D_GetHeight(texture);
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
		Entity* weapon = Scene_GetEntityByName(entity->Scene, "Weapon");
		CORE_ASSERT(weapon, "Cannot find Weapon entity!");

		s_Data.WeaponSpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(weapon, ComponentType_SpriteRenderer);
		CORE_ASSERT(s_Data.WeaponSpriteRenderer, "Entity does not have SpriteRendererComponent!");

		SpriteAnimatorSpecification spec;
		Texture2D* texture = (Texture2D*)RefPtr_Get(s_Data.WeaponSpriteRenderer->Texture);
		spec.TextureWidth = Texture2D_GetWidth(texture);
		spec.TextureHeight = Texture2D_GetHeight(texture);
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

	// Background
	{
		s_Data.SpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);
		CORE_ASSERT(s_Data.SpriteRenderer, "Entity does not have SpriteRendererComponent!");

		s_Data.RectTrans = (RectTransformComponent*)Entity_GetComponent(entity, ComponentType_RectTransform);
		CORE_ASSERT(s_Data.RectTrans, "Entity does not have RectTransformComponent!");
	}
}

void PauseGame()
{
	if (s_Data.IsPaused)
	{
		Application_SetTimeScale(1.0f);
		s_Data.RectTrans->Position = { 0,0 };
		s_Data.RectTrans->Size = { 1920.0f,1080.0f };
		s_Data.SpriteRenderer->Color = { 1.0f,0,0,0 };
	}
	else
	{
		Application_SetTimeScale(0.0f);
		s_Data.RectTrans->Position = { 800.0f,500.0f };
		s_Data.RectTrans->Size = { 300.0f,500.0f };
		s_Data.SpriteRenderer->Color = { 1.0f,1.0f,1.0f,0.5f };
	}

	s_Data.IsPaused = !s_Data.IsPaused;
}

void UIController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	if (Input_GetKeyDown(KeyCode::Esc))
	{
		PauseGame();
	}

	if (s_Data.IsPaused)
	{
		Vec2 position, size, ndcPos;
		Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

		{
			RectTransformComponent_GetPositionAndSize(s_Data.PauseMenuCanvas, viewPortSize, &ndcPos, &position, &size);
			Renderer2D_DrawText(PAUSE_MENU_TEXT, PAUSE_MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, PAUSE_MENU_FONT_SIZE);
		}

		if (Input_GetKeyDown(KeyCode::Enter))
			Application_Close();
	}

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

			SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.WeaponSpriteAnimator, currentFrame);
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

	//Blood effect
	{
		if (s_Data.IsBloodEffect)
		{
			if (s_Data.BloodEffectTimer > BLOOD_EFFECT_MAX_TIME)
			{
				s_Data.IsBloodEffect = false;
				s_Data.SpriteRenderer->Color.w = 0.0f;
				s_Data.BloodEffectTimer = 0.0f;
			}

			s_Data.BloodEffectTimer += timeStep;
		}
	}
}

void UIController_OnDestroy(Entity* entity, void* runtimeData)
{}
void UIController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{}
void UIController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void UIController_OnEnable(Entity* entity, void* runtimeData)
{}
void UIController_OnDisable(Entity* entity, void* runtimeData)
{}

void UIController_PlayShootAnimation()
{
	s_Data.IsShooting = true;
}

void UIController_PlayHitIcon()
{
	s_Data.IsHit = true;
}

void UIController_PlayBloodEffect()
{
	s_Data.IsBloodEffect = true;
	s_Data.SpriteRenderer->Color = { 1.0f,0,0,BLOOD_EFFECT_ALPHA };
	s_Data.RectTrans->Position = { 0.0f,0.0f };
	s_Data.RectTrans->Size = { 1920.0f,1080.0f };
}

bool UIController_IsPaused()
{
	return s_Data.IsPaused;
}