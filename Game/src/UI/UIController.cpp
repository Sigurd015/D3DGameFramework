#include "UIController.h"
#include "Player/PlayerController.h"
#include "ScreenEffect/Loading.h"
#include "GameMode.h"

#include <atlbase.h>
#include <atlconv.h>

#define BACKBAR_FOLLOW_COUNT_TIME 1.0f
#define SHOOTING_ANIMATION_FRAME_SPEED 0.15f
#define HIT_ICON_HOLD_TIME 0.5f

#define BLOOD_EFFECT_MAX_TIME 0.2f
#define BLOOD_EFFECT_ALPHA 0.2f

#define PAUSE_MENU_FONT_SIZE 40.0f
#define PAUSE_MENU_FONT_NAME L"Arial"

#define SCORE_FONT_SIZE 40.0f
#define SCORE_FONT_NAME L"Arial"

struct PauseMenuNode
{
	WCHAR* Text;
	float yPos;
	float yPosForD2D;

	PauseMenuNode* Next = nullptr;
	PauseMenuNode* Prev = nullptr;

	void(*OnEnter)();
};

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

	RectTransformComponent ScoreCanvas = {
		{ 700.0f,50.0f },
		{ 150.0f, 150.0f },
	};
	WCHAR* ScoreText = L"";

	uint32_t Score = 0;
	float Timer = 0;
	float MaxTime = 120.0f;
	bool TimerStarted = false;

	bool IsPaused = false;
	RectTransformComponent PauseMenuCanvas = {
		{ 840.0f,465.0f },
		{ 150.0f, 150.0f },
	};

	Entity* SelectionEntity = nullptr;
	RectTransformComponent* SelectionRectTrans = nullptr;
	SpriteRendererComponent* SelectionSprite = nullptr;

	PauseMenuNode PauseMenuNodes[2];
	PauseMenuNode* CurrentPauseMenuNode = nullptr;

	bool IsPlayerDead = false;
	bool IsTiemEnd = false;
	#ifndef CORE_DIST
	bool ShowPauseMenu = true;
	CameraComponent* Camera = nullptr;
	Entity* HPBarFront = nullptr;
	Entity* HPBarBack = nullptr;
	Entity* HPBackground = nullptr;
	Entity* Weapon = nullptr;
	Entity* SightIcon = nullptr;
	#endif 
};
static UIControllerData s_Data;

void BackToMenu()
{
	Application_SetTimeScale(1.0f);
	Loading_Reset();
	Loading_SetDepature(TITLE_MENU);
	Game_SetMode(LOADING_SCENE);
}

void ExitGame()
{
	Game_SetMode(LOADING_SCENE);
	Application_Close();
}

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

	{
		s_Data.PauseMenuNodes[0] = { L"Back to Menu" ,770.0f,250.0f ,&s_Data.PauseMenuNodes[1],&s_Data.PauseMenuNodes[1], BackToMenu };
		s_Data.PauseMenuNodes[1] = { L"Exit Game" ,620.0f,400.0f ,&s_Data.PauseMenuNodes[0],&s_Data.PauseMenuNodes[0], ExitGame };
		s_Data.CurrentPauseMenuNode = &s_Data.PauseMenuNodes[0];
	}

	//Selection
	{
		s_Data.SelectionEntity = Scene_GetEntityByName(entity->Scene, "Selection");

		CORE_ASSERT(s_Data.SelectionEntity, "Cannot find selection entity!");

		s_Data.SelectionRectTrans = (RectTransformComponent*)Entity_GetComponent(s_Data.SelectionEntity, ComponentType_RectTransform);

		CORE_ASSERT(s_Data.SelectionRectTrans, "Entity does not have RectTransformComponent!");

		s_Data.SelectionSprite = (SpriteRendererComponent*)Entity_GetComponent(s_Data.SelectionEntity, ComponentType_SpriteRenderer);

		CORE_ASSERT(s_Data.SelectionSprite, "Entity does not have RectTransformComponent!");
	}

	#ifndef CORE_DIST
	{
		Entity* camera = Scene_GetEntityByName(entity->Scene, "MainCamera");
		s_Data.Camera = (CameraComponent*)Entity_GetComponent(camera, ComponentType_Camera);

		s_Data.HPBarFront = Scene_GetEntityByName(entity->Scene, "HPBarFront");
		s_Data.HPBarBack = Scene_GetEntityByName(entity->Scene, "HPBarBack");
		s_Data.HPBackground = Scene_GetEntityByName(entity->Scene, "HPBarBackground");
		s_Data.Weapon = Scene_GetEntityByName(entity->Scene, "Weapon");
		s_Data.SightIcon = Scene_GetEntityByName(entity->Scene, "SightIcon");
	}
	#endif 
}

