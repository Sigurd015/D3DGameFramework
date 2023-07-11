#include "pch.h"
#include "Input.h"
#include "Core/Window.h"

bool Input_IsKeyPressed(KeyCode key)
{
	return GetAsyncKeyState(key) & 0x8000;
}

bool Input_IsMouseButtonPressed(MouseCode button)
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