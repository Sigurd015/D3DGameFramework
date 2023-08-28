#include "FieldController.h"

#include <stdio.h>

#define ENTITY_COUNT_PER_FILED_ELEMENT_WALL 21
#define ENTITY_COUNT_PER_FILED_ELEMENT_LIGHT 2
#define ENTITY_COUNT_PER_FILED_ELEMENT (ENTITY_COUNT_PER_FILED_ELEMENT_WALL+ENTITY_COUNT_PER_FILED_ELEMENT_LIGHT)
#define FIELD_OFFSET_Y 120.0f
#define MAX_EMEMY_COUNT 10

enum FieldElementType
{
	MAP_ELEMENT_1,
	MAP_ELEMENT_2,
};

struct FieldControllerData
{
	Entity* Element1Ents[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* Element1Trans[ENTITY_COUNT_PER_FILED_ELEMENT];

	Entity* Element2Ents[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* Element2Trans[ENTITY_COUNT_PER_FILED_ELEMENT];

	Entity* BackWallEntity = nullptr;
	Rigidbody2DComponent* BackWallRigidbody = nullptr;

	Entity* FloorEntity = nullptr;
	TransformComponent* FloorTrans = nullptr;

	Entity* RoofEntity = nullptr;
	TransformComponent* RoofTrans = nullptr;

	FieldElementType CurrentType = FieldElementType::MAP_ELEMENT_1;

	Entity* EnemyEntity[MAX_EMEMY_COUNT];
	TransformComponent* EnemyTrans[MAX_EMEMY_COUNT];
};
static FieldControllerData s_Data;

void FieldController_OnCreate(Entity& entity, void* runtimeData)
{
	s_Data = {};
	// Init map
	{
		// Back Wall
		{
			s_Data.BackWallEntity = Scene_GetEntityByName(*entity.Scene, "BackWall");
			CORE_ASSERT(s_Data.BackWallEntity, "Cannot find map entity!");
			s_Data.BackWallRigidbody = (Rigidbody2DComponent*)Entity_GetComponent(*s_Data.BackWallEntity, ComponentType_Rigidbody2D);
		}

		//Floor
		{
			s_Data.FloorEntity = Scene_GetEntityByName(*entity.Scene, "Floor");
			CORE_ASSERT(s_Data.FloorEntity, "Cannot find map entity!");
			s_Data.FloorTrans = (TransformComponent*)Entity_GetComponent(*s_Data.FloorEntity, ComponentType_Transform);
		}

		//Roof
		{
			s_Data.RoofEntity = Scene_GetEntityByName(*entity.Scene, "Roof");
			CORE_ASSERT(s_Data.RoofEntity, "Cannot find map entity!");
			s_Data.RoofTrans = (TransformComponent*)Entity_GetComponent(*s_Data.RoofEntity, ComponentType_Transform);
		}
		// MAP_Element_1
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_1_Wall_%d", i + 1);
				s_Data.Element1Ents[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.Element1Ents[i], "Cannot find map entity!");
				s_Data.Element1Trans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.Element1Ents[i], ComponentType_Transform);
			}
			uint32_t lightIndex = 0;
			for (size_t i = ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_1_Light_%d", lightIndex + 1);
				s_Data.Element1Ents[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.Element1Ents[i], "Cannot find map entity!");
				s_Data.Element1Trans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.Element1Ents[i], ComponentType_Transform);
				lightIndex++;
			}
		}

		// MAP_Element_2
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_2_Wall_%d", i + 1);
				s_Data.Element2Ents[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.Element2Ents[i], "Cannot find map entity!");
				s_Data.Element2Trans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.Element2Ents[i], ComponentType_Transform);
			}
			uint32_t lightIndex = 0;
			for (size_t i = ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_2_Light_%d", lightIndex + 1);
				s_Data.Element2Ents[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
				CORE_ASSERT(s_Data.Element2Ents[i], "Cannot find map entity!");
				s_Data.Element2Trans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.Element2Ents[i], ComponentType_Transform);
				lightIndex++;
			}
		}
	}

	////Init Emeny
	//{
	//	for (size_t i = 0; i < MAX_EMEMY_COUNT; i++)
	//	{
	//		char tempChar[256];
	//		sprintf_s(tempChar, 256, "Enemy-%d", i + 1);
	//		s_Data.EnemyEntity[i] = Scene_GetEntityByName(*entity.Scene, tempChar);
	//		CORE_ASSERT(s_Data.EnemyEntity[i], "Cannot find map entity!");
	//		s_Data.EnemyTrans[i] = (TransformComponent*)Entity_GetComponent(*s_Data.EnemyEntity[i], ComponentType_Transform);
	//	}
	//}
}

void FieldController_OnUpdate(Entity& entity, float timeStep, void* runtimeData)
{
	//#ifndef CORE_DIST
	//if (Input_GetKeyDown(KeyCode::P))
	//{
	//	for (size_t i = 0; i < MAX_EMEMY_COUNT; i++)
	//	{
	//		Scene_SetEntityEnabled(*s_Data.EnemyEntity[i], !s_Data.EnemyEntity[i]->Enabled);
	//	}
	//}
	//#endif 
}
void FieldController_OnDestroy(Entity& entity, void* runtimeData)
{}
void FieldController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}
void FieldController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{}
void FieldController_OnEnable(Entity& entity, void* runtimeData)
{}
void FieldController_OnDisable(Entity& entity, void* runtimeData)
{}

void FieldController_GenMap()
{
	switch (s_Data.CurrentType)
	{
	case FieldElementType::MAP_ELEMENT_1:
	{
		s_Data.CurrentType = FieldElementType::MAP_ELEMENT_2;
		Vec2 movement = { 0, FIELD_OFFSET_Y };
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(*s_Data.Element1Ents[i], ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(*s_Data.Element1Ents[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(*rigidbody, movement);
			}
			else
			{
				s_Data.Element1Trans[i]->Translation.y += FIELD_OFFSET_Y;
			}
		}
		Rigidbody2DComponent_MovePosition(*s_Data.BackWallRigidbody, Vec2DivFloat(movement, 2.0f));
		s_Data.FloorTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
		s_Data.RoofTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
		break;
	}
	case FieldElementType::MAP_ELEMENT_2:
	{
		s_Data.CurrentType = FieldElementType::MAP_ELEMENT_1;
		Vec2 movement = { 0, FIELD_OFFSET_Y };
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(*s_Data.Element2Ents[i], ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(*s_Data.Element2Ents[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(*rigidbody, movement);
			}
			else
			{
				s_Data.Element2Trans[i]->Translation.y += FIELD_OFFSET_Y;
			}
		}
		Rigidbody2DComponent_MovePosition(*s_Data.BackWallRigidbody, Vec2DivFloat(movement, 2.0f));
		s_Data.FloorTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
		s_Data.RoofTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
		break;
	}
	}
}