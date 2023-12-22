#include "CameraController.h"

#include <stdio.h>

struct CameraControllerData
{
	TransformComponent* PlayerTransform = nullptr;
	TransformComponent* Transform = nullptr;

	RectTransformComponent* UIRectTransform = nullptr;
};
static CameraControllerData s_Data;

void CameraController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};

	Entity* UITest = Scene_GetEntityByName(entity->Scene, "UI_Test");
	s_Data.UIRectTransform = (RectTransformComponent*)Entity_GetComponent(UITest, ComponentType_RectTransform);

	Entity* Player = Scene_GetEntityByName(entity->Scene, "Player");
	CORE_ASSERT(Player, "Cannot find Player entity!");

	if (Player)
		s_Data.PlayerTransform = (TransformComponent*)Entity_GetComponent(Player, ComponentType_Transform);
	CORE_ASSERT(s_Data.PlayerTransform, "Entity does not have TransformComponent!");

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");
}

void CameraController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	s_Data.Transform->Translation = Vec3Lerp(s_Data.Transform->Translation, s_Data.PlayerTransform->Translation, timeStep);
	s_Data.UIRectTransform->Rotation += DirectX::XMConvertToRadians(15);
}

void CameraController_OnDestroy(Entity* entity, void* runtimeData)
{}
void CameraController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{}
void CameraController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void CameraController_OnEnable(Entity* entity, void* runtimeData)
{}
void CameraController_OnDisable(Entity* entity, void* runtimeData)
{}