#include "CameraController.h"

#include <stdio.h>

struct CameraControllerData
{
	TransformComponent* Transform = nullptr;

	Vec3 Forward = { 0.0f, 0.0f, -1.0f };
	Vec3 Right = { 1.0f, 0.0f, 0.0f };

	float MoveSpeed = 5.0f;
	Vec2 RotationSpeed = { DirectX::XMConvertToRadians(45.0f), DirectX::XMConvertToRadians(45.0f) }; // Yaw, Pitch

	float MinPitch = -DirectX::XMConvertToRadians(85.0f);
	float MaxPitch = DirectX::XMConvertToRadians(85.0f);

	bool UseMouse = false;
	Vec2 MouseSensitivity = { 0.1f, 0.1f };
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
	Vec3 pos = s_Data.Transform->Translation;
	if (Input_GetKey(KeyCode_W))
	{
		pos = Vec3Add(pos, Vec3MulFloat(s_Data.Forward, s_Data.MoveSpeed));
	}
	else if (Input_GetKey(KeyCode_S))
	{
		pos = Vec3Sub(pos, Vec3MulFloat(s_Data.Forward, s_Data.MoveSpeed));
	}

	if (Input_GetKey(KeyCode_A))
	{
		pos = Vec3Sub(pos, Vec3MulFloat(s_Data.Right, s_Data.MoveSpeed));
	}
	else if (Input_GetKey(KeyCode_D))
	{
		pos = Vec3Add(pos, Vec3MulFloat(s_Data.Right, s_Data.MoveSpeed));
	}

	float yaw = s_Data.Transform->Rotation.y;
	float pitch = s_Data.Transform->Rotation.x;

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
			yaw += s_Data.RotationSpeed.y;
		}
		else if (Input_GetKey(KeyCode_L))
		{
			yaw -= s_Data.RotationSpeed.y;
		}

		if (Input_GetKey(KeyCode_I))
		{
			pitch += s_Data.RotationSpeed.x;
		}
		else if (Input_GetKey(KeyCode_K))
		{
			pitch -= s_Data.RotationSpeed.x;
		}
	}
	else
	{
		// Mouse
		Vec2 mouseDelta = Input_GetMousePosDelta();
		yaw -= mouseDelta.x * s_Data.RotationSpeed.y * s_Data.MouseSensitivity.x;
		pitch -= mouseDelta.y * s_Data.RotationSpeed.x * s_Data.MouseSensitivity.y;
	}

	pitch = FloatClamp(pitch, s_Data.MinPitch, s_Data.MaxPitch);

	APP_LOG_INFO("Yaw: %f, Pitch: %f", yaw, pitch);
	APP_LOG_INFO("Move: %f, %f, %f", pos.x, pos.y, pos.z);

	s_Data.Transform->Translation = Vec3Lerp(s_Data.Transform->Translation, pos, timeStep);
	s_Data.Transform->Rotation = Vec3Lerp(s_Data.Transform->Rotation, { pitch, yaw, 0.0f }, timeStep);

	s_Data.Forward = Vec3Normalize(Vec3Rotate(s_Data.Forward, { 0.0f, 1.0f, 0.0f }, s_Data.Transform->Rotation.y));
	s_Data.Right = Vec3Normalize(Vec3Rotate(s_Data.Right, { 0.0f, 1.0f, 0.0f }, s_Data.Transform->Rotation.y));

	// Debug
	{
		if (Input_GetKey(KeyCode_F))
		{
			s_Data.Transform->Translation.y -= 5.0f * timeStep;
		}
		else if (Input_GetKey(KeyCode_R))
		{
			s_Data.Transform->Translation.y += 5.0f * timeStep;
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