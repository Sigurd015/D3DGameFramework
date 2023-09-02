#include "pch.h"
#include "Application.h"
#include "ApplicationTypes.h"
#include "Window.h"
#include "Renderer/RendererAPI.h"

#include <Audio.h>
#include <time.h>

struct Application_State
{
	Application* AppInst;

	DirectX::AudioEngine* AudioEngine;

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

void Application_OnKeyPressed(KeyCode key)
{
	s_AppState.AppInst->OnKeyPressed(s_AppState.AppInst, key);
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
		Application_Resize,
		Application_Close,
		Application_OnKeyPressed,
	};

	Window_Create(props);

	RendererAPI_Initialize();

	if (appInst->Spec.FullScreen)
	{
		Window_SetFullScreen();
	}

	s_AppState.AudioEngine = new DirectX::AudioEngine();

	s_AppState.AppInst->Ininialize(s_AppState.AppInst);
}

void Application_SetTimeScale(float timeScale)
{
	s_AppState.TimeScale = timeScale;

	if (timeScale == 0)
		s_AppState.AudioEngine->Suspend();
	else
		s_AppState.AudioEngine->Resume();
}

void* Application_CreateSoundEffect(const WCHAR* path)
{
	DirectX::SoundEffect* soundEffect = new DirectX::SoundEffect(s_AppState.AudioEngine, path);
	return soundEffect;
}

void Application_Run()
{
	while (s_AppState.IsRunning)
	{
		float currentTime = clock();
		float timestep = (float)(currentTime - s_AppState.LastFrameTime) / CLOCKS_PER_SEC;
		s_AppState.LastFrameTime = currentTime;

		timestep *= s_AppState.TimeScale;

		s_AppState.AudioEngine->Update();

		if (!s_AppState.Minimized)
		{
			s_AppState.AppInst->Update(timestep);
		}

		Window_Update();
	}

	Window_Shutdown();
}

void Application_Shutdown()
{
	s_AppState.AppInst->Shutdown(s_AppState.AppInst);
	delete s_AppState.AudioEngine;
	RendererAPI_Shutdown();
}