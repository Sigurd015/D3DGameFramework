#pragma once
#include "Keycode.h"
#include "MouseCode.h"
#include "Core/base.h"

bool Input_IsKeyPressed(KeyCode key);
bool Input_IsMouseButtonPressed(MouseCode button);
Vec2 Input_GetMousePosition();