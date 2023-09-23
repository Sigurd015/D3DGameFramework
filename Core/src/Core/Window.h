#include "Input/KeyCode.h"
#include "Events/Event.h"

#include <stdint.h>

struct WindowProps
{
	char* Title;
	uint32_t Width;
	uint32_t Height;

	bool VSync = true;
	bool Resizable = false;
	bool Maximizable = false;
	bool Minimizable = false;

	void (*EventCallback)(Event e);
};

void Window_Create(const WindowProps& props);
void Window_Update();
void Window_Shutdown();
uint32_t Window_GetWidth();
uint32_t Window_GetHeight();
void Window_SetWindowTitle(const char* title);
void Window_SetVSync(bool enable);
bool Window_IsVSync();
void* Window_GetWindowHandle();