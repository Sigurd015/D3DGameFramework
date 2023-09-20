#pragma once
#include "Input/Keycode.h"
#include "Events/Event.h"

#include <stdint.h>
#include <Esent.h>

struct ApplicationCommandLineArgs
{
	int Count = 0;
	char** Args = nullptr;
};

struct ApplicationSpecification
{
	char* Name = "App";
	uint32_t Width = 1920;
	uint32_t Height = 1080;
	bool Resizable = false;
	bool Maximizable = false;
	bool Minimizable = false;
	bool FullScreen = false;
	bool VSync = true;

	ApplicationCommandLineArgs CommandLineArgs;
};

struct Application
{
	ApplicationSpecification Spec;

	void(*Ininialize)(Application* appInst);
	void(*Shutdown)();
	void(*Update)(float timeStep);
	void(*OnEvent)(Event* e);
};

void Application_Ininialize(Application* appInst);
void Application_SetTimeScale(float timeScale);
void Application_Run();
void Application_Close();
void Application_Shutdown();