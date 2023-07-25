#include "CameraController.h"

#include <stdio.h>

struct CameraControllerData
{
	TransformComponent* PlayerTransform = nullptr;
	TransformComponent* Transform = nullptr;
	CameraComponent* Camera = nullptr;

	float MinOrthographicSize = 5.0f;
	float MaxOrthographicSize = 50.0f;

	float Distance = -10.0f;
};
static CameraControllerData s_Data;

void CameraController_OnCreate(Entity& entity)
{
	Entity* Player = Scene_GetEntityByName(*entity.Scene, "Player");
	BV_ASSERT(Player, "Cannot find Player entity!");

	if (Player)
		s_Data.PlayerTransform = (TransformComponent*)Entity_GetComponent(*Player, ComponentType_Transform);
	BV_ASSERT(s_Data.PlayerTransform, "Entity does not have TransformComponent!");

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	BV_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");

	s_Data.Camera = (CameraComponent*)Entity_GetComponent(entity, ComponentType_Camera);
	BV_ASSERT(s_Data.Camera, "Entity does not have CameraComponent!");
}

void CameraController_OnUpdate(Entity& entity, float timeStep)
{
	if (Input_GetKey(KeyCode::R))
	{
		if (s_Data.Camera->Camera->Spec.ProjectionType == ProjectionType::Perspective)
			s_Data.Distance += 0.1f;
		else
			SceneCamera_SetOrthographicSize(*s_Data.Camera->Camera,
				FloatLerp(s_Data.Camera->Camera->Spec.OrthographicSize,
					FloatMin(s_Data.Camera->Camera->Spec.OrthographicSize - 5.0f, s_Data.MaxOrthographicSize), timeStep));
	}

	if (Input_GetKey(KeyCode::F))
	{
		if (s_Data.Camera->Camera->Spec.ProjectionType == ProjectionType::Perspective)
			s_Data.Distance -= 0.1f;
		else
			SceneCamera_SetOrthographicSize(*s_Data.Camera->Camera,
				FloatLerp(s_Data.Camera->Camera->Spec.OrthographicSize,
					FloatMax(s_Data.Camera->Camera->Spec.OrthographicSize + 5.0f, s_Data.MinOrthographicSize), timeStep));
	}

	if (Input_GetKey(KeyCode::Q))
		s_Data.Transform->Rotation.z += DirectX::XMConvertToRadians(1.0f);

	if (Input_GetKey(KeyCode::E))
		s_Data.Transform->Rotation.z -= DirectX::XMConvertToRadians(1.0f);

	if (Input_GetKeyDown(KeyCode::T))
	{
		if (s_Data.Camera->Camera->Spec.ProjectionType == ProjectionType::Perspective)
			SceneCamera_SetProjectionType(*s_Data.Camera->Camera, ProjectionType::Orthographic);
		else
			SceneCamera_SetProjectionType(*s_Data.Camera->Camera, ProjectionType::Perspective);
	}

	Vec3 playerPosition = s_Data.PlayerTransform->Translation;
	playerPosition.z = s_Data.Distance;
	s_Data.Transform->Translation = Vec3Lerp(s_Data.Transform->Translation, playerPosition, timeStep);
}

void CameraController_OnDestroy(Entity& entity)
{

}

void CameraController_OnCollision(Entity& entity, Entity& other)
{

}