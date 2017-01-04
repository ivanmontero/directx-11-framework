#include "Window.h"
#include "Renderer.h"

int				Window::Width				= 0;
int				Window::Height				= 0;
const WCHAR*	Window::ApplicationName		= L"DirectX 11 Framework";
HINSTANCE		Window::Instance			= nullptr;
HWND			Window::Handle				= nullptr;

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_KEYDOWN: // For debugging
		if (wParam == VK_ESCAPE) {
			DestroyWindow(hwnd);
		}
		return 0;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_SIZE:
		if(Renderer::IsInitialized()) Renderer::Resize(LOWORD(lParam), HIWORD(lParam));
		break;
	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

bool Window::Initialize(int width, int height)
{
	Instance = GetModuleHandle(nullptr);
	Width = width;
	Height = height;

	WNDCLASSEX wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = Instance;
	wcex.hIcon = LoadIcon(nullptr, IDI_WINLOGO);
	wcex.hIconSm = wcex.hIcon;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName = nullptr;
	wcex.lpszClassName = ApplicationName;

	if (!RegisterClassEx(&wcex)) // Or: RegisterClassExW()
	{
		return false;
	}

	int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	int screenHeight = GetSystemMetrics(SM_CYSCREEN);

	Handle = CreateWindowEx(WS_EX_APPWINDOW, ApplicationName, ApplicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP | WS_OVERLAPPEDWINDOW,
		screenWidth / 2 - Width / 2, screenHeight / 2 - Height / 2,
		Width, Height, nullptr, nullptr, Instance, nullptr);

	if (!Handle)
	{
		return false;
	}

	return true;
}


void Window::Show()
{
	ShowWindow(Handle, SW_SHOW);
	SetForegroundWindow(Handle);
	SetFocus(Handle);
}

void Window::Hide()
{
	ShowWindow(Handle, SW_HIDE);
}

void Window::Dispose()
{
	if(Handle)
	{
		return;
	}
	DestroyWindow(Handle);
	Handle = nullptr;
	UnregisterClass(ApplicationName, Instance);
	Instance = nullptr;
}

SIZE Window::GetSize()
{
	SIZE result;
	RECT rect;
	GetClientRect(Handle, &rect);
	result.cx = rect.right - rect.left;
	result.cy = rect.bottom - rect.top;
	return result;
}
