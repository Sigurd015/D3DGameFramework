#include "pch.h"
#include "Input.h"
#include "Core/Window.h"

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