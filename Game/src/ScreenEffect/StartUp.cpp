#include "Core.h"
#include "StartUp.h"
#include "../GameMode.h"

#define ENGINE_LOGO_START_FRAME_TIME 1.0f
#define ENGINE_LOGO_END_FRAME_TIME 3.0f
#define STUDIO_LOGO_START_FRAME_TIME 6.0f
#define STUDIO_LOGO_END_FRAME_TIME 8.0f
#define FADE_IN_OUT_FRAME_TIME 2.0f

#define FONT_NAME L"Arial"

struct StartUpData
{
	RectTransformComponent Canvas;
	Vec4 Color;

	float CurrentFrameTime;
	float MaxFrameTime;
};
static StartUpData s_Data;

void StartUp_Initialize()
{
	s_Data.Canvas.Position = { 655.0f,465.0f };
	s_Data.Canvas.Size = { 150.0f, 150.0f };
	s_Data.Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	s_Data.CurrentFrameTime = 0.0f;
	s_Data.MaxFrameTime = STUDIO_LOGO_END_FRAME_TIME + FADE_IN_OUT_FRAME_TIME + 1.0f;
}

void StartUp_Draw(float timeStep)
{
	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
	RectTransformComponent_GetPositionAndSize(s_Data.Canvas, viewPortSize, &ndcPos, &position, &size);

	WCHAR* text = L"";

	if (s_Data.CurrentFrameTime < ENGINE_LOGO_START_FRAME_TIME + FADE_IN_OUT_FRAME_TIME)
	{
		s_Data.Color.w = FloatLerp(s_Data.Color.w, 1.0f, timeStep);
		text = L"Made with Hanabi Engine";
	}
	else if (s_Data.CurrentFrameTime < ENGINE_LOGO_END_FRAME_TIME + FADE_IN_OUT_FRAME_TIME)
	{
		s_Data.Color.w = FloatLerp(s_Data.Color.w, 0.0f, timeStep);
		text = L"Made with Hanabi Engine";
	}

	if (s_Data.CurrentFrameTime > STUDIO_LOGO_START_FRAME_TIME)
	{
		if (s_Data.CurrentFrameTime < STUDIO_LOGO_START_FRAME_TIME + FADE_IN_OUT_FRAME_TIME)
		{
			s_Data.Color.w = FloatLerp(s_Data.Color.w, 1.0f, timeStep);
			text = L"Made by Freestyle Studio";
		}
		else if (s_Data.CurrentFrameTime < STUDIO_LOGO_END_FRAME_TIME + FADE_IN_OUT_FRAME_TIME)
		{
			s_Data.Color.w = FloatLerp(s_Data.Color.w, 0.0f, timeStep);
			text = L"Made by Freestyle Studio";
		}
	}

	Renderer2D_DrawText(text, FONT_NAME, position, s_Data.Color, 50.0f);

	s_Data.CurrentFrameTime += timeStep;

	if (s_Data.CurrentFrameTime > s_Data.MaxFrameTime)
	{
		Game_SetMode(TITLE_MENU);
	}
}

void StartUp_Destroy()
{}