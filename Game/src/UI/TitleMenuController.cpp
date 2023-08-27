#include "TitleMenuController.h"
#include "ScreenEffect/Loading.h"
#include "KeyMap/KeyMap.h"

#define CREDITS_TEXT_COUNT 5
#define CREDITS_TEXT_START_POS_Y 1180.0f
#define CREDITS_TEXT_OFFSET_Y 100.0f

#define CREDITS_FONT_NAME L"Arial"
#define CREDITS_FONT_SIZE 50.0f

#define MENU_FONT_NAME L"Arial"
#define MENU_FONT_SIZE 50.0f

#define FORWARD_KEY_CONFIG_NODE_POS_Y 165.0f
#define BACKWARD_KEY_CONFIG_NODE_POS_Y 265.0f
#define LEFT_KEY_CONFIG_NODE_POS_Y 365.0f
#define RIGHT_KEY_CONFIG_NODE_POS_Y 465.0f
#define SHOOT_KEY_CONFIG_NODE_POS_Y 565.0f

#define TIP_FONT_NAME L"Arial"
#define TIP_FONT_SIZE 20.0f
#define TIP_TEXT L"操作ガイド：方向キー、エンターキー、Escキー、ボタン変更（Settings）"
#define TIP_POS_X 1290.0f
#define TIP_POS_Y 1000.0f

#define KEY_CHANGE_TEXT L"変更したいボタンを押してください"
#define KEY_CHANGE_FONT_NAME L"Arial"
#define KEY_CHANGE_FONT_SIZE 50.0f
#define KEY_CHANGE_POS_X 800.0f
#define KEY_CHANGE_POS_Y 800.0f

struct MenuNode
{
	float yPos;

	MenuNode* Next = nullptr;
	MenuNode* Prev = nullptr;

	void(*OnEnter)() = nullptr;
};

struct KeyConfigNode
{
	const WCHAR* Text;

	float yPos;

	KeyType Type;

	KeyConfigNode* Next = nullptr;
	KeyConfigNode* Prev = nullptr;
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
	Entity* SelectionEntity = nullptr;
	RectTransformComponent* SelectionRectTrans = nullptr;
	SpriteRendererComponent* SelectionSprite = nullptr;

	MenuNode StartGameNode;
	MenuNode SettingsNode;
	MenuNode CreditsNode;
	MenuNode ExitNode;

	MenuNode* CurrentMenuNode = nullptr;

	KeyConfigNode WalkForwardNode;
	KeyConfigNode WalkBackwardNode;
	KeyConfigNode WalkLeftNode;
	KeyConfigNode WalkRightNode;
	KeyConfigNode ShootNode;

	KeyConfigNode* CurrentKeyConfigNode = nullptr;

	RectTransformComponent KeyConfiglabelCanvas = {
		{ 790.0f,465.0f },
		{ 150.0f, 150.0f },
	};
	RectTransformComponent KeyConfigNodeCanvas = {
		{ 1120.0f,465.0f },
		{ 150.0f, 150.0f },
	};

	bool IsWaitingForKey = false;

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

	RectTransformComponent TipCanvas = {
		{ TIP_POS_X,TIP_POS_Y },
		{ 150.0f, 150.0f },
	};

	RectTransformComponent KeyChangeTipCanvas = {
		{ KEY_CHANGE_POS_X,KEY_CHANGE_POS_Y },
		{ 150.0f, 150.0f },
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
	s_Data.SelectionRectTrans->Position.x = 770.0f;
	s_Data.SelectionRectTrans->Position.y = s_Data.CurrentKeyConfigNode->yPos;
	s_Data.SelectionRectTrans->Size = { 500.0f, 100.0f };
	s_Data.SelectionSprite->Color = { 0.4f, 0.4f, 0.4f, 0.5f };
	s_Data.BackgroundSprite->Texture = nullptr;
	s_Data.BackgroundSprite->Color = { 0.0f,0.0f,0.0f,0.0f };
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
	s_Data.SelectionRectTrans->Position.x = 790.0f;
	s_Data.SelectionRectTrans->Position.y = s_Data.CurrentMenuNode->yPos;
	s_Data.SelectionRectTrans->Size = { 300.0f, 70.0f };
	s_Data.SelectionSprite->Color = { 0.4f, 0.4f, 0.4f, 0.5f };
	Scene_SetEntityEnabled(*s_Data.SelectionEntity, true);
	s_Data.BackgroundSprite->Texture = s_Data.MenuBGTexture;
	s_Data.BackgroundSprite->Color = { 1.0f,1.0f,1.0f,1.0f };
}

void TitleMenuController_OnKeyPressed(KeyCode key)
{
	if (s_Data.IsWaitingForKey)
	{
		if (KeyMap_SetKey(s_Data.CurrentKeyConfigNode->Type, key))
		{
			s_Data.IsWaitingForKey = false;
		}
	}
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

		s_Data.WalkForwardNode = { L"Forward : ", 830.0f, KeyType::MOVE_FORWARD,&s_Data.WalkBackwardNode,&s_Data.ShootNode };
		s_Data.WalkBackwardNode = { L"Backward : ",730.0f, KeyType::MOVE_BACKWARD,&s_Data.WalkLeftNode,&s_Data.WalkForwardNode };
		s_Data.WalkLeftNode = { L"Left : ",630.0f, KeyType::MOVE_LEFT,&s_Data.WalkRightNode,&s_Data.WalkBackwardNode };
		s_Data.WalkRightNode = { L"Right : ",530.0f, KeyType::MOVE_RIGHT,&s_Data.ShootNode,&s_Data.WalkLeftNode };
		s_Data.ShootNode = { L"Shoot : ",430.0f, KeyType::SHOOT,&s_Data.WalkForwardNode,&s_Data.WalkRightNode };

		s_Data.CurrentMenuNode = &s_Data.StartGameNode;
		s_Data.CurrentKeyConfigNode = &s_Data.WalkForwardNode;
	}

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
	{
		if (s_Data.IsWaitingForKey)
			s_Data.IsWaitingForKey = false;
		else
			ResetToIdle();
	}

