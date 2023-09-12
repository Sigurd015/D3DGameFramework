#include "LightController.h"

#define ANIMATION_FRAME_SPEED 0.1f

void LightController_OnCreate(Entity* entity, void* runtimeData)
{
	LightData* data = (LightData*)runtimeData;

	data->SpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);
	CORE_ASSERT(data->SpriteRenderer, "Entity does not have SpriteRendererComponent!");

	SpriteAnimatorSpecification spec;
	Texture2D* texture = (Texture2D*)RefPtr_Get(data->SpriteRenderer->Texture);
	spec.TextureWidth = Texture2D_GetWidth(texture);
	spec.TextureHeight = Texture2D_GetHeight(texture);

	switch (data->Type)
	{
	case LightType::RED:
	{
		spec.ElementsCount = 4;
		spec.ElementsPerColumn = 4;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 160;
		spec.ElementHeight = 608;
		spec.StartElement = { 0,0 };

		break;
	}
	case LightType::GREEN:
	{
		spec.ElementsCount = 4;
		spec.ElementsPerColumn = 4;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 160;
		spec.ElementHeight = 570;
		spec.StartElement = { 0,0 };
		break;
	}
	}

	SpriteAnimator_Create(data->SpriteAnimator, spec);
	SpriteTimer_Create(data->SpriteTimer, 4, ANIMATION_FRAME_SPEED);
}

void LightController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	LightData* data = (LightData*)runtimeData;

	SpriteTimer_Update(data->SpriteTimer, timeStep);
	SpriteElement* spriteElement = SpriteAnimator_GetElement(data->SpriteAnimator, SpriteTimer_GetFrame(data->SpriteTimer));
	data->SpriteRenderer->UVStart = spriteElement->UVStart;
	data->SpriteRenderer->UVEnd = spriteElement->UVEnd;
}

void LightController_OnDestroy(Entity* entity, void* runtimeData)
{}
void LightController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{}
void LightController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void LightController_OnEnable(Entity* entity, void* runtimeData)
{}
void LightController_OnDisable(Entity* entity, void* runtimeData)
{}