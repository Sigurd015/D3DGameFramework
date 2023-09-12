#include "Core.h"
#include "Loading.h"

#define LOADING_FRAME_SPEED 0.15f

struct StartUpData
{
	RectTransformComponent CanvasRect = {
		{ 1820.0f,50.0f },
		{ 50.0f, 50.0f },
	};
	SpriteRendererComponent CanvasSprite = {
		{ 1.0f, 1.0f, 1.0f, 1.0f },
	};

	RefPtr* LoadingTexture;
	SpriteAnimator SpriteAnimator;
	SpriteTimer SpriteTimer;

	float CurrentTime = 0.0f;
	float MaxTime = 3.0f;

	GameMode Depature = STARTUP_TITLE;
};
static StartUpData s_Data;

void Loading_Initialize()
{
	s_Data = {};

	Texture2D texture;
	Texture2D_Create(&texture, "assets/textures/loading.png");
	RefPtr* loadingTexture = RefPtr_Create(sizeof(Texture2D), &texture);
	s_Data.LoadingTexture = RefPtr_AddRef(loadingTexture);
	s_Data.CanvasSprite.Texture = s_Data.LoadingTexture;

	SpriteAnimatorSpecification spec;
	spec.TextureWidth = Texture2D_GetWidth(&texture);
	spec.TextureHeight = Texture2D_GetHeight(&texture);
	spec.ElementsCount = 4;
	spec.ElementsPerColumn = 4;
	spec.ElementsPerRow = 1;
	spec.ElementWidth = 78;
	spec.ElementHeight = 78;
	spec.StartElement = { 0,0 };

	SpriteAnimator_Create(s_Data.SpriteAnimator, spec);
	SpriteTimer_Create(s_Data.SpriteTimer, spec.ElementsCount, LOADING_FRAME_SPEED);

	Loading_Reset();
}

void Loading_Reset(float maxTime)
{
	SpriteTimer_Reset(s_Data.SpriteTimer);
	s_Data.CurrentTime = 0.0f;
	s_Data.MaxTime = maxTime;
}

void Loading_Draw(float timeStep)
{
	if (s_Data.CurrentTime > s_Data.MaxTime)
	{
		Game_SetMode(s_Data.Depature);
	}

	SpriteTimer_Update(s_Data.SpriteTimer, timeStep);
	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
	RectTransformComponent_GetPositionAndSize(s_Data.CanvasRect, viewPortSize, &ndcPos, &position, &size);

	SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, SpriteTimer_GetFrame(s_Data.SpriteTimer));
	s_Data.CanvasSprite.UVStart = spriteElement->UVStart;
	s_Data.CanvasSprite.UVEnd = spriteElement->UVEnd;

	Renderer2D_DrawUI(ndcPos, size, s_Data.CanvasSprite.Texture,
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
	RefPtr_Release(s_Data.LoadingTexture, [](void* ptr)
		{
			Texture2D_Release((Texture2D*)ptr);
		});
}