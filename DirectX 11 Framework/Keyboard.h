#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Keyboard
{
private:
	static IDirectInputDevice8* DIKeyboard;
	static BYTE KeyboardState[256];
public:
	static bool Initialize(LPDIRECTINPUT8);
	static void Poll();
	static bool IsKeyDown(int);
	static void Dispose();
};