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

	if (Input_GetKey(GamePadCode_A))
		APP_LOG_INFO("GamePad : A Pressed");
	if (Input_GetKey(GamePadCode_B))
		APP_LOG_INFO("GamePad : B Pressed");
	if (Input_GetKey(GamePadCode_X))
		APP_LOG_INFO("GamePad : X Pressed");
	if (Input_GetKey(GamePadCode_Y))
		APP_LOG_INFO("GamePad : Y Pressed");
	if (Input_GetKey(GamePadCode_LeftShoulder))
		APP_LOG_INFO("GamePad : LB Pressed");
	if (Input_GetKey(GamePadCode_RightShoulder))
		APP_LOG_INFO("GamePad : RB Pressed");
	if (Input_GetKey(GamePadCode_Back))
		APP_LOG_INFO("GamePad : Back Pressed");
	if (Input_GetKey(GamePadCode_Start))
		APP_LOG_INFO("GamePad : Start Pressed");
	if (Input_GetKey(GamePadCode_LeftThumbStick))
		APP_LOG_INFO("GamePad : LS Pressed");
	if (Input_GetKey(GamePadCode_RightThumbStick))
		APP_LOG_INFO("GamePad : RS Pressed");
	if (Input_GetKey(GamePadCode_DPadUp))
		APP_LOG_INFO("GamePad : DPadUp Pressed");
	if (Input_GetKey(GamePadCode_DPadRight))
		APP_LOG_INFO("GamePad : DPadRight Pressed");
	if (Input_GetKey(GamePadCode_DPadDown))
		APP_LOG_INFO("GamePad : DPadDown Pressed");
	if (Input_GetKey(GamePadCode_DPadLeft))
		APP_LOG_INFO("GamePad : DPadLeft Pressed");
	if (Input_GetKey(GamePadCode_LeftTrigger))
		APP_LOG_INFO("GamePad : LTrigger Pressed");
	if (Input_GetKey(GamePadCode_RightTrigger))
		APP_LOG_INFO("GamePad : RTrigger Pressed");

	if (Input_GetKeyDown(GamePadCode_A))
		APP_LOG_INFO("GamePad : A Down");
	if (Input_GetKeyDown(GamePadCode_B))
		APP_LOG_INFO("GamePad : B Down");
	if (Input_GetKeyDown(GamePadCode_X))
		APP_LOG_INFO("GamePad : X Down");
	if (Input_GetKeyDown(GamePadCode_Y))
		APP_LOG_INFO("GamePad : Y Down");
	if (Input_GetKeyDown(GamePadCode_LeftShoulder))
		APP_LOG_INFO("GamePad : LB Down");
	if (Input_GetKeyDown(GamePadCode_RightShoulder))
		APP_LOG_INFO("GamePad : RB Down");
	if (Input_GetKeyDown(GamePadCode_Back))
		APP_LOG_INFO("GamePad : Back Down");
	if (Input_GetKeyDown(GamePadCode_Start))
		APP_LOG_INFO("GamePad : Start Down");
	if (Input_GetKeyDown(GamePadCode_LeftThumbStick))
		APP_LOG_INFO("GamePad : LS Down");
	if (Input_GetKeyDown(GamePadCode_RightThumbStick))
		APP_LOG_INFO("GamePad : RS Down");
	if (Input_GetKeyDown(GamePadCode_DPadUp))
		APP_LOG_INFO("GamePad : DPadUp Down");
	if (Input_GetKeyDown(GamePadCode_DPadRight))
		APP_LOG_INFO("GamePad : DPadRight Down");
	if (Input_GetKeyDown(GamePadCode_DPadDown))
		APP_LOG_INFO("GamePad : DPadDown Down");
	if (Input_GetKeyDown(GamePadCode_DPadLeft))
		APP_LOG_INFO("GamePad : DPadLeft Down");
	if (Input_GetKeyDown(GamePadCode_LeftTrigger))
		APP_LOG_INFO("GamePad : LTrigger Down");
	if (Input_GetKeyDown(GamePadCode_RightTrigger))
		APP_LOG_INFO("GamePad : RTrigger Down");

	//Vec2 leftStick = Input_GetLeftStick();
	//Vec2 rightStick = Input_GetRightStick();
	//APP_LOG_INFO("LeftStick : X: %f, Y: %f", leftStick.x, leftStick.y);
	//APP_LOG_INFO("RightStick : X: %f, Y: %f", rightStick.x, rightStick.y);

	//if (Input_GetKey(KeyCode_W))
	//	APP_LOG_INFO("W Pressed");
	//if (Input_GetKey(KeyCode_A))
	//	APP_LOG_INFO("A Pressed");
	//if (Input_GetKey(KeyCode_S))
	//	APP_LOG_INFO("S Pressed");
	//if (Input_GetKey(KeyCode_D))
	//	APP_LOG_INFO("D Pressed");

	//if (Input_GetKeyDown(KeyCode_W))
	//	APP_LOG_INFO("W Down");
	//if (Input_GetKeyDown(KeyCode_A))
	//	APP_LOG_INFO("A Down");
	//if (Input_GetKeyDown(KeyCode_S))
	//	APP_LOG_INFO("S Down");
	//if (Input_GetKeyDown(KeyCode_D))
	//	APP_LOG_INFO("D Down");

	//if (Input_GetMouseButtonDown(MouseCode_ButtonLeft))
	//	APP_LOG_INFO("Left Mouse Down");

	//if (Input_GetMouseButton(MouseCode_ButtonLeft))
	//	APP_LOG_INFO("Left Mouse Pressed");

	if (Input_GetKeyDown(KeyCode_F1))
		Application_SetFullScreen(true);
	if (Input_GetKeyDown(KeyCode_F2))
		Application_SetFullScreen(false);
}

void Game_Shutdown()
{
	Scene_Destroy(s_Data.TestScene);
}

bool OnKeyPressed(Event e)
{
	APP_LOG_INFO("Key Pressed: %c , IsRepeat : %d", (char)e.Context.KeyPressed.Keycode, e.Context.KeyPressed.IsRepeat);
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