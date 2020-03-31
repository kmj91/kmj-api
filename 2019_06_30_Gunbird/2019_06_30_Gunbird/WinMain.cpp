#include "stdafx.h"
#include "resource.h"

#include <mmSystem.h>		// timeBeginPeriod
#include <windowsx.h>		// GetWindowStyle


#pragma comment( lib, "Winmm.lib")

// 전역 변수:
HINSTANCE hInst;                                // 현재 인스턴스입니다.
HWND g_hWnd;
CScreenDib g_cScreenDib(df_WIN_RIGHT, df_WIN_BOTTOM, 32);		// 전역으로 스크린DIB 생성
CSpriteDib g_cSprite;			// 스프라이트
SceneManager g_sceneManager;		// 씬을 관리할 씬 매니저

// extern 전역 변수
int g_time;
int g_fps;
int g_oldTime = 0;
int g_skipFrame = 0;
int g_FramePerSecond = 0;
int g_Frame;

BYTE *g_bypDest;
int g_iDestWidth;
int g_iDestHeight;
int g_iDestPitch;

int g_iScore;			// 점수
int g_iMouseX;			// 마우스 좌표 X
int g_iMouseY;			// 마우스 좌표 Y

bool g_bActiveApp;					// 윈도우 활성화 비활성화 상태 체크
bool g_bNoBullet = false;			// 폭탄 사용중이라 총 쏘면 안됨

list<Object *> * g_ObjectList;		// 오브젝트 리스트
Object * g_StageBackground;			// 현제 배경 오브젝트
ObjectPlayer *g_Player = NULL;

CObjectPoolTLS<ObjectBullet> g_ObjectBulletPool(90, 100, false);		// 오브젝트 풀

GameState g_GameState = EMPT;		// 현제 게임 진행 상태
int g_iCoin = 0;					// 코인
int g_iContinueCnt = 9;				// 컨티뉴 카운트
bool g_bGameOver = false;			// 게임 오버


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
bool FrameSkip();							//프레임 스킵

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: 여기에 코드를 입력합니다.
	timeBeginPeriod(1);


	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20190630GUNBIRD));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY20190630GUNBIRD);
	wcex.lpszClassName = df_WIN_CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);

	// 응용 프로그램 초기화를 수행합니다:
	hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(df_WIN_CLASS_NAME, df_WIN_CLASS_NAME, WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, df_WIN_RIGHT, df_WIN_BOTTOM, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);


	SetFocus(g_hWnd);

	RECT WindowRect;
	WindowRect.top = 0;
	WindowRect.left = 0;
	WindowRect.right = df_WIN_RIGHT;
	WindowRect.bottom = df_WIN_BOTTOM;

	AdjustWindowRectEx(&WindowRect, GetWindowStyle(g_hWnd), GetMenu(g_hWnd) != NULL, GetWindowExStyle(g_hWnd));

	//화면의 크기를 얻어서 중앙으로
	int iX = (GetSystemMetrics(SM_CXSCREEN) / 2) - (df_WIN_RIGHT / 2);
	int iY = 0;//(GetSystemMetrics(SM_CYSCREEN) / 2) - (df_WIN_BOTTOM / 2);

	//윈도우 크기 변경
	MoveWindow(g_hWnd, iX, iY, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top, TRUE);

	//출력 버퍼 포인터 및 정보 얻음
	g_bypDest = g_cScreenDib.GetDibBuffer();
	g_iDestWidth = g_cScreenDib.GetWidth();
	g_iDestHeight = g_cScreenDib.GetHeight();
	g_iDestPitch = g_cScreenDib.GetPitch();

	// 랜덤 시드 값
	srand(0);
	// 초기 시간 값
	g_fps = GetTickCount();
	g_oldTime = g_fps;

	MSG msg;

	//게임을 위한 메시지 루프
	while (1) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			if (FrameSkip()) {
				g_sceneManager.SceneChangeUpdate();
				g_sceneManager.SceneUpdate();
				//Sleep(10);
			}
		}
	}

	timeEndPeriod(1);
	return (int)msg.wParam;
}





//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  용도: 주 창의 메시지를 처리합니다.
//
//  WM_COMMAND  - 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT    - 주 창을 그립니다.
//  WM_DESTROY  - 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_ACTIVATEAPP:
		g_bActiveApp = (bool)wParam;
		break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_MOUSEMOVE:
		g_iMouseX = LOWORD(lParam);
		g_iMouseY = HIWORD(lParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

bool FrameSkip()
{
	int tempTime;

	//스킵
	g_time = GetTickCount();
	tempTime = g_time - g_oldTime;
	if (g_time - g_fps >= 1000) {
		g_Frame = g_FramePerSecond;
		g_FramePerSecond = 0;
		g_fps = g_time;

		if (g_bGameOver) {
			--g_iContinueCnt;
		}
	}
	

	if (tempTime > FRAME_DELEY) {
		g_skipFrame = g_skipFrame + tempTime;
		g_skipFrame = g_skipFrame - FRAME_DELEY;
	}
	else if (g_skipFrame > FRAME_DELEY) {
		g_skipFrame = g_skipFrame + tempTime;
		g_skipFrame = g_skipFrame - FRAME_DELEY;
	}
	else {
		tempTime = FRAME_DELEY - tempTime;
		Sleep(tempTime);
	}

	//if (g_FramePerSecond >= FPS) {
	//	return false;
	//}

	++g_FramePerSecond;

	g_oldTime = GetTickCount();

	return true;
}