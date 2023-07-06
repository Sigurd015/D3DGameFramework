#include "pch.h"
#include "Application.h"
#include "ApplicationTypes.h"
#include "Window.h"

struct Application_State
{
	Application* AppInst;

	bool IsRunning = true;
	bool Minimized = false;

	float LastFrameTime;
};

static Application_State s_AppState;

void Application_Create(Application* appInst)
{
	s_AppState.AppInst = appInst;
	s_AppState.IsRunning = true;

	WindowProps props = { appInst->Spec.Name,1920,1080,true ,nullptr };

	Window_Create(&props);

	s_AppState.AppInst->Ininialize(s_AppState.AppInst);
}

void Application_Run()
{
	while (s_AppState.IsRunning)
	{
		if (!s_AppState.Minimized)
		{
			s_AppState.AppInst->Update(0.0f);
		}

		Window_Update();
	}
}