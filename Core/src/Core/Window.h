#include "Input/Keycode.h"

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

	void (*WindowResize)(float width, float height);
	void (*WindowClose)();
	void (*OnKeyPressed)(KeyCode key);
};

void Window_Create(const WindowProps& props);
void Window_Update();
void Window_Shutdown();
void Window_SetFullScreen();
uint32_t Window_GetWidth();
uint32_t Window_GetHeight();
void Window_SetWindowTitle(const char* title);
void Window_SetVSync(bool enable);
bool Window_IsVSync();
void* Window_GetWindowHandle();