#include "CameraController.h"

#include <stdio.h>

struct CameraControllerData
{
	TransformComponent* PlayerTransform = nullptr;
	TransformComponent* Transform = nullptr;

#ifndef CORE_DIST
	CameraComponent* Camera = nullptr;
	TransformComponent* DebugCameraTransform = nullptr;
	CameraComponent* DebugCameraCamera = nullptr;
	bool AutoFollow = false;
#endif
};
static CameraControllerData s_Data;

void CameraController_OnCreate(Entity& entity, void* runtimeData)
{
	s_Data = {};

	Entity* Player = Scene_GetEntityByName(*entity.Scene, "Player");
	CORE_ASSERT(Player, "Cannot find Player entity!");

	if (Player)
		s_Data.PlayerTransform = (TransformComponent*)Entity_GetComponent(*Player, ComponentType_Transform);
	CORE_ASSERT(s_Data.PlayerTransform, "Entity does not have TransformComponent!");

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

#ifndef CORE_DIST
	{
		s_Data.Camera = (CameraComponent*)Entity_GetComponent(entity, ComponentType_Camera);
		CORE_ASSERT(s_Data.Camera, "Entity does not have CameraComponent!");

		Entity* debugCamera = Scene_GetEntityByName(*entity.Scene, "DebugCamera");
		CORE_ASSERT(debugCamera, "Cannot find DebugCamera entity!");

		s_Data.DebugCameraTransform = (TransformComponent*)Entity_GetComponent(*debugCamera, ComponentType_Transform);
		CORE_ASSERT(s_Data.DebugCameraTransform, "Entity does not have TransformComponent!");

		s_Data.DebugCameraCamera = (CameraComponent*)Entity_GetComponent(*debugCamera, ComponentType_Camera);
		CORE_ASSERT(s_Data.DebugCameraCamera, "Entity does not have CameraComponent!");
	}
#endif
}

void CameraController_OnUpdate(Entity& entity, float timeStep, void* runtimeData)
{
	s_Data.Transform->Translation = s_Data.PlayerTransform->Translation;

#ifndef CORE_DIST
	{
		if (Input_GetKeyDown(KeyCode::I))
		{
			s_Data.Camera->Primary = !s_Data.Camera->Primary;
			s_Data.DebugCameraCamera->Primary = !s_Data.DebugCameraCamera->Primary;
		}

		if (Input_GetKeyDown(KeyCode::O))
		{
			s_Data.AutoFollow = !s_Data.AutoFollow;
		}

		if (s_Data.AutoFollow)
		{
			Vec3 pos = s_Data.PlayerTransform->Translation;
			pos.z = s_Data.DebugCameraTransform->Translation.z;
			s_Data.DebugCameraTransform->Translation = Vec3Lerp(s_Data.DebugCameraTransform->Translation, pos, timeStep);
		}
		else
		{
			if (s_Data.DebugCameraCamera->Primary)
			{
				Vec3 pos = s_Data.DebugCameraTransform->Translation;
				static float speed = 45.0f;

				if (Input_GetKey(KeyCode::UpArrow))
					pos.y += speed * timeStep;
				else if (Input_GetKey(KeyCode::DownArrow))
					pos.y -= speed * timeStep;
				if (Input_GetKey(KeyCode::LeftArrow))
					pos.x -= speed * timeStep;
				else if (Input_GetKey(KeyCode::RightArrow))
					pos.x += speed * timeStep;

				if (Input_GetKey(KeyCode::NumpadAdd))
					pos.z += speed * timeStep;
				else if (Input_GetKey(KeyCode::NumpadSubtract))
					pos.z -= speed * timeStep;

				s_Data.DebugCameraTransform->Translation = pos;
			}
		}
	}
#endif
}

void CameraController_OnDestroy(Entity& entity, void* runtimeData)
{}

void CameraController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}

void CameraController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{}