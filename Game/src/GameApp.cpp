#include "Core.h"
#include "EntryPoint.h"
#include "ScriptGlue.hpp"
#include "UI/StartUp.h"
#include "UI/Loading.h"
#include "GameMode.h"

#include <stdio.h>

struct GameData
{
	Scene PlayScene;
	Scene TitleScene;

	GameMode Mode = STARTUP_TITLE;
};
static GameData s_Data;

void Game_Ininialize(Application* appInst)
{
	Scene_Create(s_Data.PlayScene);
	ScriptGlue_CreatePlayScene(s_Data.PlayScene);
	Scene_Ininialize(s_Data.PlayScene);
	ScriptGlue_CreateTitleScene(s_Data.TitleScene);
	Scene_Ininialize(s_Data.TitleScene);

	StartUp_Initialize();
	Loading_Initialize();
	s_Data.Mode = STARTUP_TITLE;
}

void Game_Update(float timeStep)
{
	RendererAPI_Clear();

	switch (s_Data.Mode)
	{
	case LOADING_SCENE:
	{
		Loading_Draw(timeStep);
		break;
	}
	case STARTUP_TITLE:
	{
		StartUp_Draw(timeStep);
		break;
	}
	case TITLE_MENU:
	{
		Scene_OnViewportResize(s_Data.TitleScene, Window_GetWidth(), Window_GetHeight());
#ifndef BV_DIST
		Scene_OnUpdate(s_Data.TitleScene, timeStep, true);
#else
		Scene_OnUpdate(s_Data.TitleScene, timeStep);
#endif
		break;
	}
	case PLAY_SCENE:
	{
		Scene_OnViewportResize(s_Data.PlayScene, Window_GetWidth(), Window_GetHeight());
#ifndef BV_DIST
		Scene_OnUpdate(s_Data.PlayScene, timeStep, true);
#else
		Scene_OnUpdate(s_Data.PlayScene, timeStep);
#endif
		break;
	}
	}
	}

void Game_Shutdown(Application* appInst)
{
	Scene_Destroy(s_Data.PlayScene);
	Scene_Destroy(s_Data.TitleScene);
}

void Game_SetMode(GameMode mode)
{
	s_Data.Mode = mode;
}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Bloodveil";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = false;
	appInst->Spec.Maximizable = false;
	appInst->Spec.Minimizable = true;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	if (appInst->Spec.CommandLineArgs.Count > 1)
	{
		if (!strcmp(appInst->Spec.CommandLineArgs.Args[1], "fullscreen"))
		{
			appInst->Spec.FullScreen = true;
		}
	}
	else
	{
		appInst->Spec.FullScreen = false;
	}
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
}