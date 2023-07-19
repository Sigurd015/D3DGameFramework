#include "pch.h"
#include "Application.h"
#include "ApplicationTypes.h"
#include "Window.h"
#include "Renderer/RendererAPI.h"

#include <time.h>

struct Application_State
{
	Application* AppInst;

	bool IsRunning = true;
	bool Minimized = false;

	float LastFrameTime;
};

static Application_State s_AppState;

void Application_Close()
{
	s_AppState.IsRunning = false;
}

void Application_Resize(float width, float height)
{
	if (width == 0.0f || height == 0.0f)
	{
		s_AppState.Minimized = true;
		return;
	}

	s_AppState.Minimized = false;
	RendererAPI_SetViewport(width, height);
}

void Application_Ininialize(Application* appInst)
{
	s_AppState.AppInst = appInst;

	WindowProps props = {
		appInst->Spec.Name,
		appInst->Spec.Width,
		appInst->Spec.Height,
		appInst->Spec.VSync,
		appInst->Spec.Resizable,
		appInst->Spec.Maximizable,
		appInst->Spec.Minimizable,
		appInst->Spec.MaximizedOnStart,
		Application_Resize,
		Application_Close,
	};

	Window_Create(&props);

	RendererAPI_Initialize();

	s_AppState.AppInst->Ininialize(s_AppState.AppInst);
}

void Application_Run()
{
	while (s_AppState.IsRunning)
	{
		float currentTime = clock();
		float timestep = (float)(currentTime - s_AppState.LastFrameTime) / CLOCKS_PER_SEC;
		s_AppState.LastFrameTime = currentTime;

		if (!s_AppState.Minimized)
		{
			s_AppState.AppInst->Update(timestep);
		}

		Window_Update();
	}
}

void Application_Shutdown()
{
	s_AppState.AppInst->Shutdown(s_AppState.AppInst);

	RendererAPI_Shutdown();
}