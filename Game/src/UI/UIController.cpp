#include "UIController.h"
#include "../Player/PlayerController.h"
#define BACKBAR_FOLLOW_COUNT_TIME 1.0f

struct UIControllerData
{
	float BarMaxWidth = 500.0f;

	RectTransformComponent* HPBarFrontRectTrans;
	RectTransformComponent* HPBarBackRectTrans;
	float LastHPBarFrontWidth = BarMaxWidth;
	float HPBarBackFollowCountTime = 0.0;
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
}

void UIController_OnUpdate(Entity& entity, float timeStep)
{
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
}

void UIController_OnDestroy(Entity& entity)
{

}

void UIController_OnCollision(Entity& entity, Entity& other)
{

}