#include "FadeInOut.h"
#include "Core.h"

struct FadeInOutData
{
	RectTransformComponent CanvasRect = {
		{ 0.0f,0.0f },0,
		{ 1920.0f, 1080.0f },
	};
	SpriteRendererComponent CanvasSprite = {
		{ 0.5f, 0.5f, 0.5f, 1.0f },
	};

	FadeInOutMode CurrentMode = FADE_NONE;
	float FadeRate = 0.02f;
};
static FadeInOutData s_Data;

void FadeInOut_Initialize()
{
	s_Data = {};
}

void FadeInOut_Reset(FadeInOutMode mode)
{
	s_Data.CurrentMode = mode;
	s_Data.CanvasSprite.Color = { 0.5f, 0.5f, 0.5f, 1.0f };
}

void FadeInOut_Draw(float timeStep)
{
	if (s_Data.CurrentMode != FADE_NONE)
	{
		if (s_Data.CurrentMode == FADE_OUT)
		{
			s_Data.CanvasSprite.Color.w += s_Data.FadeRate;
			if (s_Data.CanvasSprite.Color.w >= 1.0f)
			{
				s_Data.CanvasSprite.Color.w = 1.0f;
				s_Data.CurrentMode = FADE_IN;
			}
		}
		else if (s_Data.CurrentMode == FADE_IN)
		{
			s_Data.CanvasSprite.Color.w -= s_Data.FadeRate;
			if (s_Data.CanvasSprite.Color.w >= 1.0f)
			{
				s_Data.CanvasSprite.Color.w = 1.0f;
				s_Data.CurrentMode = FADE_NONE;
			}
		}

		Vec2 position, size, ndcPos;
		Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
		RectTransformComponent_GetPositionAndSize(s_Data.CanvasRect, viewPortSize, &ndcPos, &position, &size);
		Renderer2D_DrawUI(ndcPos, size, s_Data.CanvasRect.Rotation, s_Data.CanvasSprite.Color);
	}
}