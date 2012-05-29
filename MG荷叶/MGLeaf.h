#pragma once

#include "resource.h"
enum GAMESTATE
{
	K_EMPTY,
	K_GAMING,//游戏中
	K_WIN,//游戏胜利，到达终点
	K_FINISH,//游戏失败
	K_COUNT
};
enum GAMESTAGE
{
	STAGE_1,
	STAGE_COUNT
};


// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

HRESULT InitD3D();
HRESULT  InitDirectInput();
void CleanDirectInput();
void CleanD3D(void);
void MainLoop();
void RenderFrame();
void ExitGame();

void CheckMouse();
void GameReset();