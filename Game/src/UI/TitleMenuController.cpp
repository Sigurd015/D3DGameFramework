#include "TitleMenuController.h"
#include "../ScreenEffect/Loading.h"

#define CREDITS_TEXT_COUNT 5
#define CREDITS_TEXT_START_POS_Y 1180.0f
#define CREDITS_TEXT_OFFSET_Y 100.0f

#define CREDITS_FONT_NAME L"Arial"
#define CREDITS_FONT_SIZE 50.0f

struct MenuNode
{
	float yPos;

	MenuNode* Next = nullptr;
	MenuNode* Prev = nullptr;

	void(*OnEnter)() = nullptr;
};

enum MenuState
{
	IDLE,
	SETTINGS,
	CREDITS,
};

struct TitleMenuControllerData
{
	SpriteRendererComponent* BackgroundSprite = nullptr;
	Texture2D* MenuBGTexture = nullptr;
	Texture2D* SettingsBGTexture = nullptr;
	Entity* SelectionEntity = nullptr;
	RectTransformComponent* SelectionRectTrans = nullptr;
	SpriteRendererComponent* SelectionSprite = nullptr;

	MenuNode* CurrentNode = nullptr;

	MenuNode StartGameNode;
	MenuNode SettingsNode;
	MenuNode CreditsNode;
	MenuNode ExitNode;

	MenuState State = IDLE;

	RectTransformComponent CreditsCanvas = {
		{ 655.0f,465.0f },
		{ 150.0f, 150.0f },
	};
	float CreditsSpeed = 100.0f;
	float CreditsYPos[CREDITS_TEXT_COUNT];
	WCHAR* CreditsText[CREDITS_TEXT_COUNT] = {
		{L"Freestyle Studio"},
		{L"Program, Design, etc"},
		{L"YU JIANG"},
		{L"Special Thanks"},
		{L"HAL Tokyo"},
	};
};
static TitleMenuControllerData s_Data;

void ExitGame()
{
	Application_Close();
}

void StartGame()
{
	Loading_Reset();
	Loading_SetDepature(PLAY_SCENE);
	Game_SetMode(LOADING_SCENE);
}

void Settings()
{
	s_Data.BackgroundSprite->Texture = s_Data.SettingsBGTexture;
	s_Data.BackgroundSprite->Color = { 1.0f,1.0f,1.0f,1.0f };
	s_Data.State = SETTINGS;
}

void Credits()
{
	s_Data.BackgroundSprite->Texture = nullptr;
	s_Data.BackgroundSprite->Color = { 0.0f,0.0f,0.0f,1.0f };
	Scene_SetEntityEnabled(*s_Data.SelectionEntity, false);
	s_Data.State = CREDITS;

	for (size_t i = 0; i < CREDITS_TEXT_COUNT; i++)
	{
		s_Data.CreditsYPos[i] = CREDITS_TEXT_START_POS_Y + i * CREDITS_TEXT_OFFSET_Y;
	}
}

void ResetToIdle()
{
	s_Data.State = IDLE;
	Scene_SetEntityEnabled(*s_Data.SelectionEntity, true);
	s_Data.BackgroundSprite->Texture = s_Data.MenuBGTexture;
	s_Data.BackgroundSprite->Color = { 1.0f,1.0f,1.0f,1.0f };
}

void TitleMenuController_OnCreate(Entity& entity, void* runtimeData)
{
	s_Data = {};

	//Init menu nodes
	{
		s_Data.StartGameNode = { 330.0f,&s_Data.SettingsNode,&s_Data.ExitNode,StartGame };
		s_Data.SettingsNode = { 230.0f,&s_Data.CreditsNode,&s_Data.StartGameNode,Settings };
		s_Data.CreditsNode = { 130.0f,&s_Data.ExitNode,&s_Data.SettingsNode,Credits };
		s_Data.ExitNode = { 30.0f,&s_Data.StartGameNode,&s_Data.CreditsNode,ExitGame };

		s_Data.CurrentNode = &s_Data.StartGameNode;
	}

	s_Data.SettingsBGTexture = (Texture2D*)malloc(sizeof(Texture2D));
	Texture2D_Create(*s_Data.SettingsBGTexture, "assets/textures/settings.png");

	//Background
	{
		s_Data.BackgroundSprite = (SpriteRendererComponent*)Entity_GetComponent(entity, ComponentType_SpriteRenderer);

		CORE_ASSERT(s_Data.BackgroundSprite, "Entity does not have RectTransformComponent!");

		s_Data.MenuBGTexture = s_Data.BackgroundSprite->Texture;
	}

	//Selection
	{
		s_Data.SelectionEntity = Scene_GetEntityByName(*entity.Scene, "Selection");

		CORE_ASSERT(s_Data.SelectionEntity, "Cannot find selection entity!");

		s_Data.SelectionRectTrans = (RectTransformComponent*)Entity_GetComponent(*s_Data.SelectionEntity, ComponentType_RectTransform);

		CORE_ASSERT(s_Data.SelectionRectTrans, "Entity does not have RectTransformComponent!");

		s_Data.SelectionSprite = (SpriteRendererComponent*)Entity_GetComponent(*s_Data.SelectionEntity, ComponentType_SpriteRenderer);

		CORE_ASSERT(s_Data.SelectionSprite, "Entity does not have RectTransformComponent!");
	}
}

void TitleMenuController_OnUpdate(Entity& entity, float timeStep, void* runtimeData)
{
	if (Input_GetKeyDown(KeyCode::Esc))
		ResetToIdle();

	switch (s_Data.State)
	{
	case ::IDLE:
	{
		if (Input_GetKeyDown(KeyCode::Enter))
		{
			s_Data.CurrentNode->OnEnter();
		}

		if (Input_GetKeyDown(KeyCode::UpArrow))
		{
			s_Data.CurrentNode = s_Data.CurrentNode->Prev;
		}
		else if (Input_GetKeyDown(KeyCode::DownArrow))
		{
			s_Data.CurrentNode = s_Data.CurrentNode->Next;
		}

		s_Data.SelectionRectTrans->Position.y = s_Data.CurrentNode->yPos;
		break;
	}
	case ::SETTINGS:
	{
		if (Input_GetKeyDown(KeyCode::Enter))
		{
			s_Data.CurrentNode->OnEnter();
		}

		if (Input_GetKeyDown(KeyCode::UpArrow))
		{
			s_Data.CurrentNode = s_Data.CurrentNode->Prev;
		}
		else if (Input_GetKeyDown(KeyCode::DownArrow))
		{
			s_Data.CurrentNode = s_Data.CurrentNode->Next;
		}

		s_Data.SelectionRectTrans->Position.y = s_Data.CurrentNode->yPos;
		break;
	}
	case ::CREDITS:
	{
		Vec2 position, size, ndcPos;
		Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

		for (size_t i = 0; i < CREDITS_TEXT_COUNT; i++)
		{
			s_Data.CreditsCanvas.Position.y = s_Data.CreditsYPos[i];
			RectTransformComponent_GetPositionAndSize(s_Data.CreditsCanvas, viewPortSize, &ndcPos, &position, &size);
			Renderer2D_DrawText(s_Data.CreditsText[i], CREDITS_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, CREDITS_FONT_SIZE);

			s_Data.CreditsYPos[i] -= s_Data.CreditsSpeed * timeStep;
		}

		if (s_Data.CreditsYPos[CREDITS_TEXT_COUNT - 1] < -100.0f)
			ResetToIdle();

		break;
	}
	}
}

void TitleMenuController_OnDestroy(Entity& entity, void* runtimeData)
{}

void TitleMenuController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}

void TitleMenuController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{}