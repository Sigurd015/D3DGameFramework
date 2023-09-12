#include "Game.h"
#include "Input/Input.h"
#include "Title/StartUpPage.h"
#include "Title/TitleMenu.h"
#include "Title/LoadingPage.h"
#include "Common.h"
#include "Battle/Battle.h"

#include <stdio.h>

struct GameData
{
	bool IsGameRunning = true;
	GameState CurrentGameState = GameState::GAME_STATE_MENU_SCENE;
	int FrameTime = FRAME_TIME;
	int FrameTimeAccumulator = 0;
	int FrameTimeApplyFrameCount = 0;
};
GameData s_Data;

void Game_Init()
{
	TitleMenu_Init();
}

void Game_Loop()
{
	switch (s_Data.CurrentGameState)
	{
	case GameState::GAME_STATE_LOADING_SCENE:
		LoadingPage_Draw();
		break;
	case GameState::GAME_STATE_MENU_SCENE:
		TitleMenu_Draw();
		break;
	case GameState::GAME_STATE_PLAY_SCENE:
		Battle_Draw();
		break;
	}
}

bool Game_IsRunning()
{
	return s_Data.IsGameRunning;
}

void Game_SetGameState(GameState state)
{
	s_Data.CurrentGameState = state;
}

void Game_SetFramTime(int milliseconds, int applyFrameCount)
{
	s_Data.FrameTimeAccumulator = milliseconds;
	s_Data.FrameTimeApplyFrameCount = applyFrameCount;
}

void Game_SetFramTime(int frameTime)
{
	s_Data.FrameTime = frameTime;
}

void Game_RsetFramTime()
{
	s_Data.FrameTime = FRAME_TIME;
}

int Game_GetFramTime()
{
	if (s_Data.FrameTimeApplyFrameCount > 0)
	{
		s_Data.FrameTimeApplyFrameCount--;
		return s_Data.FrameTime + s_Data.FrameTimeAccumulator;
	}
	return	s_Data.FrameTime;
}

void Game_Quit()
{
	s_Data.IsGameRunning = false;
}
