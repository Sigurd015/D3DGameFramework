#include "SettingsPage.h"
#include "TitleMenuCommon.h"
#include "../Common.h"
#include "../Input/Input.h"
#include "../Screen/Renderer.h"
#include "../Game.h"
#include "LoadingPage.h"

#include <conio.h>
#include <cstdio>
#include <ctype.h>

enum SettingsState
{
	SETTINGPAGE_IDLE,
	SETTINGPAGE_KEY_CHANGE,
};

struct SettingsMenuNode
{
	SettingsMenuNode* Prev;
	SettingsMenuNode* Next;
	SettingsMenuNode* Left;
	SettingsMenuNode* Right;
	int NodeLine;
	SettingsState EntryState;
	Key key;
};

struct SettingCanvasElement
{
	char* Text;
	int StartLine;
	Key key;
};

struct SettingsData
{
	SettingCanvasElement Text[20] = {
		{{"|         P1  Move   Up  -  "},8,Key::P1_UP},
		{{"|    -->  P1  Move   Up  -  "},8,Key::P1_UP},
		{{"|                                                P2  Move   Up  -  "},9,Key::P2_UP},
		{{"|                                           -->  P2  Move   Up  -  "},9,Key::P2_UP},
		{{"|         P1 Move  Down  -  "},10,Key::P1_DOWN},
		{{"|    -->  P1 Move  Down  -  "},10,Key::P1_DOWN},
		{{"|                                                P2  Move Down  -  "},11,Key::P2_DOWN},
		{{"|                                           -->  P2  Move Down  -  "},11,Key::P2_DOWN},
		{{"|         P1 Move Left   -  "},12,Key::P1_LEFT},
		{{"|    -->  P1 Move Left   -  "},12,Key::P1_LEFT},
		{{"|                                                P2  Move Left  -  "},13,Key::P2_LEFT},
		{{"|                                           -->  P2  Move Left  -  "},13,Key::P2_LEFT},
		{{"|         P1 Move Right  -  "},14,Key::P1_RIGHT},
		{{"|    -->  P1 Move Right  -  "},14,Key::P1_RIGHT},
		{{"|                                                P2  Move Right -  "},15,Key::P2_RIGHT},
		{{"|                                           -->  P2  Move Right -  "},15,Key::P2_RIGHT},
		{{"|            P1 Bomb     -  "},16,Key::P1_BOMB},
		{{"|    -->     P1 Bomb     -  "},16,Key::P1_BOMB},
		{{"|                                                   P2  Bomb    -  "}, 17,Key::P2_BOMB},
		{{"|                                           -->     P2  Bomb    -  "}, 17,Key::P2_BOMB}, };

	TitleCanvasElement KeyInputText[4] = {
		{{"|                          変更したいキー                                     |\n"}, 11},
		{{"|                        押してください(A-Z)                                  |\n"}, 12},
		{{"|                          違うキーを押すと                                   |\n"}, 14},
		{{"|                       この画面は表示し続けます                              |\n"}, 15}, };

	TitleMenuState Departure;
	int MaxSceneWidth = 80;

	SettingsMenuNode P1UpKeyNode = { NULL,NULL,NULL,NULL, 0 ,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P1_UP };
	SettingsMenuNode P1DownKeyNode = { NULL,NULL,NULL,NULL, 4,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P1_DOWN };
	SettingsMenuNode P1LeftKeyNode = { NULL,NULL,NULL,NULL, 8,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P1_LEFT };
	SettingsMenuNode P1RightKeyNode = { NULL,NULL,NULL,NULL, 12,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P1_RIGHT };
	SettingsMenuNode P1BombKeyNode = { NULL,NULL,NULL,NULL, 16,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P1_BOMB };

