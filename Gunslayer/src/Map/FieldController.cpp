#include "FieldController.h"
#include "Player/PlayerController.h"

#include <stdio.h>

#define ENTITY_COUNT_BACK_WALL 7
#define ENTITY_COUNT_PER_FILED_ELEMENT_WALL 21
#define ENTITY_COUNT_PER_FILED_ELEMENT_LIGHT 2
#define ENTITY_COUNT_PER_FILED_ELEMENT (ENTITY_COUNT_PER_FILED_ELEMENT_WALL+ENTITY_COUNT_PER_FILED_ELEMENT_LIGHT)
#define FIELD_OFFSET_Y 120.0f

// Make sure three type of enemy count is same
#define ENEMY_COUNT 1*3

#define MEDKIT_DROP_HP_PRECENT 0.4f

enum FieldElementType
{
	MAP_ELEMENT_1,
	MAP_ELEMENT_2,
};

struct FieldControllerData
{
	Entity* Element1Ents[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* Element1Trans[ENTITY_COUNT_PER_FILED_ELEMENT];
	Vec2 EnemySpawnPos = { 15.0f,70.0f };

	Entity* Element2Ents[ENTITY_COUNT_PER_FILED_ELEMENT];
	TransformComponent* Element2Trans[ENTITY_COUNT_PER_FILED_ELEMENT];
	Vec3 Element2EnemySpawnPos = { 0,0,0 };

	Entity* BackWallEnts[ENTITY_COUNT_BACK_WALL];
	TransformComponent* BackWallTrans[ENTITY_COUNT_BACK_WALL];

	Entity* FloorEntity = nullptr;
	TransformComponent* FloorTrans = nullptr;

	Entity* RoofEntity = nullptr;
	TransformComponent* RoofTrans = nullptr;

	FieldElementType CurrentType = FieldElementType::MAP_ELEMENT_1;

	Entity* EnemyEntity[ENEMY_COUNT];
	Rigidbody2DComponent* EnemyRigidbody[ENEMY_COUNT];

	Entity* ItemEntity;
	Rigidbody2DComponent* ItemRigidbody;
};
static FieldControllerData s_Data;

void FieldController_OnCreate(Entity* entity, void* runtimeData)
{
	s_Data = {};
	// Init map
	{
		// Back Wall
		{
			for (size_t i = 0; i < ENTITY_COUNT_BACK_WALL; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "BackWall_0%d", i + 1);
				s_Data.BackWallEnts[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.BackWallEnts[i], "Cannot find map entity!");
				s_Data.BackWallTrans[i] = (TransformComponent*)Entity_GetComponent(s_Data.BackWallEnts[i], ComponentType_Transform);
			}
		}

		//Floor
		{
			s_Data.FloorEntity = Scene_GetEntityByName(entity->Scene, "Floor");
			CORE_ASSERT(s_Data.FloorEntity, "Cannot find map entity!");
			s_Data.FloorTrans = (TransformComponent*)Entity_GetComponent(s_Data.FloorEntity, ComponentType_Transform);
		}

		//Roof
		{
			s_Data.RoofEntity = Scene_GetEntityByName(entity->Scene, "Roof");
			CORE_ASSERT(s_Data.RoofEntity, "Cannot find map entity!");
			s_Data.RoofTrans = (TransformComponent*)Entity_GetComponent(s_Data.RoofEntity, ComponentType_Transform);
		}
		// MAP_Element_1
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_1_Wall_%d", i + 1);
				s_Data.Element1Ents[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.Element1Ents[i], "Cannot find map entity!");
				s_Data.Element1Trans[i] = (TransformComponent*)Entity_GetComponent(s_Data.Element1Ents[i], ComponentType_Transform);
			}
			uint32_t lightIndex = 0;
			for (size_t i = ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_1_Light_%d", lightIndex + 1);
				s_Data.Element1Ents[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.Element1Ents[i], "Cannot find map entity!");
				s_Data.Element1Trans[i] = (TransformComponent*)Entity_GetComponent(s_Data.Element1Ents[i], ComponentType_Transform);
				lightIndex++;
			}
		}

		// MAP_Element_2
		{
			for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_2_Wall_%d", i + 1);
				s_Data.Element2Ents[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.Element2Ents[i], "Cannot find map entity!");
				s_Data.Element2Trans[i] = (TransformComponent*)Entity_GetComponent(s_Data.Element2Ents[i], ComponentType_Transform);
			}
			uint32_t lightIndex = 0;
			for (size_t i = ENTITY_COUNT_PER_FILED_ELEMENT_WALL; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
			{
				char tempChar[256];
				sprintf_s(tempChar, 256, "MAP_Element_2_Light_%d", lightIndex + 1);
				s_Data.Element2Ents[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.Element2Ents[i], "Cannot find map entity!");
				s_Data.Element2Trans[i] = (TransformComponent*)Entity_GetComponent(s_Data.Element2Ents[i], ComponentType_Transform);
				lightIndex++;
			}
		}
	}

	// Init Emeny
	{
		uint32_t perTypeEnemyCount = ENEMY_COUNT / 3;
		{
			uint32_t cacodemonCount = 0;
			uint32_t cyberdemonCount = 0;
			uint32_t soldierCount = 0;
			for (size_t i = 0; i < ENEMY_COUNT; i++)
			{
				char tempChar[256];
				if (i < perTypeEnemyCount)
				{
					sprintf_s(tempChar, 256, "Enemy_Caco_Demon_%d", cacodemonCount + 1);
					cacodemonCount++;
				}
				else if (i < perTypeEnemyCount * 2)
				{
					sprintf_s(tempChar, 256, "Enemy_Cyber_Demon_%d", cyberdemonCount + 1);
					cyberdemonCount++;
				}
				else
				{
					sprintf_s(tempChar, 256, "Enemy_Soldier_%d", soldierCount + 1);
					soldierCount++;
				}
				s_Data.EnemyEntity[i] = Scene_GetEntityByName(entity->Scene, tempChar);
				CORE_ASSERT(s_Data.EnemyEntity[i], "Cannot find Enemy entity!");
				s_Data.EnemyRigidbody[i] = (Rigidbody2DComponent*)Entity_GetComponent(s_Data.EnemyEntity[i], ComponentType_Rigidbody2D);

				Scene_SetEntityEnabled(s_Data.EnemyEntity[i], false);
			}
		}
	}

	// Init Item
	{
		s_Data.ItemEntity = Scene_GetEntityByName(entity->Scene, "Item_1");
		CORE_ASSERT(s_Data.ItemEntity, "Cannot find Item entity!");
		s_Data.ItemRigidbody = (Rigidbody2DComponent*)Entity_GetComponent(s_Data.ItemEntity, ComponentType_Rigidbody2D);
		//Scene_SetEntityEnabled(s_Data.ItemEntity, false);
	}
}

