#include "pch.h"
#include "Input.h"
#include "Core/Window.h"

#include <GamePad.h>

struct InputData
{
	DirectX::GamePad* GamePad;
	DirectX::GamePad::State GamePadState;
};
static InputData s_InputData;

void Input_Init()
{
	s_InputData = {};
	s_InputData.GamePad = new DirectX::GamePad();
}

void Input_Update()
{
	s_InputData.GamePadState = s_InputData.GamePad->GetState(0);
}

void Input_Shutdown()
{
	s_InputData.GamePad->Suspend();
	delete s_InputData.GamePad;
	s_InputData.GamePad = nullptr;
}

void Input_Suspend()
{
	s_InputData.GamePad->Suspend();
}

void Input_Resume()
{
	s_InputData.GamePad->Resume();
}

bool Input_GetKey(GamePadCode key)
{
	if (!s_InputData.GamePadState.IsConnected())
		return false;

	switch (key)
	{
	case GamePadCode::ButtonA:
		return s_InputData.GamePadState.IsAPressed();
	case GamePadCode::ButtonB:
		return s_InputData.GamePadState.IsBPressed();
	case GamePadCode::ButtonX:
		return s_InputData.GamePadState.IsXPressed();
	case GamePadCode::ButtonY:
		return s_InputData.GamePadState.IsYPressed();
	case GamePadCode::DPadUp:
		return s_InputData.GamePadState.IsDPadUpPressed();
	case GamePadCode::DPadDown:
		return s_InputData.GamePadState.IsDPadDownPressed();
	case GamePadCode::DPadLeft:
		return s_InputData.GamePadState.IsDPadLeftPressed();
	case GamePadCode::DPadRight:
		return s_InputData.GamePadState.IsDPadRightPressed();
	case GamePadCode::LeftShoulder:
		return s_InputData.GamePadState.IsLeftShoulderPressed();
	case GamePadCode::RightShoulder:
		return s_InputData.GamePadState.IsRightShoulderPressed();
	case GamePadCode::LeftThumbStick:
		return s_InputData.GamePadState.IsLeftStickPressed();
	case GamePadCode::RightThumbStick:
		return s_InputData.GamePadState.IsRightStickPressed();
	case GamePadCode::Start:
		return s_InputData.GamePadState.IsStartPressed();
	case GamePadCode::Back:
		return s_InputData.GamePadState.IsBackPressed();
	case GamePadCode::LeftTrigger:
		return s_InputData.GamePadState.IsLeftTriggerPressed();
	case GamePadCode::RightTrigger:
		return s_InputData.GamePadState.IsRightTriggerPressed();
	}

	return false;
}

bool Input_GetKeyDown(GamePadCode key)
{
	if (!s_InputData.GamePadState.IsConnected())
		return false;

	return false;
}

bool Input_GetKey(KeyCode key)
{
	return GetAsyncKeyState(key) & 0x8000;
}

bool Input_GetKeyDown(KeyCode key)
{
	return GetAsyncKeyState(key) & 0x01;
}

bool Input_GetMouseDown(MouseCode button)
{
	return GetAsyncKeyState(button) & 0x01;
}

bool Input_GetMouseButton(MouseCode button)
{
	return GetAsyncKeyState(button) & 0x8000;
}

Vec2 Input_GetMousePosition()
{
	POINT point{ 0,0 };
	GetCursorPos(&point);
	ScreenToClient((HWND)Window_GetWindowHandle(), &point);
	return { (float)point.x ,(float)point.y };
}