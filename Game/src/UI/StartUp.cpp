#include "Core.h"
#include "StartUp.h"
#include "Common.h"

struct StartUpData
{
	Canvas Canvas;

	SpriteAnimatorSpecification SpriteAnimatorSpecification;
	SpriteAnimator SpriteAnimator;
	uint32_t CurrentFrame = 0;

	float FrameTimeCount = 0.5f;
	float CurrentFrameTime = 0.0f;
};
static StartUpData s_Data;

void StartUp_Initialize()
{
	s_Data.Canvas.RectTransform.Position = { 500.0f,500.0f };
	s_Data.Canvas.RectTransform.Size = { 50.0f, 50.0f };
	s_Data.Canvas.SpriteRenderer.Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	s_Data.SpriteAnimatorSpecification.ElementsCount = 27;
	s_Data.SpriteAnimatorSpecification.ElementsPerColumn = 6;
	s_Data.SpriteAnimatorSpecification.ElementsPerRow = 5;
	s_Data.SpriteAnimatorSpecification.ElementWidth = 64;
	s_Data.SpriteAnimatorSpecification.ElementHeight = 64;
	s_Data.SpriteAnimatorSpecification.TexturePath = "assets/textures/loading.png";
	s_Data.SpriteAnimatorSpecification.Flip = true;
	SpriteAnimator_Create(s_Data.SpriteAnimator, s_Data.SpriteAnimatorSpecification);

	s_Data.Canvas.SpriteRenderer.Texture = &s_Data.SpriteAnimator.Sprite;
	SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, s_Data.CurrentFrame);
	s_Data.Canvas.SpriteRenderer.UVStart = spriteElement->UVStart;
	s_Data.Canvas.SpriteRenderer.UVEnd = spriteElement->UVEnd;

	s_Data.FrameTimeCount = 0.1f;
}

void StartUp_Draw(float timeStep)
{
	Vec2 position, size, ndcPos;
	Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };
	RectTransformComponent_GetPositionAndSize(s_Data.Canvas.RectTransform, viewPortSize, &ndcPos, &position, &size);

	Renderer2D_BeginScene(DirectX::XMMatrixIdentity());

	if (s_Data.CurrentFrameTime >= s_Data.FrameTimeCount)
	{
		s_Data.CurrentFrameTime = 0.0f;
		s_Data.CurrentFrame = (s_Data.CurrentFrame + 1) % s_Data.SpriteAnimatorSpecification.ElementsCount;
		SpriteElement* spriteElement = SpriteAnimator_GetElement(s_Data.SpriteAnimator, s_Data.CurrentFrame);
		s_Data.Canvas.SpriteRenderer.UVStart = spriteElement->UVStart;
		s_Data.Canvas.SpriteRenderer.UVEnd = spriteElement->UVEnd;
	}

	Renderer2D_DrawUI(ndcPos, size, *s_Data.Canvas.SpriteRenderer.Texture,
		s_Data.Canvas.SpriteRenderer.UVStart, s_Data.Canvas.SpriteRenderer.UVEnd,
		s_Data.Canvas.SpriteRenderer.Color, s_Data.Canvas.SpriteRenderer.TilingFactor);

	//s_Data.Canvas.SpriteRenderer.Color.w = FloatLerp(s_Data.Canvas.SpriteRenderer.Color.w, 0.5f, timeStep);

	//Renderer2D_DrawUI(ndcPos, size, s_Data.Canvas.SpriteRenderer.Color);

	Renderer2D_EndScene();

	s_Data.CurrentFrameTime += timeStep;
}

void StartUp_Destroy()
{

}