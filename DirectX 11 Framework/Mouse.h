#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Mouse
{
private:
	static IDirectInputDevice8* DIMouse;
	static DIMOUSESTATE CurrentMouseState;
public:
	static bool Initialize(LPDIRECTINPUT8);
	static void Poll();
	static int GetDX();
	static int GetDY();
	static int GetDMouseWheel();
	static bool IsButtonPressed(int);
	static void Dispose();
};