#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class Scene;

class Game
{
private:
	static LPDIRECTINPUT8 DirectInput;
	static Scene* CurrentScene;
	
	static double Frequency;
	static __int64 LastTime;
	static int FrameCount;
	static int FPS;
	static __int64 FrameTimeOld;
	static double FrameTime;

public:
	static bool Initialize();
	static void Run();
	static void Exit();

	static void UpdateTimer();
	static float GetDelta();

	static Scene* GetCurrentScene();
	static void SetCurrentScene(Scene*);
};
