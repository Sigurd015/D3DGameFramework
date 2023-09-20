#pragma once
#include "KeyCode.h"
#include "MouseCode.h"
#include "GamePadCode.h"
#include "Core/Base.h"

void Input_Init();
void Input_Update();
void Input_Shutdown();
void Input_Suspend();
void Input_Resume();
bool Input_GetKey(GamePadCode key);
bool Input_GetKeyDown(GamePadCode key);
bool Input_GetKey(KeyCode key);
bool Input_GetKeyDown(KeyCode key);
bool Input_GetMouseButton(MouseCode button);
bool Input_GetMouseDown(MouseCode button);
Vec2 Input_GetMousePosition();