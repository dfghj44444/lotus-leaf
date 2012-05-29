// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
// Windows 头文件:
#include <windows.h>

// C 运行时头文件
#include <assert.h>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <malloc.h>
#include <io.h>
#include <fcntl.h>
#include  <mmsystem.h>

// TODO: 在此处引用程序需要的其他头文件


//STL头文件
#include <list>
#include <vector>

#include  <mmsystem.h>

// DX头文件
#include  <d3d9.h>
#include  <d3dx9.h>
#include  <dinput.h>
//#include  <dsound.h>

//DX库文件
#pragma comment (lib,"winmm.lib");
#pragma comment (lib, "d3d9.lib");
#pragma comment (lib, "d3dx9.lib");
#pragma comment (lib, "dxguid.lib");
#pragma comment (lib, "dxerr.lib");
#pragma comment (lib, "dinput8.lib");
//#pragma comment (lib, "dsound.lib");

//设置棋盘起始点
#define OFF_LEFT 93
#define OFF_TOP 43
#define PIC_WIDTH 30
#define PIC_HEIGHT 30

//其他工程的头文件
#include "../PublicLib/PublicLib.h"


//本工程的头文件
#include "XM_ini.h"
#include "XM_sprite.h"
#include "XM_MainRole.h"

