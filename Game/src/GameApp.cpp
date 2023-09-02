#include "Core.h"
#include "EntryPoint.h"
#include "ScriptGlue.hpp"
#include "ScreenEffect/StartUp.h"
#include "ScreenEffect/Loading.h"
#include "GameMode.h"
#include "KeyMap/KeyMap.h"

#include <ctype.h>

struct GameData
{
	Scene PlayScene;
	Scene TitleScene;

	bool PlaySceneDestroyRequested = false;

	GameMode Mode = LOADING_SCENE;
};
static GameData s_Data;

void Game_Ininialize(Application* appInst)
{
	s_Data = {};

	Scene_Create(s_Data.TitleScene);
	ScriptGlue_CreateTitleScene(s_Data.TitleScene);
	Scene_Ininialize(s_Data.TitleScene);

	KeyMap_Init();

	StartUp_Initialize();
	Loading_Initialize();
	Loading_SetDepature(STARTUP_TITLE);
	//TODO: Set back to LOADING_SCENE when testing is done
	Game_SetMode(TITLE_MENU);
}

void Game_Update(float timeStep)
{
	if (s_Data.PlaySceneDestroyRequested)
	{
		Scene_Destroy(s_Data.PlayScene);
		s_Data.PlaySceneDestroyRequested = false;

		Scene_Create(s_Data.PlayScene);
		ScriptGlue_CreatePlayScene(s_Data.PlayScene);
		Scene_Ininialize(s_Data.PlayScene);
	}

	switch (s_Data.Mode)
	{
	case LOADING_SCENE:
	{
		RendererAPI_SetClearColor({ 0,0,0,0 });
		RendererAPI_Clear();

		Renderer2D_BeginScene(DirectX::XMMatrixIdentity());

		Loading_Draw(timeStep);
		Renderer2D_EndScene();
		break;
	}
	case STARTUP_TITLE:
	{
		RendererAPI_SetClearColor({ 0,0,0,0 });
		RendererAPI_Clear();

		Renderer2D_BeginScene(DirectX::XMMatrixIdentity());

		StartUp_Draw(timeStep);
		Renderer2D_EndScene();
		break;
	}
	case TITLE_MENU:
	{
		RendererAPI_SetClearColor({ 0.3f,0.3f,0.3f,1.0f });
		RendererAPI_Clear();

		Scene_OnViewportResize(s_Data.TitleScene, Window_GetWidth(), Window_GetHeight());
		Scene_OnUpdate(s_Data.TitleScene, timeStep);
		break;
	}
	case PLAY_SCENE:
	{
		RendererAPI_SetClearColor({ 0.3f,0.3f,0.3f,1.0f });
		RendererAPI_Clear();

		Scene_OnViewportResize(s_Data.PlayScene, Window_GetWidth(), Window_GetHeight());
		Scene_OnUpdate(s_Data.PlayScene, timeStep);
		break;
	}
	}
}

void Game_Shutdown(Application* appInst)
{
	Scene_Destroy(s_Data.TitleScene);

	Loading_Destroy();
}

void Game_SetMode(GameMode mode)
{
	switch (s_Data.Mode)
	{
	case PLAY_SCENE:
	{
		s_Data.PlaySceneDestroyRequested = true;
		break;
	}
	}

	switch (mode)
	{
	case PLAY_SCENE:
	{
		Scene_Create(s_Data.PlayScene);
		ScriptGlue_CreatePlayScene(s_Data.PlayScene);
		Scene_Ininialize(s_Data.PlayScene);
		break;
	}
	}

	s_Data.Mode = mode;
}

void Game_OnKeyPressed(Application* appInst, KeyCode key)
{
	TitleMenuController_OnKeyPressed(key);
}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Gunslayer";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = false;
	appInst->Spec.Maximizable = false;
	appInst->Spec.Minimizable = true;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	if (appInst->Spec.CommandLineArgs.Count > 1)
	{
		appInst->Spec.FullScreen = false;
		char* temp = "nonfullscreen";
		for (size_t i = 0; i < strlen(temp); i++)
		{
			if (tolower(appInst->Spec.CommandLineArgs.Args[1][i]) != temp[i])
			{
				appInst->Spec.FullScreen = true;
				break;
			}
		}
	}
	else
	{
		appInst->Spec.FullScreen = true;
	}
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
	appInst->OnKeyPressed = Game_OnKeyPressed;
}