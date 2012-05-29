// MG荷叶.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "MGLeaf.h"

#define MAX_LOADSTRING 100

// 设置屏幕尺寸
#define SCREEN_WIDTH  800
#define SCREEN_HEIGHT 553



// 全局变量:
HINSTANCE hInst;								// 当前实例
TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

BOOL   g_bActive = TRUE;	//是否窗口激活

LPDIRECT3D9			 g_pD3d		  = NULL;    // Direct3D接口指针
LPDIRECT3DDEVICE9	 g_pD3ddev	  = NULL;    //D3d设备指针
LPDIRECTINPUT8 g_lpdi       = NULL;	//D3d 输入接口指针
LPDIRECTINPUTDEVICE8 g_pKeyboard  = NULL;	//D3d键盘 输入设备
LPDIRECTINPUTDEVICE8 g_pMouse  = NULL;	//D3d鼠标 输入设备
DWORD MouseX, MouseY;					//鼠标坐标
DWORD MouseButton[4];					//按钮状态
ID3DXSprite* g_pSprite=NULL;

double g_dElpasedTime;//逝去时间
double g_dCurTime;//当前时刻
double g_dLastTime;//上次时刻
double g_dAnimationTimer  = 0.0;
double g_dControllerTimer = 0.0;

GAMESTATE g_gamestate = K_EMPTY;//全局游戏状态
bool g_bChangeState;//是否改变游戏状态
GAMESTAGE g_gamestage = STAGE_1;//游戏关卡
bool g_bChangeStage;


using namespace std;

XM_sprite g_sprtBG;//背景底
XM_sprite g_sprtBorder;//边框
XM_sprite g_sprtCover;//遮罩
XM_sprite g_sprtStart;//起点
XM_sprite g_sprtEnd;//终点
XM_MainRole g_MainRole;//主角

XM_sprite g_btnReset;//重置按钮
XM_sprite g_btnHelp;//Help按钮

vector<XM_sprite*> g_sprtColList;

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 在此放置代码。
	MSG msg;
	HACCEL hAccelTable;

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_MG, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 执行应用程序初始化:
	if (!InitInstance (hInstance, nCmdShow))
		return FALSE;


	HRESULT hr;
	if( FAILED( hr = InitD3D( ) ) )//创建3D设备
		return FALSE;


	if( FAILED( hr = InitDirectInput( ) ) )//初始化输入接口
		return FALSE;
	
	//设置游戏状态为开始
	g_gamestate = K_GAMING;
	g_bChangeState = true;
	g_bChangeStage = true;

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MG));


	// 主消息循环:
	while( TRUE )
	{
		if( PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( 0 == GetMessage(&msg, NULL, 0, 0 ) )
			{
				return (int)msg.wParam;
			}

			if( 0 == TranslateAccelerator( msg.hwnd, hAccelTable, &msg ) )
			{
				TranslateMessage( &msg ); 
				DispatchMessage( &msg );
			}
		}
		else
		{
			if( g_bActive )//如果窗口激活下
			{
				g_dCurTime     = timeGetTime();
				g_dElpasedTime = ((g_dCurTime - g_dLastTime) * 0.001);
				if(g_dElpasedTime>0.016)//控制60fps
				{
					g_dLastTime    = g_dCurTime;
					MainLoop();//进入主循环
					RenderFrame();
				}
			}
			else
			{
				// 等着
				WaitMessage();
			}
		}
	}


	ExitGame();
	return (int) msg.wParam;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor		= LoadCursor(hInstance, LPCTSTR(IDC_CURSOR1));
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_MG);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	return RegisterClassEx(&wcex);
}

