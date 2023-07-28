#include "UIController.h"

struct UIControllerData
{
	float BarMaxWidth = 500.0f;

	RectTransformComponent* HPBarFrontRectTrans;
	RectTransformComponent* HPBarBackRectTrans;
	float HPBarBackFollowCountTime = 0.0;

	RectTransformComponent* VigorBarFrontRectTrans;
	RectTransformComponent* VigorBarBackRectTrans;
	float VigorBarBackFollowCountTime = 0.0;
};
static UIControllerData s_Data;

void UIController_OnCreate(Entity& entity)
{
	//HP Bar
	{
		Entity* Front = Scene_GetEntityByName(*entity.Scene, "HPBarFront");
		Entity* Back = Scene_GetEntityByName(*entity.Scene, "HPBarBack");

		BV_ASSERT(Front, "Cannot find HPBarFront entity!");
		BV_ASSERT(Back, "Cannot find HPBarBack entity!");

		s_Data.HPBarFrontRectTrans = (RectTransformComponent*)Entity_GetComponent(*Front, ComponentType_RectTransform);
		s_Data.HPBarBackRectTrans = (RectTransformComponent*)Entity_GetComponent(*Back, ComponentType_RectTransform);

		BV_ASSERT(s_Data.HPBarFrontRectTrans, "Entity does not have RectTransformComponent!");
		BV_ASSERT(s_Data.HPBarBackRectTrans, "Entity does not have RectTransformComponent!");
	}

	//Vigor Bar
	{
		Entity* Front = Scene_GetEntityByName(*entity.Scene, "VigorBarFront");
		Entity* Back = Scene_GetEntityByName(*entity.Scene, "VigorBarBack");

		BV_ASSERT(Front, "Cannot find VigorBarFront entity!");
		BV_ASSERT(Back, "Cannot find VigorBarBack entity!");

		s_Data.VigorBarFrontRectTrans = (RectTransformComponent*)Entity_GetComponent(*Front, ComponentType_RectTransform);
		s_Data.VigorBarBackRectTrans = (RectTransformComponent*)Entity_GetComponent(*Back, ComponentType_RectTransform);

		BV_ASSERT(s_Data.VigorBarFrontRectTrans, "Entity does not have RectTransformComponent!");
		BV_ASSERT(s_Data.VigorBarBackRectTrans, "Entity does not have RectTransformComponent!");
	}
}

void UIController_OnUpdate(Entity& entity, float timeStep)
{
	if (Input_GetKeyDown(KeyCode::O))
	{
		s_Data.HPBarFrontRectTrans->Size.x -= 100.0f;
		s_Data.HPBarBackFollowCountTime = 0.0f;

		s_Data.VigorBarFrontRectTrans->Size.x -= 100.0f;
		s_Data.VigorBarBackFollowCountTime = 0.0f;
	}
	if (Input_GetKeyDown(KeyCode::P))
	{
		s_Data.HPBarFrontRectTrans->Size.x = FloatMin(s_Data.HPBarFrontRectTrans->Size.x + 100.0f, s_Data.BarMaxWidth);
		s_Data.HPBarBackRectTrans->Size.x = s_Data.HPBarFrontRectTrans->Size.x;

		s_Data.VigorBarFrontRectTrans->Size.x = FloatMin(s_Data.VigorBarFrontRectTrans->Size.x + 100.0f, s_Data.BarMaxWidth);
		s_Data.VigorBarBackRectTrans->Size.x = s_Data.VigorBarFrontRectTrans->Size.x;
	}

	s_Data.HPBarBackFollowCountTime += timeStep;

	s_Data.VigorBarBackFollowCountTime += timeStep;

	if (s_Data.HPBarBackFollowCountTime > 1.0f)
	{
		s_Data.HPBarBackRectTrans->Size.x = FloatLerp(s_Data.HPBarBackRectTrans->Size.x, s_Data.HPBarFrontRectTrans->Size.x, timeStep);

		s_Data.VigorBarBackRectTrans->Size.x = FloatLerp(s_Data.VigorBarBackRectTrans->Size.x, s_Data.VigorBarFrontRectTrans->Size.x, timeStep);
	}
}

void UIController_OnDestroy(Entity& entity)
{

}

void UIController_OnCollision(Entity& entity, Entity& other)
{

}