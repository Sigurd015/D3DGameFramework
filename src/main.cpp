//--------------------------
//--------------------------
//      Cmd Game
//  Created by Yu Jiang 
//  Version 0.1  2023.06.25
//--------------------------
//--------------------------
#include "Input/Input.h"
#include "Game.h"
#include "Screen/Renderer.h"
#include "Common.h"

#include <Windows.h>

int main()
{
	Game_Init();

	while (Game_IsRunning())
	{
		Input_Process();

		Game_Loop();

		Renderer_Draw();

		Sleep(Game_GetFramTime());
	}
	return 0;
}