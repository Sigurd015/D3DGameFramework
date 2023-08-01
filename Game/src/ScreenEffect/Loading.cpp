#include "Core.h"
#include "Loading.h"
#include "../GameMode.h"

struct StartUpData
{
	RectTransformComponent CanvasRect;
	SpriteRendererComponent CanvasSprite;

	Texture2D LoadingTexture;
	SpriteAnimatorSpecification SpriteAnimatorSpecification;
	SpriteAnimator SpriteAnimator;

	uint32_t CurrentFrame = 0;
	float CurrentFrameCount = 0.0f;
	float MaxFrameCount = 0.015f;
	float CurrentFrameTime = 0.0f;
	float MaxFrameTime = 3.0f;

	GameMode Depature = STARTUP_TITLE;
};
static StartUpData s_Data;

void Loading_Initialize()
{
	s_Data.CanvasRect.Position = { 1820.0f,50.0f };
	s_Data.CanvasRect.Size = { 50.0f, 50.0f };
	s_Data.CanvasSprite.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	s_Data.SpriteAnimatorSpecification.TextureWidth = 320;
	s_Data.SpriteAnimatorSpecification.TextureHeight = 384;
	s_Data.SpriteAnimatorSpecification.ElementsCount = 27;
	s_Data.SpriteAnimatorSpecification.ElementsPerColumn = 6;
	s_Data.SpriteAnimatorSpecification.ElementsPerRow = 5;
	s_Data.SpriteAnimatorSpecification.ElementWidth = 64;
	s_Data.SpriteAnimatorSpecification.ElementHeight = 64;
	s_Data.SpriteAnimatorSpecification.StartElement = { 0,0 };

	Texture2D_Create(s_Data.LoadingTexture, "assets/textures/loading.png");
	SpriteAnimator_Create(s_Data.SpriteAnimator, s_Data.SpriteAnimatorSpecification);

	s_Data.MaxFrameCount = 0.015f;
	s_Data.MaxFrameTime = 3.0f;
	s_Data.Depature = STARTUP_TITLE;
	Loading_Reset();
}

void Loading_Reset()
{
	s_Data.CurrentFrame = 0;
	s_Data.CurrentFrameTime = 0.0f;
	s_Data.CurrentFrameCount = 0.0f;
	s_Data.CanvasSprite.Texture = &s_Data.LoadingTexture;
	SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, s_Data.CurrentFrame);
	s_Data.CanvasSprite.UVStart = { spriteElement->UVStart.x,spriteElement->UVEnd.y };
	s_Data.CanvasSprite.UVEnd = { spriteElement->UVEnd.x,spriteElement->UVStart.y };
}

void Loading_Draw(float timeStep)
{
	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
	RectTransformComponent_GetPositionAndSize(s_Data.CanvasRect, viewPortSize, &ndcPos, &position, &size);

	if (s_Data.CurrentFrameCount >= s_Data.MaxFrameCount)
	{
		s_Data.CurrentFrameCount = 0.0f;
		s_Data.CurrentFrame = (s_Data.CurrentFrame + 1) % s_Data.SpriteAnimatorSpecification.ElementsCount;
		SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, s_Data.CurrentFrame);
		s_Data.CanvasSprite.UVStart = { spriteElement->UVStart.x,spriteElement->UVEnd.y };
		s_Data.CanvasSprite.UVEnd = { spriteElement->UVEnd.x,spriteElement->UVStart.y };
	}

	Renderer2D_DrawUI(ndcPos, size, *s_Data.CanvasSprite.Texture,
		s_Data.CanvasSprite.UVStart, s_Data.CanvasSprite.UVEnd,
		s_Data.CanvasSprite.Color, s_Data.CanvasSprite.TilingFactor);

	s_Data.CurrentFrameCount += timeStep;
	s_Data.CurrentFrameTime += timeStep;

	if (s_Data.CurrentFrameTime > s_Data.MaxFrameTime)
	{
		Game_SetMode(s_Data.Depature);
	}
}

void Loading_SetDepature(GameMode depature)
{
	s_Data.Depature = depature;
}

void Loading_Destroy()
{
	Texture2D_Release(s_Data.LoadingTexture);
}