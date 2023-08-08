#pragma once

#include <cstdint>

struct SpriteTimer
{
	uint32_t FramesCount = 0;
	uint32_t CurrentFrame = 0;
	float FrameTime = 0.1f;
	float CurrentTimer = 0.0f;
};

void SpriteTimer_Create(SpriteTimer& timer, uint32_t framesCount, float frameTime);
void SpriteTimer_Update(SpriteTimer& timer, float timeStep);
uint32_t SpriteTimer_GetFrame(SpriteTimer& timer);
void SpriteTimer_Reset(SpriteTimer& timer);