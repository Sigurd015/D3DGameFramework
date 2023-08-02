#include "Core.h"
#include "Loading.h"
#include "../GameMode.h"

struct StartUpData
{
	RectTransformComponent CanvasRect = {
		{ 1820.0f,50.0f },
		{ 50.0f, 50.0f },
	};
	SpriteRendererComponent CanvasSprite = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
	};

	Texture2D LoadingTexture;
	SpriteAnimator SpriteAnimator;
	float SpriteAnimatorSpeed = 0.15f;

	float CurrentTime = 0.0f;
	float MaxTime = 2.5f;

	GameMode Depature = STARTUP_TITLE;
};
static StartUpData s_Data;

void Loading_Initialize()
{
	s_Data = {};

	Texture2D_Create(s_Data.LoadingTexture, "assets/textures/loading.png");
	s_Data.CanvasSprite.Texture = &s_Data.LoadingTexture;

	SpriteAnimatorSpecification spec;
	spec.TextureWidth = Texture2D_GetWidth(s_Data.LoadingTexture);
	spec.TextureHeight = Texture2D_GetHeight(s_Data.LoadingTexture);
	spec.ElementsCount = 17;
	spec.ElementsPerColumn = 4;
	spec.ElementsPerRow = 5;
	spec.ElementWidth = 64;
	spec.ElementHeight = 64;
	spec.StartElement = { 0,0 };

	SpriteAnimator_Create(s_Data.SpriteAnimator, spec);

	Loading_Reset();
}

void Loading_Reset()
{
	s_Data.CurrentTime = 0.0f;
	SpriteAnimator_Reset(s_Data.SpriteAnimator, s_Data.SpriteAnimatorSpeed);
}

void Loading_Draw(float timeStep)
{
	if (s_Data.CurrentTime > s_Data.MaxTime)
	{
		Game_SetMode(s_Data.Depature);
	}

	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
	RectTransformComponent_GetPositionAndSize(s_Data.CanvasRect, viewPortSize, &ndcPos, &position, &size);

	SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, timeStep);
	s_Data.CanvasSprite.UVStart = spriteElement->UVStart;
	s_Data.CanvasSprite.UVEnd = spriteElement->UVEnd;

	Renderer2D_DrawUI(ndcPos, size, *s_Data.CanvasSprite.Texture,
		s_Data.CanvasSprite.UVStart, s_Data.CanvasSprite.UVEnd,
		s_Data.CanvasSprite.Color, s_Data.CanvasSprite.TilingFactor);

	s_Data.CurrentTime += timeStep;
}

void Loading_SetDepature(GameMode depature)
{
	s_Data.Depature = depature;
}

void Loading_Destroy()
{
	Texture2D_Release(s_Data.LoadingTexture);
}