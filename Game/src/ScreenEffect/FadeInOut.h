#pragma once

enum FadeInOutMode
{
	FADE_NONE = 0, FADE_IN, FADE_OUT
};

void FadeInOut_Initialize();
void FadeInOut_Reset(FadeInOutMode mode);
void FadeInOut_Draw(float timeStep);