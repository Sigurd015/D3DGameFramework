#include "pch.h"
#include "SpriteAnimtor.h"
#include "Core/Base.h"

void SpriteAnimator_Create(SpriteAnimator& animtor, const SpriteAnimatorSpecification& spec)
{
	Texture2D_Create(animtor.Sprite, spec.TexturePath);
	List_Create(animtor.Elements, spec.ElementsCount);

	uint32_t index = 0;
	uint32_t textureWidth = Texture2D_GetWidth(animtor.Sprite);
	uint32_t textureHeight = Texture2D_GetHeight(animtor.Sprite);
	for (size_t i = 0; i < spec.ElementsPerRow; i++)
	{
		for (size_t j = 0; j < spec.ElementsPerColumn; j++)
		{
			if (index >= spec.ElementsCount)
				break;

			SpriteElement* element = (SpriteElement*)malloc(sizeof(SpriteElement));

			float elementWidth = (float)(i * spec.ElementWidth);
			float elementHeight = (float)(j * spec.ElementHeight);

			if (spec.Flip)
			{
				element->UVEnd = { elementWidth / (float)textureWidth,
								elementHeight / (float)textureHeight };
				element->UVStart = { (float)(elementWidth + spec.ElementWidth) / (float)textureWidth,
					(float)(elementHeight + spec.ElementHeight) / (float)textureHeight, };
			}
			else
			{
				element->UVStart = { elementWidth / (float)textureWidth,
				elementHeight / (float)textureHeight };
				element->UVEnd = { (float)(elementWidth + spec.ElementWidth) / (float)textureWidth,
					(float)(elementHeight + spec.ElementHeight) / (float)textureHeight, };
			}

			List_Add(animtor.Elements, element);
			index++;
		}
	}
}

void SpriteAnimator_Destroy(SpriteAnimator& animtor)
{
	Texture2D_Release(animtor.Sprite);
	List_Free(animtor.Elements, true);
}

SpriteElement* SpriteAnimator_GetElement(const SpriteAnimator& animtor, uint32_t index)
{
	BV_ASSERT(index < List_Size(animtor.Elements), "SpriteAnimator_GetElement : Index out of range");

	return (SpriteElement*)List_Get(animtor.Elements, index);
}