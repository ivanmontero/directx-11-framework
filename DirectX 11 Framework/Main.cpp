#include <Windows.h>
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if(!Game::Initialize())
	{
		Game::Exit();
		return -1;
	}
	Game::Run();
	Game::Exit();

	return 0;
}