//
//   函数: InitInstance(HINSTANCE, int)
//
//   目的: 保存实例句柄并创建主窗口
//
//   注释:
//
//        在此函数中，我们在全局变量中保存实例句柄并
//        创建和显示主程序窗口。
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 将实例句柄存储在全局变量中

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, SCREEN_WIDTH, SCREEN_HEIGHT, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  函数: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  目的: 处理主窗口的消息。
//
//  WM_COMMAND	- 处理应用程序菜单
//  WM_PAINT	- 绘制主窗口
//  WM_DESTROY	- 发送退出消息并返回
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 分析菜单选择:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			ExitGame();
			DestroyWindow(hWnd);
			break;
		case IDM_REPLAY:
			GameReset();
		
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_SIZE:// 检查是否窗口最小化
		if (SIZE_MAXHIDE==wParam || SIZE_MINIMIZED==wParam)
			g_bActive = FALSE;
		else    g_bActive = TRUE;
		break;
	case WM_ACTIVATE:
		if( WA_INACTIVE != wParam && g_pKeyboard )
			g_pKeyboard->Acquire();
		if (WA_INACTIVE != wParam && g_pMouse )
			g_pMouse->Acquire();
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此添加任意绘图代码...
		EndPaint(hWnd, &ps);
		break;
	case WM_KILLFOCUS:
		g_bActive = FALSE;
		break;
	case WM_SETFOCUS:
		g_bActive = TRUE;
		if( WA_INACTIVE != wParam && g_pKeyboard )
			g_pKeyboard->Acquire();
		if (WA_INACTIVE != wParam && g_pMouse )
			g_pMouse->Acquire();
		break;
	case WM_DESTROY:
		ExitGame();
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// “关于”框的消息处理程序。
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

HRESULT InitD3D()
{
	HWND hWnd = FindWindow(szWindowClass,szTitle);

	g_pD3d = Direct3DCreate9(D3D_SDK_VERSION);    // 创建Direct3接口

	D3DPRESENT_PARAMETERS d3dpp;    // create a struct to hold various device information
	ZeroMemory(&d3dpp, sizeof(d3dpp));  

	D3DDISPLAYMODE d3ddm;
	g_pD3d->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm );

	d3dpp.SwapEffect		= D3DSWAPEFFECT_DISCARD;   
	d3dpp.hDeviceWindow		= hWnd;    // set the window to be used by Direct3D
	d3dpp.Windowed			= TRUE;    
	d3dpp.BackBufferFormat	= d3ddm.Format;
	d3dpp.BackBufferCount	=2;//设置交换链长度为2

	d3dpp.EnableAutoDepthStencil = TRUE;//启用深度缓存
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE; // Do NOT sync to vertical retrace
	d3dpp.Flags                  = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

	// 创建3D设备
	g_pD3d->CreateDevice(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,	hWnd,
		D3DCREATE_SOFTWARE_VERTEXPROCESSING,&d3dpp,
		&g_pD3ddev);

	//投影变换
	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH( &matProj, D3DXToRadian( 45.0f ), 
		SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f );
	g_pD3ddev->SetTransform( D3DTS_PROJECTION, &matProj );

	g_pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	g_pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	return S_OK;
}
void CleanD3D(void)
{
	_RELEASE(g_pD3ddev);    // close and release the 3D device
	_RELEASE(g_pD3d);    // close and release Direct3D
}