	SettingsMenuNode P2UpKeyNode = { NULL,NULL,NULL,NULL, 2 ,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P2_UP };
	SettingsMenuNode P2DownKeyNode = { NULL,NULL,NULL,NULL, 6,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P2_DOWN };
	SettingsMenuNode P2LeftKeyNode = { NULL,NULL,NULL,NULL, 10,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P2_LEFT };
	SettingsMenuNode P2RightKeyNode = { NULL,NULL,NULL,NULL, 14,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P2_RIGHT };
	SettingsMenuNode P2BombKeyNode = { NULL,NULL,NULL,NULL, 18,SettingsState::SETTINGPAGE_KEY_CHANGE,Key::P2_BOMB };

	SettingsMenuNode* CurrentNode;
	SettingsState CurrentState = SettingsState::SETTINGPAGE_IDLE;
};
SettingsData s_Data;

void SettingsPage_Init()
{
	s_Data.P1UpKeyNode.Prev = &s_Data.P1BombKeyNode;
	s_Data.P1UpKeyNode.Next = &s_Data.P1DownKeyNode;
	s_Data.P1UpKeyNode.Left = &s_Data.P2UpKeyNode;
	s_Data.P1UpKeyNode.Right = &s_Data.P2UpKeyNode;

	s_Data.P1DownKeyNode.Prev = &s_Data.P1UpKeyNode;
	s_Data.P1DownKeyNode.Next = &s_Data.P1LeftKeyNode;
	s_Data.P1DownKeyNode.Left = &s_Data.P2DownKeyNode;
	s_Data.P1DownKeyNode.Right = &s_Data.P2DownKeyNode;

	s_Data.P1LeftKeyNode.Prev = &s_Data.P1DownKeyNode;
	s_Data.P1LeftKeyNode.Next = &s_Data.P1RightKeyNode;
	s_Data.P1LeftKeyNode.Left = &s_Data.P2LeftKeyNode;
	s_Data.P1LeftKeyNode.Right = &s_Data.P2LeftKeyNode;

	s_Data.P1RightKeyNode.Prev = &s_Data.P1LeftKeyNode;
	s_Data.P1RightKeyNode.Next = &s_Data.P1BombKeyNode;
	s_Data.P1RightKeyNode.Left = &s_Data.P2RightKeyNode;
	s_Data.P1RightKeyNode.Right = &s_Data.P2RightKeyNode;

	s_Data.P1BombKeyNode.Prev = &s_Data.P1RightKeyNode;
	s_Data.P1BombKeyNode.Next = &s_Data.P1UpKeyNode;
	s_Data.P1BombKeyNode.Left = &s_Data.P2BombKeyNode;
	s_Data.P1BombKeyNode.Right = &s_Data.P2BombKeyNode;

	s_Data.P2UpKeyNode.Prev = &s_Data.P2BombKeyNode;
	s_Data.P2UpKeyNode.Next = &s_Data.P2DownKeyNode;
	s_Data.P2UpKeyNode.Left = &s_Data.P1UpKeyNode;
	s_Data.P2UpKeyNode.Right = &s_Data.P1UpKeyNode;

	s_Data.P2DownKeyNode.Prev = &s_Data.P2UpKeyNode;
	s_Data.P2DownKeyNode.Next = &s_Data.P2LeftKeyNode;
	s_Data.P2DownKeyNode.Left = &s_Data.P1DownKeyNode;
	s_Data.P2DownKeyNode.Right = &s_Data.P1DownKeyNode;

	s_Data.P2LeftKeyNode.Prev = &s_Data.P2DownKeyNode;
	s_Data.P2LeftKeyNode.Next = &s_Data.P2RightKeyNode;
	s_Data.P2LeftKeyNode.Left = &s_Data.P1LeftKeyNode;
	s_Data.P2LeftKeyNode.Right = &s_Data.P1LeftKeyNode;

	s_Data.P2RightKeyNode.Prev = &s_Data.P2LeftKeyNode;
	s_Data.P2RightKeyNode.Next = &s_Data.P2BombKeyNode;
	s_Data.P2RightKeyNode.Left = &s_Data.P1RightKeyNode;
	s_Data.P2RightKeyNode.Right = &s_Data.P1RightKeyNode;

	s_Data.P2BombKeyNode.Prev = &s_Data.P2RightKeyNode;
	s_Data.P2BombKeyNode.Next = &s_Data.P2UpKeyNode;
	s_Data.P2BombKeyNode.Left = &s_Data.P1BombKeyNode;
	s_Data.P2BombKeyNode.Right = &s_Data.P1BombKeyNode;

	s_Data.CurrentNode = &s_Data.P1UpKeyNode;
	s_Data.CurrentState = SettingsState::SETTINGPAGE_IDLE;
}

