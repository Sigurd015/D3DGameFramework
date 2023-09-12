#include "StartUpPage.h"
#include "TitleMenuCommon.h"
#include "../Screen/Renderer.h"
#include "../Game.h"
#include "../Input/Input.h"
#include "LoadingPage.h"
#include "../Common.h"

#include <stdio.h>

struct StartUpData
{
	int Frame = 0;
	int MaxFrame = 7;
	TitleCanvasElement Text[8] = {
		{{"|                                  by                                         |\n"},10},
		{{"|                           Freestyle Studio                                  |\n"},11},
		{{"|                             Freest Studi                                    |\n"},11},
		{{"|                               Free Stud                                     |\n"},11},
		{{"|                                Fre Stu                                      |\n"},11},
		{{"|                                 Fr St                                       |\n"},11},
		{{"|                                  F S                                        |\n"},11},
		{{"|                                  FS                                         |\n"},11}, };

	TitleMenuState Departure;
};

StartUpData s_Data;

void StartUpPage_Init()
{
	s_Data.Frame = 0;
	Game_SetFramTime(STARTUP_FRAME_TIME_ACCUMULATOR, s_Data.MaxFrame);
}

void StartUpPage_Draw()
{
	if (s_Data.Frame > s_Data.MaxFrame || Input_IsKeyPressed(Key::BACK) || Input_IsKeyPressed(Key::ENTER))
	{
		LoadingPage_Init();
		LoadingPage_SetDeparture(GAME_STATE_MENU_SCENE);
		Game_SetGameState(GAME_STATE_LOADING_SCENE);
		TitleMenu_SetMenuState(s_Data.Departure);
		return;
	}

	char result[SCREEN_BUFFER_SIZE] = "";

	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.Text[0].StartLine && s_Data.Frame != 0)
			strcat(result, s_Data.Text[0].Text);
		else if (i == s_Data.Text[s_Data.Frame].StartLine && s_Data.Frame != 0)
			strcat(result, s_Data.Text[s_Data.Frame].Text);
		else
			strcat(result, CANVAS_LINE);
	}

	Renderer_UpdateData(result);
	s_Data.Frame++;
}

void StartUpPage_SetDeparture(TitleMenuState state)
{
	s_Data.Departure = state;
}