HRESULT  InitDirectInput()
{
	HWND hWnd = FindWindow(szWindowClass,szTitle);
	HRESULT hr;

	CleanDirectInput();

	// Create a DInput object
	if( FAILED( hr = DirectInput8Create( GetModuleHandle(NULL), DIRECTINPUT_VERSION, 
		IID_IDirectInput8, (VOID**)&g_lpdi, NULL ) ) )
		return hr;
	#pragma region 初始化键盘
	if( FAILED( hr = g_lpdi->CreateDevice( GUID_SysKeyboard, &g_pKeyboard, NULL ) ) )
		return hr;
	//设置数据格式
	if( FAILED( hr = g_pKeyboard->SetDataFormat( &c_dfDIKeyboard ) ) )
		return hr;
	//协作级	
	hr = g_pKeyboard->SetCooperativeLevel( hWnd, DISCL_NOWINKEY | 	DISCL_NONEXCLUSIVE | DISCL_FOREGROUND );
	if( hr == DIERR_UNSUPPORTED )
	{
		CleanDirectInput();
		MessageBox( hWnd, TEXT("SetCooperativeLevel() returned DIERR_UNSUPPORTED.\n")
			TEXT("For security reasons, background exclusive keyboard\n")
			TEXT("access is not allowed."), TEXT("PrototypeX"), MB_OK );
		return S_OK;
	}
	if( FAILED(hr) )
	{	
		MessageBox(hWnd,"Set KeyBoard Cooperative Level","Failed",MB_OK);
		return hr;
	}
	// 获取
	hr = g_pKeyboard->Acquire();
	if( FAILED(hr) )
	{	
		MessageBox(hWnd,"KeyBoard Acquire","Failed",MB_OK);
		return hr;
	}
	#pragma endregion

	#pragma region 初始化鼠标
	if (FAILED( hr = g_lpdi->CreateDevice( GUID_SysMouse, &g_pMouse, NULL ) ) )
		return hr;
	
	if ( FAILED(hr = g_pMouse->SetDataFormat( &c_dfDIMouse ) ) ) 
	{
		MessageBox(hWnd,"Set Mouse Data Format","Failed",MB_OK);
		return hr;
	}
	//协作级	
	if ( FAILED(hr = g_pMouse->SetCooperativeLevel( hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)) ) 
	{
		MessageBox(hWnd,"Set Mouse Cooperative Level","Failed",MB_OK);
		return hr;
	}

	//属性
	DIPROPDWORD property;
	property.diph.dwSize=sizeof(DIPROPDWORD);
	property.diph.dwHeaderSize=sizeof(DIPROPHEADER);
	property.diph.dwObj=0;
	property.diph.dwHow=DIPH_DEVICE;
	property.dwData=16;

	if (FAILED(	hr = g_pMouse->SetProperty(DIPROP_BUFFERSIZE, &property.diph ) ) )
	{
		MessageBox(hWnd,"Set Mouse Property","Failed",MB_OK);
		return hr;
	}
	//获取
	hr = g_pMouse->Acquire();
	if( FAILED(hr) )
	{	
		MessageBox(hWnd,"Mouse Acquire","Failed",MB_OK);
		return hr;
	}

#pragma endregion

	return S_OK;
}

void CleanDirectInput( )
{	
	// Unacquire the device one last time just in case 
	// the app tried to exit while the device is still acquired.
	if( g_pKeyboard ) 
		g_pKeyboard->Unacquire();
	if( g_pMouse )
		g_pMouse->Unacquire();

	// Release any DirectInput objects.
	_RELEASE( g_pKeyboard );
	_RELEASE( g_pMouse);
	_RELEASE( g_lpdi );
}

