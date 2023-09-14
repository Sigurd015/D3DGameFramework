#include "Core.h"
#include "EntryPoint.h"
#include "ScriptGlue.hpp"

struct GameData
{
	Scene TestScene;
};
static GameData s_Data;

void Game_Ininialize(Application* appInst)
{
	s_Data = {};
	Scene_Create(s_Data.TestScene);
	ScriptGlue_CreateTestScene(s_Data.TestScene);
	Scene_Ininialize(s_Data.TestScene);
}

void Game_Update(float timeStep)
{
	RendererAPI_SetClearColor({ 0.3f,0.3f,0.3f,1.0f });
	RendererAPI_Clear();

	Scene_OnViewportResize(s_Data.TestScene, Window_GetWidth(), Window_GetHeight());
	Scene_OnUpdate(s_Data.TestScene, timeStep);
}

void Game_Shutdown(Application* appInst)
{}

void Game_OnKeyPressed(Application* appInst, KeyCode key)
{}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Sandbox";
	appInst->Spec.Width = 800;
	appInst->Spec.Height = 600;
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