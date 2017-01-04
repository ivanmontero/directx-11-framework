#pragma comment (lib, "dinput8.lib")
#pragma comment (lib, "dxguid.lib")

#include "Game.h"
#include "Window.h"
#include "Camera.h"
#include "Renderer.h"
#include "Mouse.h"
#include "Scenes.h"
#include "Keyboard.h"

LPDIRECTINPUT8	Game::DirectInput	= nullptr;
Scene*			Game::CurrentScene	= nullptr;
double			Game::Frequency		= 0.0;
__int64			Game::LastTime		= 0;
int				Game::FrameCount	= 0;
int				Game::FPS			= 0;
__int64			Game::FrameTimeOld	= 0;
double			Game::FrameTime		= 0.0;

bool Game::Initialize()
{
	if(!Window::Initialize(1000, 800))
	{
		return false;
	}

	Renderer::Initialize(1000, 800);

	if(FAILED(DirectInput8Create(Window::Instance, DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (void**)&DirectInput, nullptr)))
	{
		return false;
	}

	if(!Mouse::Initialize(DirectInput))
	{
		return false;
	}

	if(!Keyboard::Initialize(DirectInput))
	{
		return false;
	}

	CurrentScene = new TestScene();

	Window::Show();
	return true;
}

void Game::Run()
{
	MSG msg = { 0 };
	GetDelta();			// To clear out the delta from begining of time
	while (WM_QUIT != msg.message)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			UpdateTimer();

			Mouse::Poll();
			Keyboard::Poll();
			CurrentScene->Update(GetDelta());
			CurrentScene->Render();
		}
	}
	CurrentScene->Release();
	delete CurrentScene;
}

void Game::Exit()
{
	if(DirectInput) DirectInput->Release();
	Renderer::Dispose();
	Window::Dispose();
	exit(0);
}

//Updates the timer and FPS
void Game::UpdateTimer()
{
	// Getting time between last call
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter(&currentTime);
	double time = double(currentTime.QuadPart - LastTime) / Frequency;

	// Incrementing frame and seeing if a second passed.
	FrameCount++;
	if (time > 1.0f)
	{
		FPS = FrameCount;
		FrameCount = 0;

		wchar_t wFps[256];
		wsprintfW(wFps, L"fps: %d", FPS);
		SetWindowText(Window::Handle, wFps);

		//Starting the timer again
		LARGE_INTEGER frequencyCount;
		QueryPerformanceFrequency(&frequencyCount);

		Frequency = double(frequencyCount.QuadPart);

		QueryPerformanceCounter(&frequencyCount);
		LastTime = frequencyCount.QuadPart;
	}
}

float Game::GetDelta()
{
	LARGE_INTEGER currentTime;
	__int64 tickCount;
	QueryPerformanceCounter(&currentTime);

	tickCount = currentTime.QuadPart - FrameTimeOld;
	FrameTimeOld = currentTime.QuadPart;

	if (tickCount < 0)
		tickCount = 0;

	return float(tickCount / Frequency);
}

Scene* Game::GetCurrentScene()
{
	return CurrentScene;
}

void Game::SetCurrentScene(Scene* scene)
{
	CurrentScene = scene;
}
