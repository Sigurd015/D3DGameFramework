#pragma once

#include <stdint.h>

struct Application;

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
	bool MaximizedOnStart = false;
	bool VSync = true;

	ApplicationCommandLineArgs CommandLineArgs;
};

void Application_Ininialize(Application* appInst);
void Application_Run();
void Application_Shutdown();