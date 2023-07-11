#include "Core.h"
#include "EntryPoint.h"

#include <stdio.h>

void Game_Ininialize(Application* appInst)
{
	CORE_LOG_INFO("Test Msg");
	CORE_LOG_WARN("Test Msg");
	CORE_LOG_ERROR("Test Msg");
}

void Game_Update(float timeStep)
{
	Vec4 color = { 0, 0, 0, 0 };

	if (Input_IsKeyPressed(KeyCode::A))
		color.x = 1.0f;
	if (Input_IsKeyPressed(KeyCode::S))
		color.y = 1.0f;
	if (Input_IsKeyPressed(KeyCode::D))
		color.z = 1.0f;
	if (Input_IsKeyPressed(KeyCode::W))
		color.w = 1.0f;

	RendererAPI_SetClearColor(&color);
	RendererAPI_Clear();
}

void Game_Shutdown(Application* appInst)
{}

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