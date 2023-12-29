#include "pch.h"
#include "Application.h"
#include "Window.h"
#include "Renderer/RendererAPI.h"
#include "Input/Input.h"
#include "Audio/Audio.h"
#include "Asset/AssetManager.h"

#include <time.h>

struct Application_State
{
	Application* AppInst;

	bool IsRunning = true;
	bool Minimized = false;

	float TimeScale = 1.0f;
	float LastFrameTime;
};
static Application_State s_AppState;

void Application_Close()
{
	s_AppState.IsRunning = false;
}

bool OnWndResize(Event e)
{
	uint32_t width = e.Context.WndResize.Width;
	uint32_t height = e.Context.WndResize.Height;
	if (width == 0.0f || height == 0.0f)
	{
		s_AppState.Minimized = true;
		return true;
	}

	s_AppState.Minimized = false;
	RendererAPI_SetViewport(width, height);
	return true;
}

bool OnWndClose(Event e)
{
	Application_Close();
	return true;
}

void Application_OnEvent(Event e)
{
	Event_Dispatcher(EventType_WindowClose, e, OnWndClose);
	Event_Dispatcher(EventType_WindowResize, e, OnWndResize);

	if (!e.Handled)
		s_AppState.AppInst->OnEvent(e);
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
		Application_OnEvent,
	};

	Window_Create(props);

	// Make sure AssetManager is initialized before RendererAPI
	// because RendererAPI will use AssetManager to load shaders which are memory assets
	AssetManager_Init();
	RendererAPI_Initialize();

	if (appInst->Spec.FullScreen)
	{
		RendererContext_SetFullscreen(true);
	}

	Audio_Init();
	Input_Init();

	s_AppState.AppInst->Ininialize(s_AppState.AppInst);
}

void Application_SetTimeScale(float timeScale)
{
	s_AppState.TimeScale = timeScale;

	if (timeScale == 0)
	{
		Audio_Suspend();
	}
	else
	{
		Audio_Resume();
	}
}

void Application_SetFullScreen(bool fullscreen)
{
	s_AppState.AppInst->Spec.FullScreen = fullscreen;
	RendererContext_SetFullscreen(fullscreen);
}

void Application_Run()
{
	while (s_AppState.IsRunning)
	{
		float currentTime = clock();
		float timestep = (float)(currentTime - s_AppState.LastFrameTime) / CLOCKS_PER_SEC;
		s_AppState.LastFrameTime = currentTime;

		timestep *= s_AppState.TimeScale;

		if (!s_AppState.Minimized)
		{
			Input_Update();
			Audio_Update();

			s_AppState.AppInst->Update(timestep);
		}

		Window_Update();
	}

	Window_Shutdown();
}

void Application_Shutdown()
{
	s_AppState.AppInst->Shutdown();
	Audio_Shutdown();
	Input_Shutdown();
	AssetManager_Shutdown();
	RendererAPI_Shutdown();
}