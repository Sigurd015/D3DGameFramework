#include "Core.h"
#include "EntryPoint.h"
#include "ScriptGlue.h"

#include <stdio.h>

enum GameMode
{
	STARTUP_TITLE = 0, TITLE_MENU, PLAY_SCENE
};

struct GameData
{
	Scene PlayScene;
	Scene TitleScene;

	GameMode Mode = STARTUP_TITLE;
};
static GameData s_Data;

struct StatrtUpTitleData
{

};

void DrawStartUpTitle()
{
	RendererAPI_SetClearColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	RendererAPI_Clear();


}

void Game_Ininialize(Application* appInst)
{
	APP_LOG_ERROR("ERROR");
	APP_LOG_WARN("WARN");
	APP_LOG_INFO("INFO");

	Scene_Create(s_Data.PlayScene);
	ScriptGlue_CreatePlayScene(s_Data.PlayScene);
	Scene_Ininialize(s_Data.PlayScene);
	ScriptGlue_CreateTitleScene(s_Data.TitleScene);
	Scene_Ininialize(s_Data.TitleScene);
}

void Game_Update(float timeStep)
{
	switch (s_Data.Mode)
	{
	case STARTUP_TITLE:
	{
		break;
	}
	}

	const Vec4 color = { 0.3f, 0.3f, 0.3f, 1.0f };

	Scene_OnViewportResize(s_Data.PlayScene, Window_GetWidth(), Window_GetHeight());

	RendererAPI_SetClearColor(color);
	RendererAPI_Clear();

#ifndef BV_DIST
	Scene_OnUpdate(s_Data.PlayScene, timeStep, true);
#else
	Scene_OnUpdate(s_Data.PlayScene, timeStep);
#endif
}

void Game_Shutdown(Application* appInst)
{
	Scene_Destroy(s_Data.PlayScene);
}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Bloodveil";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = false;
	appInst->Spec.Maximizable = false;
	appInst->Spec.Minimizable = true;
	appInst->Spec.MaximizedOnStart = false;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
}