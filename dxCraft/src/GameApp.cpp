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
	Scene_OnViewportResize(s_Data.TestScene, Window_GetWidth(), Window_GetHeight());
	Scene_OnUpdate(s_Data.TestScene, timeStep);

	RendererAPI_CompositeToSwapChain(RenderPass_GetOutput(SceneRenderer_GetFinalPass()));
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
	appInst->Spec.Width = 800;
	appInst->Spec.Height = 600;
	appInst->Spec.Resizable = true;
	appInst->Spec.Maximizable = true;
	appInst->Spec.Minimizable = true;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	appInst->Spec.FullScreen = false;
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
	appInst->OnEvent = Game_OnEvent;
}