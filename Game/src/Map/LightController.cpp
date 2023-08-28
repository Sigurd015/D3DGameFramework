#include "LightController.h"

#define ANIMATION_FRAME_SPEED 0.1f

void LightController_OnCreate(Entity& entity, void* runtimeData)
{
	LightData* data = (LightData*)runtimeData;

	data->SpriteRenderer = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);
	CORE_ASSERT(data->SpriteRenderer, "Entity does not have SpriteRendererComponent!");

	data->SpriteSheet = data->SpriteRenderer->Texture;

	switch (data->Type)
	{
	case LightType::RED:
	{
		SpriteAnimatorSpecification spec;
		spec.TextureWidth = Texture2D_GetWidth(*data->SpriteSheet);
		spec.TextureHeight = Texture2D_GetHeight(*data->SpriteSheet);
		spec.ElementsCount = 4;
		spec.ElementsPerColumn = 4;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 160;
		spec.ElementHeight = 608;
		spec.StartElement = { 0,0 };

		SpriteAnimator_Create(data->SpriteAnimator, spec);
		SpriteTimer_Create(data->SpriteTimer, 4, ANIMATION_FRAME_SPEED);
		break;
	}
	case LightType::GREEN:
	{
		SpriteAnimatorSpecification spec;
		spec.TextureWidth = Texture2D_GetWidth(*data->SpriteSheet);
		spec.TextureHeight = Texture2D_GetHeight(*data->SpriteSheet);
		spec.ElementsCount = 4;
		spec.ElementsPerColumn = 4;
		spec.ElementsPerRow = 1;
		spec.ElementWidth = 160;
		spec.ElementHeight = 570;
		spec.StartElement = { 0,0 };

		SpriteAnimator_Create(data->SpriteAnimator, spec);
		SpriteTimer_Create(data->SpriteTimer, 4, ANIMATION_FRAME_SPEED);
		break;
	}
	}
}

void LightController_OnUpdate(Entity& entity, float timeStep, void* runtimeData)
{
	LightData* data = (LightData*)runtimeData;

	SpriteTimer_Update(data->SpriteTimer, timeStep);
	uint32_t currentFrame = SpriteTimer_GetFrame(data->SpriteTimer);

	data->SpriteRenderer->Texture = data->SpriteSheet;
	SpriteElement* spriteElement = SpriteAnimator_GetElement(data->SpriteAnimator, SpriteTimer_GetFrame(data->SpriteTimer));
	data->SpriteRenderer->UVStart = spriteElement->UVStart;
	data->SpriteRenderer->UVEnd = spriteElement->UVEnd;
}

void LightController_OnDestroy(Entity& entity, void* runtimeData)
{}
void LightController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}
void LightController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{}
void LightController_OnEnable(Entity& entity, void* runtimeData)
{}
void LightController_OnDisable(Entity& entity, void* runtimeData)
{}