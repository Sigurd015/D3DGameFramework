#include "DX2D.h"
#include "EntryPoint.h"

#include <stdio.h>

void Sandbox_Ininialize(Application* appInst)
{
	CORE_LOG_INFO("Test Msg");
	CORE_LOG_WARN("Test Msg");
	CORE_LOG_ERROR("Test Msg");
}

void Sandbox_Update(float timeStep)
{
	static char buffer[32];
	sprintf(buffer, "%f", timeStep);
	Window_SetWindowTitle(buffer);
}

void Sandbox_Shutdown(Application* appInst)
{}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "SandBox App";
	appInst->Spec.CommandLineArgs = args;
	appInst->Ininialize = Sandbox_Ininialize;
	appInst->Shutdown = Sandbox_Shutdown;
	appInst->Update = Sandbox_Update;
}