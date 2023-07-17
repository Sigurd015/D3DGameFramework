#include "Core.h"
#include "EntryPoint.h"

#include <stdio.h>

void Game_Ininialize(Application* appInst)
{
	APP_LOG_INFO("Test Msg");
	APP_LOG_INFO("Test Msg");
	APP_LOG_INFO("Test Msg");

	{
		List list;
		List_Create(list);

		for (uint32_t i = 0; i < 1500; i++)
		{
			int* newInt = (int*)malloc(sizeof(int));
			*newInt = i;
			List_Add(list, newInt);
		}

		for (uint32_t i = 0; i < List_Size(list); i++)
		{
			char tempChar[256];
			sprintf_s(tempChar, 256, "%d", *(int*)List_Get(list, i));
			APP_LOG_INFO(tempChar);
		}

		List_Clear(list);

		for (uint32_t i = 0; i < 800; i++)
		{
			int* newInt = (int*)malloc(sizeof(int));
			*newInt = i;
			List_Add(list, newInt);
		}

		for (uint32_t i = 0; i < List_Size(list); i++)
		{
			char tempChar[256];
			sprintf_s(tempChar, 256, "%d", *(int*)List_Get(list, i));
			APP_LOG_INFO(tempChar);
		}

		List_Free(list);
	}

	{
		List list;
		List_Create(list);

		for (uint32_t i = 0; i < 1500; i++)
		{
			char tempChar[256];
			sprintf_s(tempChar, 256, "the number is %d", i);
			char* newString = strdup(tempChar);
			List_Add(list, newString);
		}

		for (uint32_t i = 0; i < List_Size(list); i++)
		{
			APP_LOG_INFO((char*)List_Get(list, i));
		}

		List_Clear(list);

		for (uint32_t i = 0; i < 800; i++)
		{
			char tempChar[256];
			sprintf_s(tempChar, 256, "the number is %d", i);
			char* newString = strdup(tempChar);
			List_Add(list, newString);
		}

		for (uint32_t i = 0; i < List_Size(list); i++)
		{
			APP_LOG_INFO((char*)List_Get(list, i));
		}

		List_Free(list);
	}

	Scene scene;
	SceneSerializer_Deserialize(scene, "assets/test.txt");
}

void Game_Update(float timeStep)
{
	Vec4 color = { 0, 0, 0, 0 };

	if (Input_IsKeyPressed(KeyCode::A))
		color.x = 1.0f;
	if (Input_IsKeyPressed(KeyCode::S))
		color.y = 1.0f;
	if (Input_IsKeyPressed(KeyCode::D))
		color.z = 1.0f;
	if (Input_IsKeyPressed(KeyCode::W))
		color.w = 1.0f;

	RendererAPI_SetClearColor(color);
	RendererAPI_Clear();
}

void Game_Shutdown(Application* appInst)
{}

void CreateApplication(Application* appInst, ApplicationCommandLineArgs args)
{
	appInst->Spec.Name = "Bloodveil";
	appInst->Spec.Width = 1920;
	appInst->Spec.Height = 1080;
	appInst->Spec.Resizable = false;
	appInst->Spec.Maximizable = false;
	appInst->Spec.Minimizable = true;
	appInst->Spec.MaximizedOnStart = false;
	appInst->Spec.VSync = true;
	appInst->Spec.CommandLineArgs = args;
	appInst->Ininialize = Game_Ininialize;
	appInst->Shutdown = Game_Shutdown;
	appInst->Update = Game_Update;
}