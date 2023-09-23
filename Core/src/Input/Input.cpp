#include "pch.h"
#include "Input.h"
#include "Core/Window.h"

#include <GamePad.h>

struct InputData
{
	DirectX::GamePad* GamePad;

	DirectX::GamePad::State PrevGamePadState;
	DirectX::GamePad::State CurrentGamePadState;

	bool PrevKeyState[256];
	bool CurrentKeyState[256];
};
static InputData s_InputData;

void Input_Init()
{
	s_InputData = {};
	s_InputData.GamePad = new DirectX::GamePad();
}

void Input_Update()
{
	// GamePad
	s_InputData.PrevGamePadState = s_InputData.CurrentGamePadState;
	s_InputData.CurrentGamePadState = s_InputData.GamePad->GetState(0);

	// Keyboard & Mouse
	for (int KeyCode = 0; KeyCode < 256; ++KeyCode)
	{
		s_InputData.PrevKeyState[KeyCode] = s_InputData.CurrentKeyState[KeyCode];
		s_InputData.CurrentKeyState[KeyCode] = (GetAsyncKeyState(KeyCode) & 0x8000) != 0;
	}
}

void Input_Shutdown()
{
	// TODO: Fix this(random crash when shutting down)
	//s_InputData.GamePad->Suspend();
	//delete s_InputData.GamePad;
	//s_InputData.GamePad = nullptr;
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
	if (!s_InputData.CurrentGamePadState.IsConnected())
		return false;

	switch (key)
	{
	case GamePadCode_A:
		return s_InputData.CurrentGamePadState.IsAPressed()
			&& s_InputData.PrevGamePadState.IsAPressed();
	case GamePadCode_B:
		return s_InputData.CurrentGamePadState.IsBPressed()
			&& s_InputData.PrevGamePadState.IsBPressed();
	case GamePadCode_X:
		return s_InputData.CurrentGamePadState.IsXPressed()
			&& s_InputData.PrevGamePadState.IsXPressed();
	case GamePadCode_Y:
		return s_InputData.CurrentGamePadState.IsYPressed()
			&& s_InputData.PrevGamePadState.IsYPressed();
	case GamePadCode_DPadUp:
		return s_InputData.CurrentGamePadState.IsDPadUpPressed()
			&& s_InputData.PrevGamePadState.IsDPadUpPressed();
	case GamePadCode_DPadDown:
		return s_InputData.CurrentGamePadState.IsDPadDownPressed()
			&& s_InputData.PrevGamePadState.IsDPadDownPressed();
	case GamePadCode_DPadLeft:
		return s_InputData.CurrentGamePadState.IsDPadLeftPressed()
			&& s_InputData.PrevGamePadState.IsDPadLeftPressed();
	case GamePadCode_DPadRight:
		return s_InputData.CurrentGamePadState.IsDPadRightPressed()
			&& s_InputData.PrevGamePadState.IsDPadRightPressed();
	case GamePadCode_LeftShoulder:
		return s_InputData.CurrentGamePadState.IsLeftShoulderPressed()
			&& s_InputData.PrevGamePadState.IsLeftShoulderPressed();
	case GamePadCode_RightShoulder:
		return s_InputData.CurrentGamePadState.IsRightShoulderPressed()
			&& s_InputData.PrevGamePadState.IsRightShoulderPressed();
	case GamePadCode_LeftThumbStick:
		return s_InputData.CurrentGamePadState.IsLeftStickPressed()
			&& s_InputData.PrevGamePadState.IsLeftStickPressed();
	case GamePadCode_RightThumbStick:
		return s_InputData.CurrentGamePadState.IsRightStickPressed()
			&& s_InputData.PrevGamePadState.IsRightStickPressed();
	case GamePadCode_Start:
		return s_InputData.CurrentGamePadState.IsStartPressed()
			&& s_InputData.PrevGamePadState.IsStartPressed();
	case GamePadCode_Back:
		return s_InputData.CurrentGamePadState.IsBackPressed()
			&& s_InputData.PrevGamePadState.IsBackPressed();
	case GamePadCode_LeftTrigger:
		return s_InputData.CurrentGamePadState.IsLeftTriggerPressed()
			&& s_InputData.PrevGamePadState.IsLeftTriggerPressed();
	case GamePadCode_RightTrigger:
		return s_InputData.CurrentGamePadState.IsRightTriggerPressed()
			&& s_InputData.PrevGamePadState.IsRightTriggerPressed();
	}

	return false;
}

