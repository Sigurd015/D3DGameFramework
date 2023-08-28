#include "pch.h"
#include "SpriteTimer.h"

void SpriteTimer_Create(SpriteTimer& timer, uint32_t framesCount, float frameTime)
{
	timer.FramesCount = framesCount;
	timer.FrameTime = frameTime;
	timer.CurrentFrame = 0;
	timer.CurrentTimer = 0.0f;
}

void SpriteTimer_Update(SpriteTimer& timer, float timeStep)
{
	if (timer.CurrentTimer > timer.FrameTime)
	{
		timer.CurrentTimer = 0.0f;
		timer.CurrentFrame = (timer.CurrentFrame + 1) % timer.FramesCount;
	}

	timer.CurrentTimer += timeStep;
}

uint32_t SpriteTimer_GetFrame(SpriteTimer& timer)
{
	return timer.CurrentFrame;
}

void SpriteTimer_Reset(SpriteTimer& timer)
{
	timer.CurrentFrame = 0;
	timer.CurrentTimer = 0.0f;
}