	switch (s_Data.State)
	{
	case ::IDLE:
	{
		if (Input_GetKeyDown(KeyCode::Enter))
		{
			s_Data.CurrentMenuNode->OnEnter();
		}

		if (Input_GetKeyDown(KeyCode::UpArrow))
		{
			s_Data.CurrentMenuNode = s_Data.CurrentMenuNode->Prev;
		}
		else if (Input_GetKeyDown(KeyCode::DownArrow))
		{
			s_Data.CurrentMenuNode = s_Data.CurrentMenuNode->Next;
		}

		s_Data.SelectionRectTrans->Position.y = s_Data.CurrentMenuNode->yPos;
		break;
	}
	case ::SETTINGS:
	{
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyConfiglabelCanvas.Position.y = FORWARD_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfiglabelCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(s_Data.WalkForwardNode.Text, MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
			{
				s_Data.KeyConfigNodeCanvas.Position.y = FORWARD_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfigNodeCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KeyMap_GetKeyName(s_Data.WalkForwardNode.Type),
					MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
		}
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyConfiglabelCanvas.Position.y = BACKWARD_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfiglabelCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(s_Data.WalkBackwardNode.Text, MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
			{
				s_Data.KeyConfigNodeCanvas.Position.y = BACKWARD_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfigNodeCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KeyMap_GetKeyName(s_Data.WalkBackwardNode.Type),
					MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
		}
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyConfiglabelCanvas.Position.y = LEFT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfiglabelCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(s_Data.WalkLeftNode.Text, MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
			{
				s_Data.KeyConfigNodeCanvas.Position.y = LEFT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfigNodeCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KeyMap_GetKeyName(s_Data.WalkLeftNode.Type),
					MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
		}
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyConfiglabelCanvas.Position.y = RIGHT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfiglabelCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(s_Data.WalkRightNode.Text, MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
			{
				s_Data.KeyConfigNodeCanvas.Position.y = RIGHT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfigNodeCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KeyMap_GetKeyName(s_Data.WalkRightNode.Type),
					MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
		}
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyConfiglabelCanvas.Position.y = SHOOT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfiglabelCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(s_Data.ShootNode.Text, MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
			{
				s_Data.KeyConfigNodeCanvas.Position.y = SHOOT_KEY_CONFIG_NODE_POS_Y;
				RectTransformComponent_GetPositionAndSize(s_Data.KeyConfigNodeCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KeyMap_GetKeyName(s_Data.ShootNode.Type),
					MENU_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, MENU_FONT_SIZE);
			}
		}

		if (s_Data.IsWaitingForKey)
		{
			Vec2 position, size, ndcPos;
			Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

			{
				s_Data.KeyChangeTipCanvas.Position = { KEY_CHANGE_POS_X,KEY_CHANGE_POS_Y };
				RectTransformComponent_GetPositionAndSize(s_Data.KeyChangeTipCanvas, viewPortSize, &ndcPos, &position, &size);
				Renderer2D_DrawText(KEY_CHANGE_TEXT, KEY_CHANGE_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, KEY_CHANGE_FONT_SIZE);
			}
		}
		else
		{
			if (Input_GetKeyDown(KeyCode::Enter))
			{
				s_Data.IsWaitingForKey = true;
			}

			if (Input_GetKeyDown(KeyCode::UpArrow))
			{
				s_Data.CurrentKeyConfigNode = s_Data.CurrentKeyConfigNode->Prev;
			}
			else if (Input_GetKeyDown(KeyCode::DownArrow))
			{
				s_Data.CurrentKeyConfigNode = s_Data.CurrentKeyConfigNode->Next;
			}
			s_Data.SelectionRectTrans->Position.y = s_Data.CurrentKeyConfigNode->yPos;
		}

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

	if (s_Data.State != CREDITS)
	{
		Vec2 position, size, ndcPos;
		Vec2 viewPortSize = { (float)Window_GetWidth(),(float)Window_GetHeight() };

		{
			RectTransformComponent_GetPositionAndSize(s_Data.TipCanvas, viewPortSize, &ndcPos, &position, &size);
			Renderer2D_DrawText(TIP_TEXT, TIP_FONT_NAME, position, { 1.0f,1.0f,1.0f,1.0f }, TIP_FONT_SIZE);
		}
	}
}

void TitleMenuController_OnDestroy(Entity& entity, void* runtimeData)
{}
void TitleMenuController_OnCollision(Entity& entity, Entity& other, void* runtimeData)
{}
void TitleMenuController_OnRaycastHit(Entity& entity, Entity& other, void* runtimeData)
{}
void TitleMenuController_OnEnable(Entity& entity, void* runtimeData)
{}
void TitleMenuController_OnDisable(Entity& entity, void* runtimeData)
{}