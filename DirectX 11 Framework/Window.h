#pragma once
#include <Windows.h>

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

class Window
{
private:
	static const WCHAR* ApplicationName;
	static int Width;
	static int Height;

public:
	static HINSTANCE Instance;
	static HWND Handle;

	static bool Initialize(int, int);
	static void Show();
	static void Hide();
	static void Dispose();
	static SIZE GetSize();
};