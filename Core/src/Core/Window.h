#include <Windows.h>
#include <stdint.h>

#include "Event/Event.h"

struct WindowProps
{
	char* Title;
	uint32_t Width;
	uint32_t Height;

	bool VSync;
	void(*EventCallBack)(Event* event);
};

void Window_Create(const WindowProps* props);
void Window_Update();
void Window_Shutdown();
uint32_t Window_GetWidth();
uint32_t Window_GetHeight();
void Window_SetWindowTitle(const char* title);
void Window_SetVSync(bool enable);
bool Window_IsVSync();
void* Window_GetWindowHandler();