#include "FieldController.h"

#include <stdio.h>

#define ENTITY_COUNT_PER_FILED_ELEMENT 14
#define FIELD_OFFSET_Y 60.0f

enum FieldElementType
{
	LEFT,
	RIGHT,
	END,
};

struct FieldControllerData
{
	Entity* LeftEntity[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* LeftTrans[ENTITY_COUNT_PER_FILED_ELEMENT];

	Entity* RightEntity[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* RightTrans[ENTITY_COUNT_PER_FILED_ELEMENT];

	Entity* EndEntity[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* EndTrans[ENTITY_COUNT_PER_FILED_ELEMENT];

	FieldElementType CurrentType = FieldElementType::RIGHT;
};
static FieldControllerData s_Data;

void FieldController_OnCreate(Entity& entity)
{
	s_Data = {};
	// Init map
	{
		// Left
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "left-wall-%d", i + 1);
				s_Data.LeftEntity[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.LeftEntity[i], "Cannot find map entity!");
				s_Data.LeftTrans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.LeftEntity[i], ComponentType_Transform);
			}
		}

		// Right
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "right-wall-%d", i + 1);
				s_Data.RightEntity[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.RightEntity[i], "Cannot find map entity!");
				s_Data.RightTrans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.RightEntity[i], ComponentType_Transform);
			}
		}

		// End
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "end-wall-%d", i + 1);
				s_Data.EndEntity[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.EndEntity[i], "Cannot find map entity!");
				s_Data.EndTrans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.EndEntity[i], ComponentType_Transform);
				s_Data.EndEntity[i]->Enabled = false;
			}
		}
	}
}

void FieldController_OnUpdate(Entity& entity, float timeStep)
{}

void FieldController_OnDestroy(Entity& entity)
{}

void FieldController_OnCollision(Entity& entity, Entity& other)
{}

void FieldController_GenMap()
{
	switch (s_Data.CurrentType)
	{
	case FieldElementType::LEFT:
	{
		s_Data.CurrentType = FieldElementType::RIGHT;
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(*s_Data.LeftEntity[i], ComponentType_Rigidbody2D))
			{
				Vec2 movement = { 0, FIELD_OFFSET_Y };
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(*s_Data.LeftEntity[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(*rigidbody, movement);
			}
			else
			{
				s_Data.LeftTrans[i]->Translation.y += FIELD_OFFSET_Y;
			}

			Scene_SetEntityEnabled(*s_Data.LeftEntity[i], true);
		}
		Scene_SetEntityEnabled(*s_Data.LeftEntity[ENTITY_COUNT_PER_FILED_ELEMENT - 1], false);
		Scene_SetEntityEnabled(*s_Data.RightEntity[ENTITY_COUNT_PER_FILED_ELEMENT - 1], true);
		break;
	}
	case FieldElementType::RIGHT:
	{
		s_Data.CurrentType = FieldElementType::LEFT;
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(*s_Data.RightEntity[i], ComponentType_Rigidbody2D))
			{
				Vec2 movement = { 0, FIELD_OFFSET_Y };
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(*s_Data.RightEntity[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(*rigidbody, movement);
			}
			else
			{
				s_Data.RightTrans[i]->Translation.y += FIELD_OFFSET_Y;
			}

			Scene_SetEntityEnabled(*s_Data.RightEntity[i], true);
		}
		Scene_SetEntityEnabled(*s_Data.LeftEntity[ENTITY_COUNT_PER_FILED_ELEMENT - 1], true);
		Scene_SetEntityEnabled(*s_Data.RightEntity[ENTITY_COUNT_PER_FILED_ELEMENT - 1], false);
		break;
	}
	}
}