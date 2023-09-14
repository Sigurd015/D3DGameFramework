#include "pch.h"
#include "Window.h"
#include "Renderer/RendererContext.h"

#include <Windows.h>
#include <atlbase.h>
#include <atlconv.h>

struct Window_State
{
	WindowProps Props;
	HWND WndHandle;
};
static Window_State s_WindowState;

LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_SIZE:
	{
		static bool first = true;  //TODO:The first WM_SIZE call before RenderingContext Ininialized can crash the app
		if (first)
		{
			first = false;
			return 0;
		}

		WindowProps& data = s_WindowState.Props;
		if (wParam == SIZE_MINIMIZED)
		{
			data.Width = data.Height = 0;
		}
		else
		{
			data.Width = LOWORD(lParam);
			data.Height = HIWORD(lParam);
		}

		WndResizeEvnet size = { data.Width, data.Height };
		Event* e = Event_Create(EventType_WindowResize, &size);
		data.EventCallback(e);
		return 0;
	}
	case WM_CLOSE:
	{
		WindowProps& data = s_WindowState.Props;
		Event* e = Event_Create(EventType_WindowClose);
		data.EventCallback(e);
		return 0;
	}
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		WindowProps& data = s_WindowState.Props;
		KeyPressedEvent key = { (KeyCode)wParam, lParam & 0x40000000 ? true : false };
		Event* e = Event_Create(EventType_KeyPressed, &key);
		data.EventCallback(e);
		return 0;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		WindowProps& data = s_WindowState.Props;
		KeyReleasedEvent key = { (KeyCode)wParam };
		Event* e = Event_Create(EventType_KeyReleased, &key);
		data.EventCallback(e);
		return 0;
	}
	case WM_CHAR:
	{
		WindowProps& data = s_WindowState.Props;
		KeyTypedEvent key = { (KeyCode)wParam };
		Event* e = Event_Create(EventType_KeyTyped, &key);
		data.EventCallback(e);
		return 0;
	}
	case WM_MOUSEMOVE:
	{
		WindowProps& data = s_WindowState.Props;
		auto point = MAKEPOINTS(lParam);
		MouseMovedEvent mouse = { (float)point.x, (float)point.y };
		Event* e = Event_Create(EventType_MouseMoved, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_MOUSEWHEEL:
	{
		WindowProps& data = s_WindowState.Props;
		MouseScrolledEvent mouse = { (float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA };
		Event* e = Event_Create(EventType_MouseScrolled, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_RBUTTONDOWN:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonPressedEvent mouse = { MouseCode::ButtonRight };
		Event* e = Event_Create(EventType_MouseButtonPressed, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_MBUTTONDOWN:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonPressedEvent mouse = { MouseCode::ButtonMiddle };
		Event* e = Event_Create(EventType_MouseButtonPressed, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_LBUTTONDOWN:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonPressedEvent mouse = { MouseCode::ButtonLeft };
		Event* e = Event_Create(EventType_MouseButtonPressed, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_RBUTTONUP:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonReleasedEvent mouse = { MouseCode::ButtonRight };
		Event* e = Event_Create(EventType_MouseButtonReleased, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_MBUTTONUP:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonReleasedEvent mouse = { MouseCode::ButtonMiddle };
		Event* e = Event_Create(EventType_MouseButtonReleased, &mouse);
		data.EventCallback(e);
		return 0;
	}
	case WM_LBUTTONUP:
	{
		WindowProps& data = s_WindowState.Props;
		MouseButtonReleasedEvent mouse = { MouseCode::ButtonLeft };
		Event* e = Event_Create(EventType_MouseButtonReleased, &mouse);
		data.EventCallback(e);
		return 0;
	}
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
}

void Window_Create(const WindowProps& props)
{
	s_WindowState.Props = props;
	WNDCLASSEX wndClass = { sizeof(WNDCLASSEX), CS_OWNDC,
			WndProc, 0, 0, GetModuleHandle(nullptr), nullptr,
			nullptr, nullptr, nullptr, L"GMAE_CORE", nullptr };

	CORE_ASSERT(RegisterClassEx(&wndClass), "RegisterClass - Failed");

	DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU;

	if (s_WindowState.Props.Minimizable)
		style |= WS_MINIMIZEBOX;

	if (s_WindowState.Props.Maximizable)
		style |= WS_MAXIMIZEBOX;

	if (s_WindowState.Props.Resizable)
		style |= WS_THICKFRAME;

	RECT rect = { 0, 0, s_WindowState.Props.Width, s_WindowState.Props.Height };
	AdjustWindowRect(&rect, style, false);

	s_WindowState.WndHandle = CreateWindowEx(0, wndClass.lpszClassName, CA2T(s_WindowState.Props.Title),
		style, CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, wndClass.hInstance, nullptr);

	CORE_ASSERT(s_WindowState.WndHandle, "CreateWindow - Failed");

	RendererContext_Initialize(&s_WindowState.WndHandle);

	ShowWindow(s_WindowState.WndHandle, SW_SHOW);
}

void DispatchMsg()
{
	MSG msg;
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void Window_Update()
{
	DispatchMsg();

	RendererContext_SwapBuffer(s_WindowState.Props.VSync);
}

void Window_Shutdown()
{
	DestroyWindow(s_WindowState.WndHandle);
}

void Window_SetFullScreen()
{
	DEVMODE devMode = { 0 };

	if (!EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode))
		CORE_LOG_ERROR("EnumDisplaySettings - Failed");

	devMode.dmPelsWidth = s_WindowState.Props.Width;
	devMode.dmPelsHeight = s_WindowState.Props.Height;
	devMode.dmFields |= DM_PELSWIDTH | DM_PELSHEIGHT;

	if (ChangeDisplaySettings(&devMode, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)
		CORE_LOG_ERROR("ChangeDisplaySettings - Failed");

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	SetWindowLongPtr(s_WindowState.WndHandle, GWL_STYLE, WS_POPUP);
	SetWindowPos(s_WindowState.WndHandle, HWND_TOP, 0, 0, screenWidth, screenHeight, SWP_FRAMECHANGED);

	SetCursor(NULL);

	ShowWindow(s_WindowState.WndHandle, SW_SHOW);
}

uint32_t Window_GetWidth()
{
	return s_WindowState.Props.Width;
}

uint32_t Window_GetHeight()
{
	return s_WindowState.Props.Height;
}

void Window_SetWindowTitle(const char* title)
{
	char temp[256];
	sprintf_s(temp, 256, "%s - %s", title, s_WindowState.Props.Title);
	SetWindowTextA(s_WindowState.WndHandle, temp);
}

void Window_SetVSync(bool enable)
{
	s_WindowState.Props.VSync = enable;
}

bool Window_IsVSync()
{
	return s_WindowState.Props.VSync;
}

void* Window_GetWindowHandle()
{
	return &s_WindowState.WndHandle;
}