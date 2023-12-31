﻿#include "Core.h"
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
	Game_SetMode(LOADING_SCENE);
}

void Game_Update(float timeStep)
{
	if (s_Data.PlaySceneDestroyRequested)
	{
		Scene_Destroy(s_Data.PlayScene);
		s_Data.PlaySceneDestroyRequested = false;
	}

	switch (s_Data.Mode)
	{
	case LOADING_SCENE:
	{
		// For some effect, which need to directly draw to swap chain, 
		// call RendererAPI_Clear to clear the back buffer
		// and the call Renderer2D_ResetTargetFrameBuffer to reset the target frame buffer to swap chain
		RendererAPI_Clear();
		Renderer2D_ResetTargetFrameBuffer();

		Renderer2D_BeginScene(DirectX::XMMatrixIdentity());

		Loading_Draw(timeStep);
		Renderer2D_EndScene();
		break;
	}
	case STARTUP_TITLE:
	{
		RendererAPI_Clear();
		Renderer2D_ResetTargetFrameBuffer();

		Renderer2D_BeginScene(DirectX::XMMatrixIdentity());

		StartUp_Draw(timeStep);
		Renderer2D_EndScene();
		break;
	}
	case TITLE_MENU:
	{
		// For scene rendering, which will be composited to swap chain,
		// remember to call RendererAPI_CompositeToSwapChain at the last,
		// and pass the final image, get from SceneRenderer_GetFinalPass,
		// rendering result can automatically be composited to back buffer
		Scene_OnViewportResize(s_Data.TitleScene, Window_GetWidth(), Window_GetHeight());
		Scene_OnUpdate(s_Data.TitleScene, timeStep);
		
		// TODO: Better way to do this       	
		RendererAPI_CompositeToSwapChain(RenderPass_GetOutput(SceneRenderer_GetFinalPass()));
		break;
	}
	case PLAY_SCENE:
	{
		Scene_OnViewportResize(s_Data.PlayScene, Window_GetWidth(), Window_GetHeight());
		Scene_OnUpdate(s_Data.PlayScene, timeStep);
	
		// TODO: Better way to do this    	
		RendererAPI_CompositeToSwapChain(RenderPass_GetOutput(SceneRenderer_GetFinalPass()));
		break;
	}
	}
}

void Game_Shutdown()
{
	Scene_Destroy(s_Data.TitleScene);
	if (s_Data.PlaySceneDestroyRequested)
	{
		Scene_Destroy(s_Data.PlayScene);
		s_Data.PlaySceneDestroyRequested = false;
	}
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

bool OnKeyPressed(Event e)
{
	TitleMenuController_OnKeyPressed(e.Context.KeyPressed.Keycode);
	return true;
}

void Game_OnEvent(Event e)
{
	Event_Dispatcher(EventType_KeyPressed, e, OnKeyPressed);
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
	appInst->OnEvent = Game_OnEvent;
}