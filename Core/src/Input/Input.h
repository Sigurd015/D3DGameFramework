#pragma once
#include "KeyCode.h"
#include "MouseCode.h"
#include "Core/Base.h"

bool Input_GetKey(KeyCode key);
bool Input_GetKeyDown(KeyCode key);
bool Input_GetMouseButton(MouseCode button);
bool Input_GetMouseDown(MouseCode button);
Vec2 Input_GetMousePosition();