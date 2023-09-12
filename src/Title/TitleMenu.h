#pragma once

enum TitleMenuState
{
	TITLE_STARTUP, TITLE_IDLE, TITLE_START, TITLE_SETTINGS, TITLE_CREDITS, TITLE_EXIT
};

void TitleMenu_Init();
void TitleMenu_Draw();
void TitleMenu_SetMenuState(TitleMenuState state);