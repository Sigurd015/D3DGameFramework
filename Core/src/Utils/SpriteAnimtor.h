#pragma once
#include "Renderer/Texture.h"
#include "Math/Math.h"
#include "List.h"

struct SpriteAnimatorSpecification
{
	char* TexturePath;

	uint32_t ElementWidth;
	uint32_t ElementHeight;
	uint32_t ElementsCount;

	uint32_t ElementsPerRow;
	uint32_t ElementsPerColumn;

	bool Flip;
};

struct SpriteElement
{
	Vec2 UVStart;
	Vec2 UVEnd;
};

struct SpriteAnimator
{
	Texture2D Sprite;
	List Elements;
};

void SpriteAnimator_Create(SpriteAnimator& animtor, const SpriteAnimatorSpecification& spec);
void SpriteAnimator_Destroy(SpriteAnimator& animtor);
SpriteElement* SpriteAnimator_GetElement(const SpriteAnimator& animtor, uint32_t index);