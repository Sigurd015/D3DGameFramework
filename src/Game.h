#pragma once

enum GameState
{
	GAME_STATE_LOADING_SCENE,
	GAME_STATE_MENU_SCENE,
	GAME_STATE_PLAY_SCENE,
};

void Game_Init();
void Game_Loop();
bool Game_IsRunning();
void Game_SetGameState(GameState state);
void Game_SetFramTime(int milliseconds, int applyFrameCount);
void Game_SetFramTime(int frameTime);
void Game_RsetFramTime();
int Game_GetFramTime();
void Game_Quit();