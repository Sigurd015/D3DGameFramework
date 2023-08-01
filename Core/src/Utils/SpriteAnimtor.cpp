#include "pch.h"
#include "SpriteAnimtor.h"
#include "Core/Base.h"

void SpriteAnimator_Create(SpriteAnimator& animtor, const SpriteAnimatorSpecification& spec)
{
	List_Create(animtor.Elements, spec.ElementsCount);

	uint32_t index = 0;
	for (size_t i = spec.StartElement.y; i < spec.ElementsPerRow; i++)
	{
		for (size_t j = spec.StartElement.x; j < spec.ElementsPerColumn; j++)
		{
			if (index >= spec.ElementsCount)
				break;

			SpriteElement* element = (SpriteElement*)malloc(sizeof(SpriteElement));

			float elementWidth = (float)(j * spec.ElementWidth);
			float elementHeight = (float)(i * spec.ElementHeight);

			element->UVStart = { elementWidth / (float)spec.TextureWidth, elementHeight / (float)spec.TextureHeight };
			element->UVEnd = { (float)(elementWidth + spec.ElementWidth) / (float)spec.TextureWidth,
				(float)(elementHeight + spec.ElementHeight) / (float)spec.TextureHeight, };

			List_Add(animtor.Elements, element);
			index++;
		}
	}
}

void SpriteAnimator_Destroy(SpriteAnimator& animtor)
{
	List_Free(animtor.Elements, true);
}

SpriteElement* SpriteAnimator_GetElement(const SpriteAnimator& animtor, uint32_t index)
{
	BV_ASSERT(index < List_Size(animtor.Elements), "SpriteAnimator_GetElement : Index out of range");

	return (SpriteElement*)List_Get(animtor.Elements, index);
}