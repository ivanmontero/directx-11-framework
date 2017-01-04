#include "Mouse.h"
#include "Window.h"


IDirectInputDevice8*	Mouse::DIMouse = nullptr;
DIMOUSESTATE			Mouse::CurrentMouseState;

bool Mouse::Initialize(LPDIRECTINPUT8 DirectInput)
{
	if(FAILED(DirectInput->CreateDevice(GUID_SysMouse, &DIMouse, nullptr)))
	{
		return false;
	}
	if(FAILED(DIMouse->SetDataFormat(&c_dfDIMouse)))
	{
		return false;
	}
	if(FAILED(DIMouse->SetCooperativeLevel(Window::Handle, DISCL_EXCLUSIVE | DISCL_NOWINKEY | DISCL_FOREGROUND)))
	{
		return false;
	}

	return true;
}

void Mouse::Poll()
{
	DIMouse->Acquire();
	DIMouse->GetDeviceState(sizeof(DIMOUSESTATE), &CurrentMouseState);
}

int Mouse::GetDX()
{
	return CurrentMouseState.lX;
}

int Mouse::GetDY()
{
	return CurrentMouseState.lY;
}

int Mouse::GetDMouseWheel()
{
	return CurrentMouseState.lZ;
}

bool Mouse::IsButtonPressed(int button)
{
	return (CurrentMouseState.rgbButtons[button] & 0x80) != 0;
}

void Mouse::Dispose()
{
	DIMouse->Unacquire();
	DIMouse->Release();
}

