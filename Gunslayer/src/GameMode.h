#pragma once

enum GameMode
{
	STARTUP_TITLE = 0, TITLE_MENU, PLAY_SCENE, LOADING_SCENE
};

void Game_SetMode(GameMode mode);