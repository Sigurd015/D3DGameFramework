#pragma once

#include "Core/Application.h"
#include "Core/Log.h"

extern void CreateApplication(Application* appInst, ApplicationCommandLineArgs args);

int Main(int argc, char** argv)
{
	Log_Initialize();
	Application app;
	CreateApplication(&app, { argc, argv });
	Application_Ininialize(&app);
	Application_Run();
	Application_Shutdown();
	Memory_CheckMemoryLeaks();
	Log_Shutdown();
	return 0;
}

#ifdef CORE_DIST
#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Main(__argc, __argv);
}

#else

int main(int argc, char** argv)
{
	return Main(argc, argv);
}

#endif