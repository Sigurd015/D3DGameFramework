#include "pch.h"
#include "SpriteAnimtor.h"
#include "Core/Base.h"

void SpriteAnimator_Create(SpriteAnimator& animtor, const SpriteAnimatorSpecification& spec)
{
	List_Create(animtor.Elements, sizeof(SpriteElement), spec.ElementsCount);

	uint32_t index = 0;
	for (size_t i = spec.StartElement.y; i < spec.ElementsPerRow; i++)
	{
		for (size_t j = spec.StartElement.x; j < spec.ElementsPerColumn; j++)
		{
			if (index >= spec.ElementsCount)
				break;

			SpriteElement element;

			float elementWidth = (float)(j * spec.ElementWidth);
			float elementHeight = (float)(i * spec.ElementHeight);

			element.UVStart = { elementWidth / (float)spec.TextureWidth,elementHeight / (float)spec.TextureHeight };
			element.UVEnd = { (float)(elementWidth + spec.ElementWidth) / (float)spec.TextureWidth,
				(float)(elementHeight + spec.ElementHeight) / (float)spec.TextureHeight };

			List_Add(animtor.Elements, &element);
			index++;
		}
	}
}

void SpriteAnimator_Destroy(SpriteAnimator& animtor)
{
	List_Free(animtor.Elements);
}

SpriteElement* SpriteAnimator_GetElement(SpriteAnimator& animtor, uint32_t index)
{
	CORE_ASSERT(index < List_Size(animtor.Elements), "Index out of range");
	return (SpriteElement*)List_Get(animtor.Elements, index);
}
