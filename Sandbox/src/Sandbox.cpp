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

	if (Input_GetKey(GamePadCode::ButtonA))
		APP_LOG_INFO("GamePad : A Pressed");
	if (Input_GetKey(GamePadCode::ButtonB))
		APP_LOG_INFO("GamePad : B Pressed");
	if (Input_GetKey(GamePadCode::ButtonX))
		APP_LOG_INFO("GamePad : X Pressed");
	if (Input_GetKey(GamePadCode::ButtonY))
		APP_LOG_INFO("GamePad : Y Pressed");
	if (Input_GetKey(GamePadCode::LeftShoulder))
		APP_LOG_INFO("GamePad : LB Pressed");
	if (Input_GetKey(GamePadCode::RightShoulder))
		APP_LOG_INFO("GamePad : RB Pressed");
	if (Input_GetKey(GamePadCode::Back))
		APP_LOG_INFO("GamePad : Back Pressed");
	if (Input_GetKey(GamePadCode::Start))
		APP_LOG_INFO("GamePad : Start Pressed");
	if (Input_GetKey(GamePadCode::LeftThumbStick))
		APP_LOG_INFO("GamePad : LS Pressed");
	if (Input_GetKey(GamePadCode::RightThumbStick))
		APP_LOG_INFO("GamePad : RS Pressed");
	if (Input_GetKey(GamePadCode::DPadUp))
		APP_LOG_INFO("GamePad : DPadUp Pressed");
	if (Input_GetKey(GamePadCode::DPadRight))
		APP_LOG_INFO("GamePad : DPadRight Pressed");
	if (Input_GetKey(GamePadCode::DPadDown))
		APP_LOG_INFO("GamePad : DPadDown Pressed");
	if (Input_GetKey(GamePadCode::DPadLeft))
		APP_LOG_INFO("GamePad : DPadLeft Pressed");
	if (Input_GetKey(GamePadCode::LeftTrigger))
		APP_LOG_INFO("GamePad : LTrigger Pressed");
	if (Input_GetKey(GamePadCode::RightTrigger))
		APP_LOG_INFO("GamePad : RTrigger Pressed");
}

void Game_Shutdown()
{
	Scene_Destroy(s_Data.TestScene);
}

bool OnKeyPressed(const Event* e)
{
	KeyPressedEvent* event = (KeyPressedEvent*)e->Data;
	APP_LOG_INFO("Key Pressed: %c , IsRepeat : %d", (char)event->Keycode, event->IsRepeat);
	return true;
}

bool OnMouseMoved(const Event* e)
{
	MouseMovedEvent* event = (MouseMovedEvent*)e->Data;
	//APP_LOG_INFO("Mouse Moved: X: %f, Y: %f", event->x, event->y);
	return true;
}

void Game_OnEvent(Event* e)
{
	Event_Dispatcher(EventType_KeyPressed, e, OnKeyPressed);
	Event_Dispatcher(EventType_MouseMoved, e, OnMouseMoved);
}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Sandbox";
	appInst->Spec.Width = 800;
	appInst->Spec.Height = 600;
	appInst->Spec.Resizable = true;
	appInst->Spec.Maximizable = true;
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