//主循环
void MainLoop()
{

	switch(g_gamestate)
	{
	case K_EMPTY:
		break;
	case K_WIN:
		MessageBox(NULL,"恭喜你赢了,再玩一次吧","胜利",MB_OK);
		g_gamestate=K_EMPTY;
		break;
	case K_FINISH:
		MessageBox(NULL,"别灰心，再接再厉","失败",MB_OK);
		g_gamestate=K_EMPTY;
		break;
	case K_GAMING:
		if (g_bChangeState)
		{
			g_sprtCover.Clear();
			g_sprtCover.LoadFromFile(g_pD3ddev,"img\\mask2.PNG",1200,840);//遮罩	
			g_sprtCover.SetPosition(90-600,43);
			g_sprtBG.Clear();
			g_sprtBG.LoadFromFile(g_pD3ddev,"img\\bg.PNG",800,530);//背景
			g_sprtBorder.Clear();
			g_sprtBorder.LoadFromFile(g_pD3ddev,"img\\border.PNG",800,530);//边框	
			g_btnReset.Clear();
			g_btnReset.LoadFromFile(g_pD3ddev,"img\\reset.jpg",100,32);
			g_btnReset.SetPosition(10,465);
			g_btnHelp.Clear();
			g_btnHelp.LoadFromFile(g_pD3ddev,"img\\help.jpg",100,35);
			g_btnHelp.SetPosition(200,420);
			g_bChangeState = false;
		}//if (g_bChangeState)
		if (g_bChangeStage)
		{
			for (vector<XM_sprite*>::iterator it=g_sprtColList.begin();it!=g_sprtColList.end();++it)
			{
				_DELETE(*it);
			}
			g_sprtColList.erase(g_sprtColList.begin(),g_sprtColList.end());
			Wxm_IniFile ini;
			int colCount;//柱子总数
			int width;//
			int height;
			char* imgPath;
			switch(g_gamestage)
			{
			case STAGE_1:
				ini.Open("stage1.ini");
				colCount=ini.ReadValueInt("TITLE","count");
				width=ini.ReadValueInt("TITLE","width");
				height=ini.ReadValueInt("TITLE","height");
				imgPath=ini.ReadValue("TITLE","img");
				break;
			default:
				break;
			}
			char str[3];
			int pos_x,pos_y;
			for (int i=0;i<colCount;i++)
			{		
				XM_sprite* colsprite=new XM_sprite();
				colsprite->LoadFromFile(g_pD3ddev,imgPath,width,height);			
				memset(str,0,sizeof(str));
				sprintf(str,   "%d",   i); 
				pos_x=ini.ReadValueInt(str,"x");
				pos_y=ini.ReadValueInt(str,"y");
				colsprite->SetPosition(OFF_LEFT+pos_x*width,OFF_TOP+pos_y*(height));
				g_sprtColList.push_back(colsprite);
			}
			_FREE(imgPath);

			imgPath=ini.ReadValue("START","img");
			g_sprtStart.Clear();
			g_sprtStart.LoadFromFile(g_pD3ddev,imgPath,width,height);
			_FREE(imgPath);
			pos_x=ini.ReadValueInt("START","x");
			pos_y=ini.ReadValueInt("START","y");
			g_sprtStart.SetPosition(OFF_LEFT+pos_x*width,OFF_TOP+pos_y*(height));

			imgPath=ini.ReadValue("END","img");
			g_sprtEnd.Clear();
			g_sprtEnd.LoadFromFile(g_pD3ddev,imgPath,width,height);
			_FREE(imgPath);
			pos_x=ini.ReadValueInt("END","x");
			pos_y=ini.ReadValueInt("END","y");
			g_sprtEnd.SetPosition(OFF_LEFT+pos_x*width,OFF_TOP+pos_y*(height));

			imgPath=ini.ReadValue("ROLE","img");
			g_MainRole.Clear();
			g_MainRole.LoadFromFile(g_pD3ddev,imgPath,width,height);
			_FREE(imgPath);
			pos_x=ini.ReadValueInt("ROLE","x");
			pos_y=ini.ReadValueInt("ROLE","y");
			g_MainRole.SetPosition(OFF_LEFT+pos_x*width,OFF_TOP+pos_y*(height));
			g_MainRole.ClearLeaf();
			g_MainRole.CreateLeaf(g_pD3ddev,"img\\leaf.png",width,height);

			g_bChangeStage =false;
		}//		if (g_bChangeStage)



		//碰撞检测
		if (g_MainRole.Check(g_sprtEnd.getX(),g_sprtEnd.getY(),g_dElpasedTime))//检测到达终点
		{
			g_gamestate = K_WIN;
		}
		for (vector<XM_sprite*>::iterator it=g_sprtColList.begin();it!=g_sprtColList.end();++it)
		{
			if(g_MainRole.Check((*it)->getX(),(*it)->getY(),g_dElpasedTime))
				break;
		}
		if (g_MainRole.CheckFail())
		{
			g_gamestate= K_FINISH;
		}
		CheckMouse();
		g_MainRole.Update(g_dElpasedTime);
		g_sprtCover.SetPosition(-600+g_MainRole.getX()+14,g_MainRole.getY()-PIC_HEIGHT*14+17);
		break;

	}
	CheckMouse();
}
//渲染
void RenderFrame()
{
	g_pD3ddev->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	g_pD3ddev->BeginScene();    // begins the 3D scene
	
	g_sprtBG.Draw();
	
	

	for (vector<XM_sprite*>::iterator it=g_sprtColList.begin();it!=g_sprtColList.end();++it)
	{
		(*it)->Draw();
	}
	g_sprtStart.Draw();
	g_sprtEnd.Draw();
	g_MainRole.Draw();//主角和荷叶
	
	g_sprtCover.Draw();
	g_sprtBorder.Draw();
	g_btnReset.Draw();
	//g_btnHelp.Draw();

	g_pD3ddev->EndScene();    // ends the 3D scene
	g_pD3ddev->Present(NULL, NULL, NULL, NULL);   // displays the created frame on the screen
}

