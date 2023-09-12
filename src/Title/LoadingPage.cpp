#include "LoadingPage.h"
#include "TitleMenuCommon.h"
#include "../Screen/Renderer.h"
#include "../Common.h"

struct LoadingData
{
	int Frame = 0;
	int MaxFrame = 6;
	TitleCanvasElement Text[7] = {
	    {{"|                                                                =------      |\n"},22},
	    {{"|                                                                ==-----      |\n"},22},
	    {{"|                                                                ===----      |\n"},22},
	    {{"|                                                                ====---      |\n"},22},
	    {{"|                                                                =====--      |\n"},22},
	    {{"|                                                                ======-      |\n"},22},
	    {{"|                                                                =======      |\n"},22}, };

	GameState Departure;
};
LoadingData s_Data;

void LoadingPage_Init()
{
	s_Data.Frame = 0;
}

void LoadingPage_Draw()
{
	if (s_Data.Frame > s_Data.MaxFrame)
	{
		Game_SetGameState(s_Data.Departure);
		return;
	}

	char result[SCREEN_BUFFER_SIZE] = "";

	for (int i = 0; i < CANVAS_MAX_LINE; i++)
	{
		if (i == 0 || i == CANVAS_MAX_LINE - 1)
			strcat(result, CANVAS_BORDER_LINE);
		else if (i == s_Data.Text[s_Data.Frame].StartLine)
			strcat(result, s_Data.Text[s_Data.Frame].Text);
		else
			strcat(result, CANVAS_LINE);
	}

	Renderer_UpdateData(result);
	s_Data.Frame++;
}

void LoadingPage_SetDeparture(GameState state)
{
	s_Data.Departure = state;
}