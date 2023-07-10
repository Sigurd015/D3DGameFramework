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
	char* Name = "DX2D App";
	uint32_t Width = 1920;
	uint32_t Height = 1080;
	bool Resizable = false;
	bool Maximized = false;
	bool VSync = true;
	ApplicationCommandLineArgs CommandLineArgs;
};

void Application_Ininialize(Application* appInst);

void Application_Run();