void ExitGame()
{
	for (vector<XM_sprite*>::iterator it=g_sprtColList.begin();it!=g_sprtColList.end();++it)
	{
		_DELETE(*it);
	}
	g_sprtColList.erase(g_sprtColList.begin(),g_sprtColList.end());
	CleanDirectInput();
	CleanD3D();

}

void CheckMouse()
{
	DIDEVICEOBJECTDATA	data;
	DWORD				elements=1;		//一个元素
	HRESULT hr = g_pMouse->GetDeviceData(sizeof(data), &data, &elements, 0);
	if FAILED(hr)
	{
		if(hr == DIERR_INPUTLOST)
		{
			hr = g_pMouse->Acquire();
		}
	}
	else
	{
		if(elements == 1)
		{
			switch(data.dwOfs)
			{
			case DIMOFS_BUTTON0: 
				if( data.dwData & 0x80 )//高位为1：按下
				{
					HWND hWnd = FindWindow(szWindowClass,szTitle);
					POINT	pt;
					GetCursorPos(&pt);
					//ScreenToClient(hWnd, &pt);
					if (g_gamestate==K_GAMING)
					{						
						RECT rcEast = g_MainRole.getLeafRect(DIR_EAST);
						RECT rcWest = g_MainRole.getLeafRect(DIR_WEST);
						RECT rcNorth= g_MainRole.getLeafRect(DIR_NORTH);
						RECT rcSouth= g_MainRole.getLeafRect(DIR_SOUTH);
						MapWindowPoints(hWnd, NULL, (LPPOINT)&rcEast, 2);
						MapWindowPoints(hWnd, NULL, (LPPOINT)&rcWest, 2);
						MapWindowPoints(hWnd, NULL, (LPPOINT)&rcNorth, 2);
						MapWindowPoints(hWnd, NULL, (LPPOINT)&rcSouth, 2);
						if (PtInRect(&rcEast, pt) && WindowFromPoint(pt)==hWnd)//向东
						{
							g_MainRole.SetActive(DIR_EAST);
							return;
						}
						if (PtInRect(&rcWest, pt) && WindowFromPoint(pt)==hWnd)//向西
						{
							g_MainRole.SetActive(DIR_WEST);
							return;
						}
						if(PtInRect(&rcSouth, pt) && WindowFromPoint(pt)==hWnd)//向南
						{
							g_MainRole.SetActive(DIR_SOUTH);
							return;
						}
						if(PtInRect(&rcNorth, pt) && WindowFromPoint(pt)==hWnd)//向北
						{
							g_MainRole.SetActive(DIR_NORTH);
							return;
						}
					}
					RECT rcBtnReset=g_btnReset.getRect();
					MapWindowPoints(hWnd, NULL, (LPPOINT)&rcBtnReset, 2);
					if (PtInRect(&rcBtnReset, pt) && WindowFromPoint(pt)==hWnd)//重置游戏
					{
						GameReset();
						return;
					}
				}				
				break;
			}		
		}
		else 
			if(elements==0) {}
	}
}

void GameReset()
{
	g_gamestage=STAGE_1;
	g_bChangeStage=TRUE;
	g_gamestate=K_GAMING;
}