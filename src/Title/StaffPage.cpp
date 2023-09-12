#include "StaffPage.h"
#include "TitleMenuCommon.h"
#include "../Screen/Renderer.h"
#include "../Common.h"
#include "LoadingPage.h"
#include "../Game.h"
#include "../Input/Input.h"

#define START_LINE 22

struct StaffData
{
	int Frame = 0;
	int MaxFrame = 31;
	int CreditsLineCount = 0;
	TitleCanvasElement Text[9] = {
		{{"|                      Freestyle Studio                                       |\n"},START_LINE},
		{{"|                                                                             |\n"},START_LINE},
		{{"|                     Program, Design, etc                                    |\n"},START_LINE},
		{{"|                                                                             |\n"},START_LINE},
		{{"|                             YU JIANG                                        |\n"},START_LINE},
		{{"|                                                                             |\n"},START_LINE},
		{{"|                        Special Thanks                                       |\n"},START_LINE},
		{{"|                                                                             |\n"},START_LINE},
		{{"|                            HAL Tokyo                                        |\n"},START_LINE}, };

	TitleMenuState Departure;
};
StaffData s_Data;

void StaffPage_Init()
{
	s_Data.Frame = 0;
	s_Data.CreditsLineCount = 0;
	s_Data.Text[0].StartLine = START_LINE;
	s_Data.Text[1].StartLine = START_LINE;
	s_Data.Text[2].StartLine = START_LINE;
	s_Data.Text[3].StartLine = START_LINE;
	s_Data.Text[4].StartLine = START_LINE;
	s_Data.Text[5].StartLine = START_LINE;
	s_Data.Text[6].StartLine = START_LINE;
	s_Data.Text[7].StartLine = START_LINE;
	s_Data.Text[8].StartLine = START_LINE;
}

int GetIndex(int index)
{
	if (s_Data.Text[index].StartLine == 0)
		return GetIndex(index + 1);
	else
		return index;
}

void StaffPage_Draw()
{
	if (s_Data.Frame > s_Data.MaxFrame || Input_IsKeyPressed(Key::BACK) || Input_IsKeyPressed(Key::ENTER))
	{
		LoadingPage_Init();
		LoadingPage_SetDeparture(GameState::GAME_STATE_MENU_SCENE);
		Game_SetGameState(GameState::GAME_STATE_LOADING_SCENE);
		TitleMenu_SetMenuState(s_Data.Departure);
		return;
	}

	char result[SCREEN_BUFFER_SIZE] = "";
	int index = 0;
	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		index = GetIndex(index);

		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.Text[index].StartLine)
		{
			if (s_Data.CreditsLineCount == 0)
			{
				strcat(result, CANVAS_LINE);
				s_Data.CreditsLineCount++;
			}
			else
			{
				strcat(result, s_Data.Text[index].Text);
				s_Data.Text[index].StartLine--;
			}
			index++;
		}
		else
			strcat(result, CANVAS_LINE);
	}

	Renderer_UpdateData(result);
	s_Data.Frame++;
}

void StaffPage_SetDeparture(TitleMenuState state)
{
	s_Data.Departure = state;
}
