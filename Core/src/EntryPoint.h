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
#include <crtdbg.h>

int main(int argc, char** argv)
{
	#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	#endif

	return Main(argc, argv);
}

#endif