#include "DX2D.h"
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

}

void Game_Shutdown(Application* appInst)
{}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "SandBox App";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = true;
	appInst->Spec.Maximized = false;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
}