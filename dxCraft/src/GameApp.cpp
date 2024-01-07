#include "Core.h"
#include "EntryPoint.h"
#include "ScriptGlue.hpp"

#include <imgui.h>

struct GameData
{
	Scene TestScene;

#ifndef CORE_DIST
	uint32_t RenderingMode = 0; // 0: Final, 1: Albedo, 2: MRE, 3: Normal, 4: Position
#endif 
};
static GameData s_Data;

void Game_Ininialize(Application* appInst)
{
	s_Data = {};

	ScriptGlue_LoadAssets();

	Scene_Create(s_Data.TestScene);
	ScriptGlue_CreateTestScene(s_Data.TestScene);
	Scene_Ininialize(s_Data.TestScene);
}

void Game_Update(float timeStep)
{
	Scene_OnViewportResize(s_Data.TestScene, Window_GetWidth(), Window_GetHeight());
	Scene_OnUpdate(s_Data.TestScene, timeStep);

	Image2D* image = RenderPass_GetOutput(SceneRenderer_GetFinalPass());

#ifndef CORE_DIST
	if (Input_GetKeyDown(KeyCode_D0))
	{
		s_Data.RenderingMode = 0;
	}
	else if (Input_GetKeyDown(KeyCode_D1))
	{
		s_Data.RenderingMode = 1;
	}
	else if (Input_GetKeyDown(KeyCode_D2))
	{
		s_Data.RenderingMode = 2;
	}
	else if (Input_GetKeyDown(KeyCode_D3))
	{
		s_Data.RenderingMode = 3;
	}
	else if (Input_GetKeyDown(KeyCode_D4))
	{
		s_Data.RenderingMode = 4;
	}

	switch (s_Data.RenderingMode)
	{
	case 0:
		image = RenderPass_GetOutput(SceneRenderer_GetFinalPass());
		break;
	case 1:
		image = SceneRenderer_GetGBufferAlbedo();
		break;
	case 2:
		image = SceneRenderer_GetGBufferMRE();
		break;
	case 3:
		image = SceneRenderer_GetGBufferNormal();
		break;
	case 4:
		image = SceneRenderer_GetGBufferPosition();
		break;
	}
#endif
	RendererAPI_CompositeToSwapChain(image);
}

void Game_OnImGuiRender()
{
#ifndef CORE_DIST
	//ImGui::Begin("Debug");
	//ImGui::Text("Test");
	//ImGui::End();
#endif 
}

void Game_Shutdown()
{
	Scene_Destroy(s_Data.TestScene);
}

bool OnKeyPressed(Event e)
{
	//APP_LOG_INFO("Key Pressed: %c , IsRepeat : %d", (char)e.Context.KeyPressed.Keycode, e.Context.KeyPressed.IsRepeat);
	return true;
}

bool OnMouseMoved(Event e)
{
	//float x = e.Context.MouseMoved.x;
	//float y = e.Context.MouseMoved.y;
	//APP_LOG_INFO("Mouse Moved: X: %f, Y: %f", x, y);
	return true;
}

void Game_OnEvent(Event e)
{
	Event_Dispatcher(EventType_KeyPressed, e, OnKeyPressed);
	Event_Dispatcher(EventType_MouseMoved, e, OnMouseMoved);
}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "dxCraft";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = true;
	appInst->Spec.Maximizable = true;
	appInst->Spec.Minimizable = true;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	appInst->Spec.FullScreen = false;

#ifndef CORE_DIST
	appInst->Spec.EnableImGui = true;
#endif 

	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
	appInst->OnEvent = Game_OnEvent;
	appInst->OnImGuiRender = Game_OnImGuiRender;
}