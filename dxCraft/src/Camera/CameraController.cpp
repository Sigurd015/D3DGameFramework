#include "CameraController.h"

#include <stdio.h>

struct CameraControllerData
{
	TransformComponent* Transform = nullptr;

	float MoveSpeed = 5.0f;
	float RotationSpeedYaw = DirectX::XMConvertToRadians(45.0f);
	float RotationSpeedPitch = DirectX::XMConvertToRadians(45.0f);

	float MinPitch = -DirectX::XMConvertToRadians(85.0f);
	float MaxPitch = DirectX::XMConvertToRadians(85.0f);

	bool UseMouse = false;
	Vec2 MouseSensitivity = { 0.1f, 0.1f };

	// Debug
	bool LockOnPlane = false;
	float ClimbSpeed = 5.0f;
};
static CameraControllerData s_Data;

void CameraController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};

	s_Data.Transform = (TransformComponent*)Entity_GetComponent(entity, ComponentType_Transform);
	CORE_ASSERT(s_Data.Transform, "Entity does not have TransformComponent!");
}

void CameraController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	Vec3 forward = TransformComponent_GetForward(*s_Data.Transform);
	Vec3 right = TransformComponent_GetRight(*s_Data.Transform);

	Vec3 pos = s_Data.Transform->Translation;
	float yaw = s_Data.Transform->Rotation.y;
	float pitch = s_Data.Transform->Rotation.x;

	// Debug
	{
		if (Input_GetKeyDown(KeyCode_P))
			s_Data.LockOnPlane = !s_Data.LockOnPlane;

		if (s_Data.LockOnPlane)
		{
			forward.y = 0.0f;
			right.y = 0.0f;
		}
	}

	if (Input_GetKey(KeyCode_W))
	{
		pos = Vec3Add(pos, Vec3MulFloat(forward, s_Data.MoveSpeed));
	}
	else if (Input_GetKey(KeyCode_S))
	{
		pos = Vec3Sub(pos, Vec3MulFloat(forward, s_Data.MoveSpeed));
	}

	if (Input_GetKey(KeyCode_A))
	{
		pos = Vec3Sub(pos, Vec3MulFloat(right, s_Data.MoveSpeed));
	}
	else if (Input_GetKey(KeyCode_D))
	{
		pos = Vec3Add(pos, Vec3MulFloat(right, s_Data.MoveSpeed));
	}

	if (Input_GetKeyDown(KeyCode_M))
	{
		s_Data.UseMouse = !s_Data.UseMouse;
		Input_SetCursorVisible(!s_Data.UseMouse);
	}

	if (!s_Data.UseMouse)
	{
		// Keyboard
		if (Input_GetKey(KeyCode_J))
		{
			yaw += s_Data.RotationSpeedYaw;
		}
		else if (Input_GetKey(KeyCode_L))
		{
			yaw -= s_Data.RotationSpeedYaw;
		}

		if (Input_GetKey(KeyCode_I))
		{
			pitch += s_Data.RotationSpeedPitch;
		}
		else if (Input_GetKey(KeyCode_K))
		{
			pitch -= s_Data.RotationSpeedPitch;
		}
	}
	else
	{
		// Mouse
		Vec2 mouseDelta = Input_GetMousePosDelta();
		yaw -= mouseDelta.x * s_Data.RotationSpeedYaw * s_Data.MouseSensitivity.x;
		pitch -= mouseDelta.y * s_Data.RotationSpeedPitch * s_Data.MouseSensitivity.y;
	}

	pitch = FloatClamp(pitch, s_Data.MinPitch, s_Data.MaxPitch);

	s_Data.Transform->Translation = Vec3Lerp(s_Data.Transform->Translation, pos, timeStep);
	s_Data.Transform->Rotation = Vec3Lerp(s_Data.Transform->Rotation, { pitch, yaw, 0.0f }, timeStep);

	// Debug
	{
		if (Input_GetKey(KeyCode_F))
		{
			s_Data.Transform->Translation.y -= s_Data.ClimbSpeed * timeStep;
		}
		else if (Input_GetKey(KeyCode_R))
		{
			s_Data.Transform->Translation.y += s_Data.ClimbSpeed * timeStep;
		}
	}
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