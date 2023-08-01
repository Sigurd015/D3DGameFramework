#include "FadeInOut.h"
#include "Core.h"

struct FadeInOutData
{
	RectTransformComponent CanvasRect;
	SpriteRendererComponent CanvasSprite;

	FadeInOutMode CurrentMode;
	float FadeRate = 0.1f;
};
static FadeInOutData s_Data;

void FadeInOut_Initialize()
{
	s_Data.CanvasRect.Position = { 0,0 };
	s_Data.CanvasRect.Size = { 1920.0f,1080.0f };
	s_Data.CanvasSprite.Color = { 0.5f, 0.5f, 0.5f, 1.0f };

	s_Data.CurrentMode = FADE_NONE;
	s_Data.FadeRate = 0.02f;
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
		Renderer2D_DrawUI(ndcPos, size, s_Data.CanvasSprite.Color);
	}
}