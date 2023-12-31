#pragma once
#include "GameMode.h"

void Loading_Initialize();
void Loading_Reset(float maxTime = 3.0f);
void Loading_Draw(float timeStep);
void Loading_SetDepature(GameMode depature);
void Loading_Destroy();