bool Input_GetKeyDown(GamePadCode key)
{
	if (!s_InputData.CurrentGamePadState.IsConnected())
		return false;

	switch (key)
	{
	case GamePadCode_A:
		return s_InputData.CurrentGamePadState.IsAPressed()
			&& !s_InputData.PrevGamePadState.IsAPressed();
	case GamePadCode_B:
		return s_InputData.CurrentGamePadState.IsBPressed()
			&& !s_InputData.PrevGamePadState.IsBPressed();
	case GamePadCode_X:
		return s_InputData.CurrentGamePadState.IsXPressed()
			&& !s_InputData.PrevGamePadState.IsXPressed();
	case GamePadCode_Y:
		return s_InputData.CurrentGamePadState.IsYPressed()
			&& !s_InputData.PrevGamePadState.IsYPressed();
	case GamePadCode_DPadUp:
		return s_InputData.CurrentGamePadState.IsDPadUpPressed()
			&& !s_InputData.PrevGamePadState.IsDPadUpPressed();
	case GamePadCode_DPadDown:
		return s_InputData.CurrentGamePadState.IsDPadDownPressed()
			&& !s_InputData.PrevGamePadState.IsDPadDownPressed();
	case GamePadCode_DPadLeft:
		return s_InputData.CurrentGamePadState.IsDPadLeftPressed()
			&& !s_InputData.PrevGamePadState.IsDPadLeftPressed();
	case GamePadCode_DPadRight:
		return s_InputData.CurrentGamePadState.IsDPadRightPressed()
			&& !s_InputData.PrevGamePadState.IsDPadRightPressed();
	case GamePadCode_LeftShoulder:
		return s_InputData.CurrentGamePadState.IsLeftShoulderPressed()
			&& !s_InputData.PrevGamePadState.IsLeftShoulderPressed();
	case GamePadCode_RightShoulder:
		return s_InputData.CurrentGamePadState.IsRightShoulderPressed()
			&& !s_InputData.PrevGamePadState.IsRightShoulderPressed();
	case GamePadCode_LeftThumbStick:
		return s_InputData.CurrentGamePadState.IsLeftStickPressed()
			&& !s_InputData.PrevGamePadState.IsLeftStickPressed();
	case GamePadCode_RightThumbStick:
		return s_InputData.CurrentGamePadState.IsRightStickPressed()
			&& !s_InputData.PrevGamePadState.IsRightStickPressed();
	case GamePadCode_Start:
		return s_InputData.CurrentGamePadState.IsStartPressed()
			&& !s_InputData.PrevGamePadState.IsStartPressed();
	case GamePadCode_Back:
		return s_InputData.CurrentGamePadState.IsBackPressed()
			&& !s_InputData.PrevGamePadState.IsBackPressed();
	case GamePadCode_LeftTrigger:
		return s_InputData.CurrentGamePadState.IsLeftTriggerPressed()
			&& !s_InputData.PrevGamePadState.IsLeftTriggerPressed();
	case GamePadCode_RightTrigger:
		return s_InputData.CurrentGamePadState.IsRightTriggerPressed()
			&& !s_InputData.PrevGamePadState.IsRightTriggerPressed();
	}

	return false;
}

Vec2 Input_GetLeftStick()
{
	if (!s_InputData.CurrentGamePadState.IsConnected())
		return Vec2Zero;

	return { s_InputData.CurrentGamePadState.thumbSticks.leftX,
			s_InputData.CurrentGamePadState.thumbSticks.leftY };
}

Vec2 Input_GetRightStick()
{
	if (!s_InputData.CurrentGamePadState.IsConnected())
		return Vec2Zero;

	return { s_InputData.CurrentGamePadState.thumbSticks.rightX,
			s_InputData.CurrentGamePadState.thumbSticks.rightY };
}

bool Input_GetKey(KeyCode key)
{
	return s_InputData.CurrentKeyState[key] && s_InputData.PrevKeyState[key];
}

bool Input_GetKeyDown(KeyCode key)
{
	return s_InputData.CurrentKeyState[key] && !s_InputData.PrevKeyState[key];
}

bool Input_GetMouseButton(MouseCode button)
{
	return s_InputData.CurrentKeyState[button] && s_InputData.PrevKeyState[button];
}

bool Input_GetMouseButtonDown(MouseCode button)
{
	return s_InputData.CurrentKeyState[button] && !s_InputData.PrevKeyState[button];
}

Vec2 Input_GetMousePosition()
{
	POINT point{ 0,0 };
	GetCursorPos(&point);
	ScreenToClient((HWND)Window_GetWindowHandle(), &point);
	return { (float)point.x ,(float)point.y };
}