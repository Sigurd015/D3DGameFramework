#include "Core.h"
#include "StartUp.h"
#include "../GameMode.h"

#define ENGINE_LOGO_START_FRAME_TIME 1.0f
#define ENGINE_LOGO_END_FRAME_TIME 3.0f
#define STUDIO_LOGO_START_FRAME_TIME 6.0f
#define STUDIO_LOGO_END_FRAME_TIME 8.0f
#define FADE_IN_OUT_FRAME_TIME 2.0f

#define FONT_NAME L"Arial"
#define FONT_SIZE 50.0f

struct StartUpData
{
	RectTransformComponent Canvas = {
		{ 655.0f,465.0f },
		{ 150.0f, 150.0f },
	};
	Vec4 Color = { 1.0f, 1.0f, 1.0f, 0.0f };

	float CurrentFrameTime = 0.0f;
	float MaxFrameTime = STUDIO_LOGO_END_FRAME_TIME + FADE_IN_OUT_FRAME_TIME + 1.0f;
};
static StartUpData s_Data;

void StartUp_Initialize()
{
	s_Data = {};
}

void StartUp_Draw(float timeStep)
{
	if (s_Data.CurrentFrameTime > s_Data.MaxFrameTime)
	{
		Game_SetMode(TITLE_MENU);
	}

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

	Renderer2D_DrawText(text, FONT_NAME, position, s_Data.Color, FONT_SIZE);

	s_Data.CurrentFrameTime += timeStep;
}

void StartUp_Destroy()
{}