void FieldController_OnUpdate(Entity* entity, float timeStep, void* runtimeData)
{
	#ifndef CORE_DIST
	{
		if (Input_GetKeyDown(KeyCode_Numpad1))
		{
			Scene_SetEntityEnabled(s_Data.EnemyEntity[0], !s_Data.EnemyEntity[0]->Enabled);
		}
		if (Input_GetKeyDown(KeyCode_Numpad2))
		{
			Scene_SetEntityEnabled(s_Data.EnemyEntity[1], !s_Data.EnemyEntity[1]->Enabled);
		}
		if (Input_GetKeyDown(KeyCode_Numpad3))
		{
			Scene_SetEntityEnabled(s_Data.EnemyEntity[1], !s_Data.EnemyEntity[1]->Enabled);
		}
	}
	#endif 
}
void FieldController_OnDestroy(Entity* entity, void* runtimeData)
{}
void FieldController_OnCollision(Entity* entity, Entity* other, void* runtimeData)
{}
void FieldController_OnRaycastHit(Entity* entity, Entity* other, void* runtimeData)
{}
void FieldController_OnEnable(Entity* entity, void* runtimeData)
{}
void FieldController_OnDisable(Entity* entity, void* runtimeData)
{}

void FieldController_GenMap()
{
	Vec2 movement = Vec2Zero;
	switch (s_Data.CurrentType)
	{
	case FieldElementType::MAP_ELEMENT_1:
	{
		s_Data.CurrentType = FieldElementType::MAP_ELEMENT_2;
		movement = { 0, FIELD_OFFSET_Y };
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(s_Data.Element1Ents[i], ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(s_Data.Element1Ents[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(rigidbody, movement);
			}
			else
			{
				s_Data.Element1Trans[i]->Translation.y += FIELD_OFFSET_Y;
			}
		}
		break;
	}
	case FieldElementType::MAP_ELEMENT_2:
	{
		s_Data.CurrentType = FieldElementType::MAP_ELEMENT_1;
		movement = { 0, FIELD_OFFSET_Y };
		for (size_t i = 0; i < ENTITY_COUNT_PER_FILED_ELEMENT; i++)
		{
			if (Entity_HasComponent(s_Data.Element2Ents[i], ComponentType_Rigidbody2D))
			{
				Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(s_Data.Element2Ents[i], ComponentType_Rigidbody2D);
				Rigidbody2DComponent_MovePosition(rigidbody, movement);
			}
			else
			{
				s_Data.Element2Trans[i]->Translation.y += FIELD_OFFSET_Y;
			}
		}
		break;
	}
	}

	for (size_t i = 0; i < ENTITY_COUNT_BACK_WALL; i++)
	{
		if (Entity_HasComponent(s_Data.BackWallEnts[i], ComponentType_Rigidbody2D))
		{
			Rigidbody2DComponent* rigidbody = (Rigidbody2DComponent*)Entity_GetComponent(s_Data.BackWallEnts[i], ComponentType_Rigidbody2D);
			Rigidbody2DComponent_MovePosition(rigidbody, Vec2DivFloat(movement, 2.0f));
		}
		else
		{
			s_Data.BackWallTrans[i]->Translation.y += FIELD_OFFSET_Y / 2.0f;
		}
	}

	s_Data.FloorTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
	s_Data.RoofTrans->Translation.y += FIELD_OFFSET_Y / 2.0f;
	float offsetX = 0.0f;
	for (size_t i = 0; i < ENEMY_COUNT; i++)
	{
		Scene_SetEntityEnabled(s_Data.EnemyEntity[i], true);
		Vec2 tempPos = s_Data.EnemySpawnPos;
		tempPos.x += offsetX;
		offsetX += 5.0f;
		Rigidbody2DComponent_SetPosition(s_Data.EnemyRigidbody[i], tempPos);
	}
	s_Data.EnemySpawnPos.y += FIELD_OFFSET_Y / 2.0f;
}

void FieldController_OnEnemyDead(const Vec3& pos)
{
	if (!s_Data.ItemEntity->Enabled)
	{
		if (PlayerController_GetHpPercent() < MEDKIT_DROP_HP_PRECENT)
		{
			Scene_SetEntityEnabled(s_Data.ItemEntity, true);
			Rigidbody2DComponent_SetPosition(s_Data.ItemRigidbody, { pos.x,pos.y });
		}
	}
}