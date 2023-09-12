#include "TitleMenu.h"
#include "TitleMenuCommon.h"
#include "../Screen/Renderer.h"
#include "../Input/Input.h"
#include "../Common.h"
#include "LoadingPage.h"
#include "StaffPage.h"
#include "SettingsPage.h"
#include "StartUpPage.h"
#include "../Battle/Battle.h"

struct TitleMenuNode
{
	TitleMenuNode* Prev;
	TitleMenuNode* Next;
	int NodeLine;
	TitleMenuState EntryState;
};

struct TitleMenuData
{
	TitleCanvasElement Text[14] = {
	{{"|             ,--.!,                            Start                         |\n"},7},
	{{"|             ,--.!,                       -->  Start                         |\n"},7},
	{{"|          __/   -*-                                                          |\n"},8},
	{{"|        ,d08b.  '|`                            Settings                      |\n"},9},
	{{"|        ,d08b.  '|`                       -->  Settings                      |\n"},9},
	{{"|        0088MM                                                               |\n"},10},
	{{"|        `9MMP'                                 Credits                       |\n"},11},
	{{"|        `9MMP'                            -->  Credits                       |\n"},11},
	{{"|        BomberMan                              Exit                          |\n"},13},
	{{"|        BomberMan                         -->  Exit                          |\n"},13},
	{{"|            操作ガイド：方向キー、エンターキー、Escキー                      |\n"},20},
	{{"|            このゲームには二つのプレイヤーが必要です                         |\n"},21},
	{{"|                                FS 2023               Preview Ver 0.1        |\n"},22}, };

	TitleMenuNode StartNode = { NULL,NULL, 0, TitleMenuState::TITLE_START };
	TitleMenuNode SettingsNode = { NULL,NULL, 3, TitleMenuState::TITLE_SETTINGS };
	TitleMenuNode CreditsNode = { NULL,NULL, 6, TitleMenuState::TITLE_CREDITS };
	TitleMenuNode ExitNode = { NULL,NULL, 8, TitleMenuState::TITLE_EXIT };

	TitleMenuNode* CurrentNode;
	TitleMenuState CurrentState = TitleMenuState::TITLE_STARTUP;
	int MaxSceneWidth = 80;
};
TitleMenuData s_Data;

void TitleMenu_Init()
{
	s_Data.StartNode.Prev = &s_Data.ExitNode;
	s_Data.StartNode.Next = &s_Data.SettingsNode;

	s_Data.SettingsNode.Prev = &s_Data.StartNode;
	s_Data.SettingsNode.Next = &s_Data.CreditsNode;

	s_Data.CreditsNode.Prev = &s_Data.SettingsNode;
	s_Data.CreditsNode.Next = &s_Data.ExitNode;

	s_Data.ExitNode.Prev = &s_Data.CreditsNode;
	s_Data.ExitNode.Next = &s_Data.StartNode;

	s_Data.CurrentNode = &s_Data.StartNode;

	StartUpPage_Init();
	StartUpPage_SetDeparture(TitleMenuState::TITLE_IDLE);
}

void DrawTitleIdelMenu()
{
	char result[SCREEN_BUFFER_SIZE] = "";

	if (Input_IsKeyPressed(Key::UPARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Prev;
	else if (Input_IsKeyPressed(Key::DOWNARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Next;

	int index = 0;
	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.Text[index].StartLine)
		{
			if (index == s_Data.CurrentNode->NodeLine)
				strcat(result, s_Data.Text[index + 1].Text);
			else
				strcat(result, s_Data.Text[index].Text);

			if (index == s_Data.StartNode.NodeLine || index == s_Data.SettingsNode.NodeLine ||
				index == s_Data.CreditsNode.NodeLine || index == s_Data.ExitNode.NodeLine)
			{
				index++;
			}

			index++;
		}
		else
			strcat(result, CANVAS_LINE);
	}

	if (Input_IsKeyPressed(Key::ENTER))
	{
		s_Data.CurrentState = s_Data.CurrentNode->EntryState;
		switch (s_Data.CurrentState)
		{
		case TitleMenuState::TITLE_CREDITS:
		{
			StaffPage_Init();
			StaffPage_SetDeparture(TitleMenuState::TITLE_IDLE);
			LoadingPage_Init();
			LoadingPage_SetDeparture(GameState::GAME_STATE_MENU_SCENE);
			Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);	
			break;
		}
		case TitleMenuState::TITLE_SETTINGS:
		{
			SettingsPage_Init();
			SettingsPage_SetDeparture(TitleMenuState::TITLE_IDLE);
			LoadingPage_Init();
			LoadingPage_SetDeparture(GameState::GAME_STATE_MENU_SCENE);
			Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);
			break;
		}
		}
	}

	Renderer_UpdateData(result);
}

void TitleMenu_Draw()
{
	switch (s_Data.CurrentState)
	{
	case TitleMenuState::TITLE_STARTUP:
	{
		StartUpPage_Draw();
		break;
	}
	case TitleMenuState::TITLE_IDLE:
	{
		DrawTitleIdelMenu();
		break;
	}
	case TitleMenuState::TITLE_START:
	{
		LoadingPage_Init();
		LoadingPage_SetDeparture(GameState::GAME_STATE_PLAY_SCENE);
		Battle_Init();
		Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);
		break;
	}
	case TitleMenuState::TITLE_SETTINGS:
	{
		SettingsPage_Draw();
		break;
	}
	case TitleMenuState::TITLE_CREDITS:
	{
		StaffPage_Draw();
		break;
	}
	case TitleMenuState::TITLE_EXIT:
	{
		Game_Quit();
		break;
	}
	}
}

void TitleMenu_SetMenuState(TitleMenuState state)
{
	s_Data.CurrentState = state;
}