void PauseGame()
{
	if (s_Data.IsPaused)
	{
		Application_SetTimeScale(1.0f);
		Scene_SetEntityEnabled(s_Data.SelectionEntity, false);
	}
	else
	{
		Application_SetTimeScale(0.0f);
		Scene_SetEntityEnabled(s_Data.SelectionEntity, true);
	}

	s_Data.IsPaused = !s_Data.IsPaused;
}

void DrawScore()
{
	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

	wchar_t text[256];
	swprintf_s(text, 256, L"Score: %d    Time: %.1f", s_Data.Score, s_Data.Timer);
	s_Data.ScoreText = wcsdup(text);
	RectTransformComponent_GetPositionAndSize(s_Data.ScoreCanvas, viewPortSize, &ndcPos, &position, &size);
	Renderer2D_DrawText(s_Data.ScoreText, SCORE_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, SCORE_FONT_SIZE);
}

void UIController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	#ifndef CORE_DIST
	{
		if (!s_Data.Camera->Primary)
		{
			Scene_SetEntityEnabled(s_Data.Weapon, false);
			Scene_SetEntityEnabled(s_Data.SightIcon, false);
			Scene_SetEntityEnabled(s_Data.HPBarFront, false);
			Scene_SetEntityEnabled(s_Data.HPBarBack, false);
			Scene_SetEntityEnabled(s_Data.HPBackground, false);

			return;
		}
		else
		{
			Scene_SetEntityEnabled(s_Data.Weapon, true);
			Scene_SetEntityEnabled(s_Data.SightIcon, true);
			Scene_SetEntityEnabled(s_Data.HPBarFront, true);
			Scene_SetEntityEnabled(s_Data.HPBarBack, true);
			Scene_SetEntityEnabled(s_Data.HPBackground, true);
		}
	}
	#endif 

	if (s_Data.IsPaused)
	{
		#ifndef CORE_DIST
		if (Input_GetKeyDown(KeyCode::Numpad5))
			s_Data.ShowPauseMenu = !s_Data.ShowPauseMenu;

		if (!s_Data.ShowPauseMenu)
		{
			Scene_SetEntityEnabled(s_Data.SelectionEntity, false);
			return;
		}
		else
		{
			Scene_SetEntityEnabled(s_Data.SelectionEntity, true);
		}
		#endif

		Vec2 position, size, ndcPos;
		Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

		for (size_t i = 0; i < 2; i++)
		{
			s_Data.PauseMenuCanvas.Position.y = s_Data.PauseMenuNodes[i].yPosForD2D;
			RectTransformComponent_GetPositionAndSize(s_Data.PauseMenuCanvas, viewPortSize, &ndcPos, &position, &size);
			Renderer2D_DrawText(s_Data.PauseMenuNodes[i].Text, PAUSE_MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, PAUSE_MENU_FONT_SIZE);
		}

		//Score
		if (s_Data.IsPlayerDead || s_Data.IsTiemEnd)
			DrawScore();

		if (Input_GetKeyDown(KeyCode::Enter))
		{
			s_Data.CurrentPauseMenuNode->OnEnter();
		}

		if (Input_GetKeyDown(KeyCode::UpArrow))
		{
			s_Data.CurrentPauseMenuNode = s_Data.CurrentPauseMenuNode->Prev;
		}
		else if (Input_GetKeyDown(KeyCode::DownArrow))
		{
			s_Data.CurrentPauseMenuNode = s_Data.CurrentPauseMenuNode->Next;
		}

		s_Data.SelectionRectTrans->Position.y = s_Data.CurrentPauseMenuNode->yPos;
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

	if (s_Data.IsTiemEnd || s_Data.IsPlayerDead)
		return;

	if (Input_GetKeyDown(KeyCode::Esc))
	{
		PauseGame();
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

	// Score
	{
		DrawScore();

		if (s_Data.TimerStarted)
			s_Data.Timer += timeStep;

		if (s_Data.Timer > s_Data.MaxTime)
		{
			s_Data.IsTiemEnd = true;
			PauseGame();
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

void UIController_OnEnemyDead(uint32_t score)
{
	s_Data.Score += score;
}

void UIController_OnPlayerDead()
{
	#ifndef CORE_DIST
	{
		if (!s_Data.Camera->Primary)
			return;
	}
	#endif

	s_Data.IsPlayerDead = true;
	PauseGame();
}

void UIController_PlayShootAnimation()
{
	s_Data.IsShooting = true;
}

void UIController_PlayHitIcon()
{
	s_Data.IsHit = true;
}

void UIController_PlayBloodEffect(const Vec3& color)
{
	s_Data.IsBloodEffect = true;
	s_Data.SpriteRenderer->Color = { color.x,color.y,color.z,BLOOD_EFFECT_ALPHA };
}

bool UIController_IsPaused()
{
	return s_Data.IsPaused;
}

void UIController_StartTimer()
{
	if (!s_Data.TimerStarted)
	{
		s_Data.Timer = 0.0f;
		s_Data.TimerStarted = true;
	}
}