void DrawKeyNode(char* result, char key, int index)
{
	char tempArray[2] = { toupper(key), '\0' };
	strcat(result, tempArray);
	int len = s_Data.MaxSceneWidth - strlen(tempArray) - strlen(s_Data.Text[index].Text);
	for (size_t i = 1; i < len - 1; i++)
	{
		strcat(result, " ");
	}
	strcat(result, "|\n");
}

void DrawSettingPageIdelMenu()
{
	char result[SCREEN_BUFFER_SIZE] = "";

	if (Input_IsKeyPressed(Key::UPARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Prev;
	else if (Input_IsKeyPressed(Key::DOWNARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Next;
	else if (Input_IsKeyPressed(Key::LEFTARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Left;
	else if (Input_IsKeyPressed(Key::RIGHTARROW))
		s_Data.CurrentNode = s_Data.CurrentNode->Right;

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

			DrawKeyNode(result, Input_GetKey(s_Data.Text[index].key), index);

			if (index == s_Data.P1UpKeyNode.NodeLine || index == s_Data.P1DownKeyNode.NodeLine ||
				index == s_Data.P1LeftKeyNode.NodeLine || index == s_Data.P1RightKeyNode.NodeLine ||
				index == s_Data.P1BombKeyNode.NodeLine || index == s_Data.P2UpKeyNode.NodeLine ||
				index == s_Data.P2DownKeyNode.NodeLine || index == s_Data.P2LeftKeyNode.NodeLine ||
				index == s_Data.P2RightKeyNode.NodeLine || index == s_Data.P2BombKeyNode.NodeLine)
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
	}

	if (Input_IsKeyPressed(Key::BACK))
	{
		s_Data.CurrentState = SettingsState::SETTINGPAGE_IDLE;
		LoadingPage_Init();
		LoadingPage_SetDeparture(GameState::GAME_STATE_MENU_SCENE);
		Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);
		TitleMenu_SetMenuState(s_Data.Departure);
	}

	Renderer_UpdateData(result);
}

void DrawKeyInput()
{
	char result[SCREEN_BUFFER_SIZE] = "";

	int index = 0;
	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.KeyInputText[index].StartLine)
		{
			strcat(result, s_Data.KeyInputText[index].Text);
			index++;
		}
		else
			strcat(result, CANVAS_LINE);
	}
	strcat(result, "-->");
	Renderer_UpdateData(result);
	Renderer_Draw();  // このフレームをすぐにレンダリングする
}

void SetKey(Key key, int keyCode)
{
	if (Input_SetKey(key, (KeyCode)keyCode))
	{
		s_Data.CurrentState = SettingsState::SETTINGPAGE_IDLE;
	}
	else
	{
		int temp;
		rewind(stdin);
		temp = _getch();
		SetKey(key, temp);
	}
}

void SettingsPage_Draw()
{
	switch (s_Data.CurrentState)
	{
	case SettingsState::SETTINGPAGE_IDLE:
	{
		DrawSettingPageIdelMenu();
		break;
	}
	case SettingsState::SETTINGPAGE_KEY_CHANGE:
	{
		DrawKeyInput();
		int temp;
		rewind(stdin);
		temp = _getch();
		SetKey(s_Data.CurrentNode->key, temp);
		break;
	}
	}
}

void SettingsPage_SetDeparture(TitleMenuState mode)
{
	s_Data.Departure = mode;
}