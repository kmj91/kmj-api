#include "stdafx.h"
#include "resource.h"
#include "CScreenDib.h"
#include "ObjectStageMap.h"
#include "MenuTileUnit.h"
#include "UIElement.h"			// ESC 누를시 변경사항 있음
#include "MenuTrigger.h"
#include "UnitLantern.h"

#include <time.h>
#include <mmSystem.h>						// timeBeginPeriod
#pragma comment( lib, "Winmm.lib")			// timeBeginPeriod


#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;


// 전역 변수:
HINSTANCE g_hInst;							// 현재 인스턴스입니다.
HWND g_hWnd;								// 윈 메인 핸들
HWND g_hWndChangeSize;						// 다이얼로그 핸들
RECT g_DCRect;								// 윈도우 크기
CScreenDib *g_ScreenDib;					// 전역으로 스크린DIB 생성
BITMAPINFO *g_stpDibInfo;					// 스크린 DIB에 대한 정보가 들어 있는 구조체

ObjectStageMap *g_ObjectStageMap;			// 오브젝트 스테이지 맵
MenuTileUnit *g_ObjectMenu;					// 오브젝트 메뉴
MenuTrigger *g_ObjectTriggerMenu;		// 트리거 메뉴

int g_iTileWidth;							// 타일의 길이 (가장자리 1칸은 뺌)
int g_iTileHeight;							// 타일의 길이 (가장자리 1칸은 뺌)

DWORD g_dwTime;
DWORD g_dwOldTime = 0;
DWORD g_dwFps;
DWORD g_dwSkipFrame = 0;
int g_iFramePerSecond = 0;
int g_iFrameDeley;
int g_iFrameNokori;							// 나머지 부분 값
int g_iCountNokori;							// 1000ms 동안 남은 나머지 부분
int g_iFramePoint;							// 소수점 보정 값
int g_iCountPoint;							// 10000 넘어가면 보정

CList<Object *> *g_SectorList;				// 섹터 리스트
int g_iSectorWidth;							// 섹터 가로 길이
int g_iSectorHeight;						// 섹터 세로 길이
int g_iSectorArrX;							// 섹터 배열 가로 길이
int g_iSectorArrY;							// 섹터 배열 세로 길이
int g_iSectorArrSize;						// 섹터 배열 크기


// extern 변수
CSpriteDib *g_cSprite;					// 스프라이트
CList<Object *> *g_ObjectList;			// WinMain 오브젝트 리스트
CList<ObjectLight *> *g_ObjectLightList;// 조명 리스트 (ObjectLightDib 에서 사용)
ObjectLightDib *g_ObjectLightDib;		// 조명 처리 비트맵
BYTE *g_bypDest;						// 스크린 버퍼
int g_iDestWidth;						// 스크린 가로 길이
int g_iDestHeight;						// 스크린 세로 길이
int g_iDestPitch;						// 스크린 피치 값

int g_iMapMouseX;						// 맵 기준 마우스 좌표 X
int g_iMapMouseY;						// 맵 기준 마우스 좌표 Y
int g_iUIMouseX;						// 화면(윈도우 창) 기준 마우스 좌표 X
int g_iUIMouseY;						// 화면(윈도우 창) 기준 마우스 좌표 Y
RECT g_CameraRect;						// 카메라 좌표
RECT g_WorldRect;						// 화면 구역 크기
int g_iMapWidth;						// 맵 가로 길이
int g_iMapHeight;						// 맵 세로 길이

ObjectUI *g_SelectedObjectUI = NULL;					// 선택된 오브젝트 UI
ObjectUnit *g_SelectedObjectUnit = NULL;				// 선택된 유닛
int g_iMouseClickObjectType = df_MOUSE_CLICK_EMPT;		// 마우스 클릭했을 때 선택된 오브젝트 타입 (드래그에서 사용)
int g_iMouseState = df_MOUSE_STATE_EMPT;				// 마우스 상태
int g_iSelectedType = df_TILE_TYPE_EMPT;				// 메뉴에서 선택한 타일이나 오브젝트 종류
int g_iSelectedState = df_TILE_STATE_EMPT;				// 메뉴에서 선택한 타일 상태
int g_iSelectedSpriteIncreaseVlaue = 1;					// 메뉴에서 선택한 스프라이트 증가 값
ObjectMousePointer * g_MousePointer;					// 마우스 지시자 (마우스 상태에따라 변함)

int g_iLocationNumber = 0;								// 로케이션 고유 번호

FT_Library g_library;									// 프리타입 라이브러리
HDC g_hdc;												// hdc;

wchar_t g_szTriggerConditions[1][50] = {
	L"무조건"
};
wchar_t g_szTriggerActions[3][50] = {
	L"문 닫기",
	L"문 열기",
	L"유닛 생성"
};


// 이 코드 모듈에 포함된 함수의 선언을 전달합니다:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK    ChangeSizeProc(HWND, UINT, WPARAM, LPARAM);			// 크기 변경 다이얼로그
void FrameSkip();				//프레임 스킵
void Update();					// 업데이트
void LoadSprite();				// 스프라이트 로드
void KeyProcess();
void InsertionSort();
void SaveProcess();				// 세이브 처리
void LoadProcess();				// 불러오기 처리
void UTF8toUTF16(const char *szText, WCHAR *szBuff, int iBuffLen);



int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 여기에 코드를 입력합니다.
	timeBeginPeriod(1);

	// 프리타입 라이브러리 초기화
	if (FT_Init_FreeType(&g_library)) {
		MessageBox(NULL, TEXT("FreeType 라이브러리 초기화 실패"), TEXT("에러"), MB_OK);
		return -1;
	}


	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY20190826ENTERTHEGUNGEONMAPTOOL));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MY20190826ENTERTHEGUNGEONMAPTOOL);
	wcex.lpszClassName = df_WIN_PARENT_CLASS_NAME;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	RegisterClassExW(&wcex);


    // 응용 프로그램 초기화를 수행합니다:
	g_hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

	g_hWnd = CreateWindowW(df_WIN_PARENT_CLASS_NAME, df_WIN_PARENT_TITLE_NAME, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!g_hWnd)
	{
		return FALSE;
	}

	// 윈도우 최대 활성화
	nCmdShow = SW_SHOWMAXIMIZED;
	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// 사이즈 변경 다이얼로그
	g_hWndChangeSize = CreateDialog(hInstance, MAKEINTRESOURCE(IDD_CHANGE_SIZE), NULL, ChangeSizeProc);


	// 윈도우 크기 얻기
	GetClientRect(g_hWnd, &g_DCRect);

	// 윈도우 스크린 버퍼 생성
	g_ScreenDib = new CScreenDib(g_DCRect.right, g_DCRect.bottom, 32);
	// 스프라이트 버퍼 생성
	g_cSprite = new CSpriteDib(500, 0xff00ff);
	// 오브젝트 리스트 생성
	g_ObjectList = new CList<Object *>;
	// 조명 리스트 생성
	g_ObjectLightList = new CList<ObjectLight *>;

	// 섹터 리스트 생성
	g_iSectorWidth = g_DCRect.right / 2;
	g_iSectorHeight = g_DCRect.bottom / 2;
	g_iSectorArrX = g_iMapWidth / g_iSectorWidth;
	if (g_iMapWidth % g_iSectorWidth) {
		++g_iSectorArrX;
	}
	g_iSectorArrY = g_iMapHeight / g_iSectorHeight;
	if (g_iMapHeight % g_iSectorHeight) {
		++g_iSectorArrY;
	}
	g_iSectorArrSize = g_iSectorArrX * g_iSectorArrY;
	g_SectorList = new CList<Object *>[g_iSectorArrSize];

	// 맵 크기 값
	g_iMapWidth = df_DEFAULT_TILE_X * df_DEFAULT_TILE_SIZE;			// 맵 가로 길이
	g_iMapHeight = df_DEFAULT_TILE_Y * df_DEFAULT_TILE_SIZE;		// 맵 세로 길이
	g_iTileWidth = g_iMapWidth - df_DEFAULT_TILE_SIZE;				// 타일의 길이 (가장자리 1칸은 뺌)
	g_iTileHeight = g_iMapHeight - df_DEFAULT_TILE_SIZE;			// 타일의 길이 (가장자리 1칸은 뺌)


	// 스프라이트 로드
	LoadSprite();

	// rand 시드
	srand((unsigned)time(NULL));

	//출력 버퍼 포인터 및 정보 얻음
	g_bypDest = g_ScreenDib->GetDibBuffer();
	g_iDestWidth = g_ScreenDib->GetWidth();
	g_iDestHeight = g_ScreenDib->GetHeight();
	g_iDestPitch = g_ScreenDib->GetPitch();
	g_stpDibInfo = g_ScreenDib->GetDibInfo();

	// 초기 시간 값
	g_dwFps = GetTickCount();
	g_dwOldTime = g_dwFps;

	g_iFrameDeley = 1000 / FPS;
	g_iFrameNokori = 1000 % FPS;
	g_iCountNokori = g_iFrameNokori;

	// 소수점 보정
	g_iFramePoint = (1000 * 10000) / FPS;
	g_iFramePoint = g_iFramePoint - (g_iFrameDeley * 10000);
	g_iCountPoint = 0;


	int iTemp;

	//--------------------------------------
	// 카메라, 화면 구역
	// 맵 크기가 스크린보다 작을 경우
	// 초기 값이 다르다
	//--------------------------------------
	if (g_iMapWidth < g_iDestWidth) {
		iTemp = ((g_iDestWidth - g_iMapWidth) / 2);
		g_CameraRect.left = -iTemp;
		g_CameraRect.right = g_CameraRect.left + g_iDestWidth;

		// 화면 크기 기준
		g_WorldRect.left = -(iTemp + 500);
		g_WorldRect.right = g_iMapWidth + iTemp + 500;

		//g_CameraRect.left = 0;
		//g_CameraRect.right = g_CameraRect.left + g_iDestWidth;
		//g_WorldRect.left = 0;
		//g_WorldRect.right = g_iMapWidth;
	}
	else {
		g_CameraRect.left = 0;
		g_CameraRect.right = g_iDestWidth;

		// 맵 크기 기준
		g_WorldRect.left = -500;
		g_WorldRect.right = g_iMapWidth + 500;

		//g_WorldRect.left = 0;
		//g_WorldRect.right = g_iMapWidth;
	}


	if (g_iMapHeight < g_iDestHeight) {
		iTemp = ((g_iDestHeight - g_iMapHeight) / 2);
		g_CameraRect.top = -iTemp;
		g_CameraRect.bottom = g_CameraRect.top + g_iDestHeight;

		// 화면 크기 기준
		g_WorldRect.top = -(iTemp + 500);
		g_WorldRect.bottom = g_iMapHeight + iTemp + 500;

		//g_CameraRect.top = 0;
		//g_CameraRect.bottom = g_CameraRect.top + g_iDestHeight;
		//g_WorldRect.top = 0;
		//g_WorldRect.bottom = g_iMapHeight;
	}
	else {
		g_CameraRect.top = 0;
		g_CameraRect.bottom = g_iDestHeight;

		// 맵 크기 기준
		g_WorldRect.top = -500;
		g_WorldRect.bottom = g_iMapHeight + 500;

		//g_WorldRect.top = 0;
		//g_WorldRect.bottom = g_iMapHeight;
	}


	// 기본 생성 오브젝트들
	Object * newObject;

	// 조명처리 비트맵
	g_ObjectLightDib = new ObjectLightDib(g_iMapWidth, g_iMapHeight, 32, 0xff00ff);
	g_ObjectList->push_back(g_ObjectLightDib);

	// 배경
	g_ObjectStageMap = new ObjectStageMap(g_iMapWidth, g_iMapHeight);
	g_ObjectList->push_back(g_ObjectStageMap);

	// 타일 메뉴
	g_ObjectMenu = new MenuTileUnit(g_iDestWidth - 400, 50, 350, 300);
	g_ObjectList->push_back(g_ObjectMenu);

	// 마우스
	g_MousePointer = new ObjectMousePointer(0, 0);
	g_ObjectList->push_back(g_MousePointer);

	// 트리거 메뉴
	g_ObjectTriggerMenu = new MenuTrigger(g_iDestWidth / 2 - 500 / 2, 50, 500, 400);
	g_ObjectList->push_back(g_ObjectTriggerMenu);

	// 전역으로 hdc 생성
	g_hdc = GetDC(g_hWnd);

    MSG msg;

    // 기본 메시지 루프입니다:
	while (1) {
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			FrameSkip();
			Update();
			//Sleep(1);
		}
	}
	
	// hdc 정리
	ReleaseDC(g_hWnd, g_hdc);

	CList<Object*>::iterator iter;
	CList<Object*>::iterator iter_end;
	Object * ObjectTemp;

	//--------------------------------------
	// 메모리 정리
	//--------------------------------------
	iter = g_ObjectList->begin();
	iter_end = g_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = g_ObjectList->erase(iter);
		delete ObjectTemp;
	}

	delete g_ObjectList;
	delete g_cSprite;
	delete g_ScreenDib;


	// 프리타입 라이브러리 정리
	FT_Done_FreeType(g_library);

	timeEndPeriod(1);

    return (int) msg.wParam;
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
	CList<Object*>::iterator iter;
	CList<Object*>::iterator iter_end;
	Object * ObjectTemp;
	int iMouseX;		// 이전 마우스 X 좌표
	int iMouseY;		// 이전 마우스 Y 좌표
	int iTileX;			// 타일 좌표 X
	int iTileY;			// 타일 좌표 Y
	wchar_t szWidth[255];
	wchar_t szHeight[255];


    switch (message)
    {
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: 여기에 hdc를 사용하는 그리기 코드를 추가합니다...
            EndPaint(hWnd, &ps);
        }
        break;

	case WM_MOUSEMOVE:
		//--------------------------------------
		// 마우스 이동
		//--------------------------------------
		iMouseX = g_iMapMouseX;
		iMouseY = g_iMapMouseY;

		// 화면 기준 마우스 좌표
		g_iUIMouseX = LOWORD(lParam);
		g_iUIMouseY = HIWORD(lParam);

		// 카메라 위치 + 화면 기준 좌표 = 맵 기준 위치
		g_iMapMouseX = g_CameraRect.left + g_iUIMouseX;
		g_iMapMouseY = g_CameraRect.top + g_iUIMouseY;

		// 마우스가 UI위에 있냐
		if (g_ObjectMenu->CheckOnMouse() || g_ObjectTriggerMenu->CheckOnMouse()) {
			g_MousePointer->m_bOnUIMouse = true;
		}
		else {
			g_MousePointer->m_bOnUIMouse = false;
		}

		// 마우스 클릭 중 (드래그)
		if (MK_LBUTTON & wParam) {
			switch (g_iMouseClickObjectType) {
			case df_MOUSE_CLICK_MENU_TITLE_BAR:
				//---------------------------------------------------
				// 타이틀 바
				//---------------------------------------------------
				iMouseX = iMouseX - g_iMapMouseX;
				iMouseY = iMouseY - g_iMapMouseY;

				g_SelectedObjectUI->Move(iMouseX, iMouseY);
				break;
			case df_MOUSE_CLICK_MENU_SCROLL_BAR:
				//---------------------------------------------------
				//  스크롤 바
				//---------------------------------------------------
				iMouseY = iMouseY - g_iMapMouseY;

				//g_MenuElementListScrollBar->ScrollMove(iMouseY);
				((UIScrollBar *)g_SelectedObjectUI)->ScrollMove(iMouseY);
				break;
			case df_MOUSE_CLICK_UNIT_LOCATION:
				//---------------------------------------------------
				//  로케이션
				//---------------------------------------------------
				iMouseX = (iMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);
				iMouseY = (iMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);

				// 강제 형변환
				// ObjectUnit -> UnitLocation
				((UnitLocation *)g_SelectedObjectUnit)->Move(iMouseX, iMouseY);

				break;
			case df_MOUSE_CLICK_UNIT_LOCATION_LEFT:
				//---------------------------------------------------
				//  로케이션 크기 조절
				//---------------------------------------------------
				iMouseX = (iMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);

				// 강제 형변환
				// ObjectUnit -> UnitLocation
				((UnitLocation *)g_SelectedObjectUnit)->ReSizeLeft(iMouseX);
				break;
			case df_MOUSE_CLICK_UNIT_LOCATION_TOP:
				//---------------------------------------------------
				//  로케이션 크기 조절
				//---------------------------------------------------
				iMouseY = (iMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);

				// 강제 형변환
				// ObjectUnit -> UnitLocation
				((UnitLocation *)g_SelectedObjectUnit)->ReSizeTop(iMouseY);
				break;
			case df_MOUSE_CLICK_UNIT_LOCATION_RIGHT:
				//---------------------------------------------------
				//  로케이션 크기 조절
				//---------------------------------------------------
				iMouseX = (iMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);

				// 강제 형변환
				// ObjectUnit -> UnitLocation
				((UnitLocation *)g_SelectedObjectUnit)->ReSizeRight(iMouseX);
				break;
			case df_MOUSE_CLICK_UNIT_LOCATION_BOTTOM:
				//---------------------------------------------------
				//  로케이션 크기 조절
				//---------------------------------------------------
				iMouseY = (iMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE) - (g_iMapMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE);

				// 강제 형변환
				// ObjectUnit -> UnitLocation
				((UnitLocation *)g_SelectedObjectUnit)->ReSizeBottom(iMouseY);
				break;
			default :
				//-------------------------------------------
				// 마우스 상태에 따라 별개의 처리
				// 타일이나 물체를 만들때 연속해서 만들기
				// 로케이션의 경우 크기 변경이 되야 한다
				//-------------------------------------------

				switch (g_iMouseState)
				{
				case df_MOUSE_STATE_TILE_MAKE:
				case df_MOUSE_STATE_OBJECT_MAKE:
					//--------------------------------------
					// 타일이나 물체를 만들기
					// 드래그시 연속적으로 생성 가능
					// 조작감이 좋아짐
					//--------------------------------------
					//--------------------------------------
					// 마우스 클릭 체크
					//--------------------------------------
					iter = g_ObjectList->rbegin();
					iter_end = g_ObjectList->head();
					while (iter != iter_end) {
						ObjectTemp = *iter;
						if (ObjectTemp->Click()) {
							break;
						}
						--iter;
					}
					break;
				case df_MOUSE_STATE_LOCATION_DRAG:
					//---------------------------------------------------
					// 로케이션 생성 후 드래그 중
					// 로케이션 크기를 변경한다
					// 변경시 좌표를 아마 다시 잡아줘야 함
					// 오브젝트 영역도 뒤집어졌을경우 예외처리
					// 최종적으로 마우스를 땠을때 로케이션을 생성 한다.
					//---------------------------------------------------

					// 강제 형변환
					// ObjectUnit -> UnitLocation
					((UnitLocation *)g_SelectedObjectUnit)->Drag();
					break;
				default:
					break;
				}
				
				break;
			}
		}

		

		

		switch (g_iMouseState) {
		case df_MOUSE_STATE_OBJECT_MAKE:
			iTileX = g_iMapMouseX / df_DEFAULT_TILE_SIZE;
			iTileY = g_iMapMouseY / df_DEFAULT_TILE_SIZE;
			if (g_ObjectStageMap->CheckObjectMake(iTileX, iTileY)) {
				// 만들 수 있음
				g_MousePointer->m_bRedOption = false;
				g_MousePointer->m_bBlueOption = true;
			}
			else {
				// 못 만듬
				g_MousePointer->m_bRedOption = true;
				g_MousePointer->m_bBlueOption = false;
			}
			break;
		case df_MOUSE_STATE_TILE_MAKE:
			g_MousePointer->m_bRedOption = false;
			g_MousePointer->m_bBlueOption = false;
			break;
		default:

			break;
		}


		break;

	case WM_LBUTTONDOWN:
		//--------------------------------------
		// 마우스 왼쪽 버튼 DOWN
		//--------------------------------------

		// 화면 기준 마우스 좌표
		g_iUIMouseX = LOWORD(lParam);
		g_iUIMouseY = HIWORD(lParam);

		// 카메라 위치 + 화면 기준 좌표 = 맵 기준 위치
		g_iMapMouseX = g_CameraRect.left + g_iUIMouseX;
		g_iMapMouseY = g_CameraRect.top + g_iUIMouseY;

		//--------------------------------------
		// 마우스 클릭 체크
		//--------------------------------------
		iter = g_ObjectList->rbegin();
		iter_end = g_ObjectList->head();
		while (iter != iter_end) {
			ObjectTemp = *iter;
			if (ObjectTemp->Click()) {
				break;
			}
			--iter;
		}

		break;

	case WM_LBUTTONUP:
		//--------------------------------------
		// 마우스 왼쪽 버튼 UP
		//--------------------------------------

		// 화면 기준 마우스 좌표
		g_iUIMouseX = LOWORD(lParam);
		g_iUIMouseY = HIWORD(lParam);

		// 카메라 위치 + 화면 기준 좌표 = 맵 기준 위치
		g_iMapMouseX = g_CameraRect.left + g_iUIMouseX;
		g_iMapMouseY = g_CameraRect.top + g_iUIMouseY;

		// 마우스 클릭 오브젝트 해제
		g_iMouseClickObjectType = df_MOUSE_CLICK_EMPT;

		// 마우스 상태
		switch (g_iMouseState) {
		case df_MOUSE_STATE_LOCATION_DRAG :
			//--------------------------------------
			// 드래그 해제
			//--------------------------------------
			g_iMouseState = df_MOUSE_STATE_LOCATION_MAKE;

			// 로케이션 좌표 정렬
			// 강제 형변환
			// ObjectUnit -> UnitLocation
			((UnitLocation *)g_SelectedObjectUnit)->m_iX = g_SelectedObjectUnit->m_iLeft;
			((UnitLocation *)g_SelectedObjectUnit)->m_iY = g_SelectedObjectUnit->m_iTop;
			break;
		default:

			break;
		}

		break;
	case WM_COMMAND:
		//--------------------------------------
		// 커맨드 입력 처리
		//--------------------------------------
		switch (LOWORD(wParam)) {
		case ID_SAVE:
			SaveProcess();
			break;
		case ID_LOAD:
			LoadProcess();
			break;
		case ID_CHANGE_SIZE:
			wsprintf(szWidth, L"%d", g_iMapWidth / df_DEFAULT_TILE_SIZE);
			wsprintf(szHeight, L"%d", g_iMapHeight / df_DEFAULT_TILE_SIZE);

			// 텍스트 필드 채우기
			SetWindowText(GetDlgItem(g_hWndChangeSize, IDC_WIDTH), szWidth);
			SetWindowText(GetDlgItem(g_hWndChangeSize, IDC_HEIGHT), szHeight);

			ShowWindow(g_hWndChangeSize, SW_SHOWDEFAULT);
			break;
		}
		break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



LRESULT ChangeSizeProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iX;
	int iY;
	int iTemp;
	wchar_t szWidth[255];
	wchar_t szHeight[255];

	switch (message) {
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			GetWindowText(GetDlgItem(g_hWndChangeSize, IDC_WIDTH), szWidth, 255);
			GetWindowText(GetDlgItem(g_hWndChangeSize, IDC_HEIGHT), szHeight, 255);

			iX = _wtoi(szWidth);
			iY = _wtoi(szHeight);

			if (iX > 100 || iY > 100) {
				// 에러 처리
				MessageBox(NULL, TEXT("값이 너무 큽니다"), TEXT("에러"), MB_OK);
				return FALSE;
			}
			else if (iX < 0 || iY < 0) {
				MessageBox(NULL, TEXT("값이 너무 작습니다"), TEXT("에러"), MB_OK);
				return FALSE;
			}

			g_iMapWidth = iX * df_DEFAULT_TILE_SIZE;
			g_iMapHeight = iY * df_DEFAULT_TILE_SIZE;
			g_iTileWidth = g_iMapWidth - df_DEFAULT_TILE_SIZE;				// 타일의 길이 (가장자리 1칸은 뺌)
			g_iTileHeight = g_iMapHeight - df_DEFAULT_TILE_SIZE;			// 타일의 길이 (가장자리 1칸은 뺌)


			//--------------------------------------
			// 카메라, 화면 구역
			// 맵 크기가 스크린보다 작을 경우
			// 초기 값이 다르다
			//--------------------------------------
			if (g_iMapWidth < g_iDestWidth) {
				iTemp = ((g_iDestWidth - g_iMapWidth) / 2);
				g_CameraRect.left = -iTemp;
				g_CameraRect.right = g_CameraRect.left + g_iDestWidth;

				// 화면 크기 기준
				g_WorldRect.left = -(iTemp + 500);
				g_WorldRect.right = g_iMapWidth + iTemp + 500;

				//g_CameraRect.left = 0;
				//g_CameraRect.right = g_CameraRect.left + g_iDestWidth;
				//g_WorldRect.left = 0;
				//g_WorldRect.right = g_iMapWidth;
			}
			else {
				g_CameraRect.left = 0;
				g_CameraRect.right = g_iDestWidth;

				// 맵 크기 기준
				g_WorldRect.left = -500;
				g_WorldRect.right = g_iMapWidth + 500;

				//g_WorldRect.left = 0;
				//g_WorldRect.right = g_iMapWidth;
			}


			if (g_iMapHeight < g_iDestHeight) {
				iTemp = ((g_iDestHeight - g_iMapHeight) / 2);
				g_CameraRect.top = -iTemp;
				g_CameraRect.bottom = g_CameraRect.top + g_iDestHeight;

				// 화면 크기 기준
				g_WorldRect.top = -(iTemp + 500);
				g_WorldRect.bottom = g_iMapHeight + iTemp + 500;

				//g_CameraRect.top = 0;
				//g_CameraRect.bottom = g_CameraRect.top + g_iDestHeight;
				//g_WorldRect.top = 0;
				//g_WorldRect.bottom = g_iMapHeight;
			}
			else {
				g_CameraRect.top = 0;
				g_CameraRect.bottom = g_iDestHeight;

				// 맵 크기 기준
				g_WorldRect.top = -500;
				g_WorldRect.bottom = g_iMapHeight + 500;

				//g_WorldRect.top = 0;
				//g_WorldRect.bottom = g_iMapHeight;
			}


			// 사이즈 변경
			g_ObjectStageMap->ResizeMap(g_iMapWidth , g_iMapHeight);
			g_ObjectLightDib->ResizeMap(g_iMapWidth, g_iMapHeight, 32, 0xff00ff);



			EndDialog(hDlg, TRUE);
			SetForegroundWindow(g_hWnd);
			return TRUE;
		case IDCANCEL:

			EndDialog(hDlg, TRUE);
			SetForegroundWindow(g_hWnd);
			return TRUE;
		}
		break;
	case WM_CLOSE:
		EndDialog(hDlg, TRUE);
		SetForegroundWindow(g_hWnd);
		return TRUE;
	}

	return FALSE;
}



//--------------------------------------
// 프레임 스킵
//--------------------------------------
void FrameSkip()
{
	DWORD dwTempTime;
	DWORD dwMillisecond;
	WCHAR szStr[256];

	


	//스킵
	g_dwTime = GetTickCount();
	dwTempTime = g_dwTime - g_dwOldTime;
	dwMillisecond = g_dwTime - g_dwFps;
	if (dwMillisecond >= 1000) {
		swprintf_s(szStr, L"Enter the Gungeon Map Tool - %d fps", g_iFramePerSecond);
		SetWindowTextW(g_hWnd, szStr);

		g_iFramePerSecond = 0;
		g_dwFps = g_dwTime;
		g_iCountNokori = g_iFrameNokori;
	}
	//else if (g_iFramePerSecond >= FPS) {
	//	return false;
	//}



	// 나머지 값 부분 보정
	if (g_iCountNokori != 0) {
		--g_iCountNokori;
		++dwTempTime;
	}
	// 소수점 값 보정
	g_iCountPoint = g_iCountPoint + g_iFramePoint;
	if (g_iCountPoint >= 10000) {
		g_iCountPoint = g_iCountPoint - 10000;
		++dwTempTime;
	}


	// Sleep 할지 안할지 판단
	if (dwTempTime > g_iFrameDeley) {
		g_dwSkipFrame = g_dwSkipFrame + dwTempTime - g_iFrameDeley;
	}
	else if (g_dwSkipFrame > g_iFrameDeley) {
		g_dwSkipFrame = g_dwSkipFrame + dwTempTime - g_iFrameDeley;
	}
	else {
		dwTempTime = g_iFrameDeley - dwTempTime;

		//// 나머지 값 부분 보정
		//if (g_iCountNokori) {
		//	--g_iCountNokori;
		//	++dwTempTime;
		//}

		//// 소수점 값 보정
		//g_iCountPoint = g_iCountPoint + g_iFramePoint;
		//if (g_iCountPoint >= 10000) {
		//	g_iCountPoint = g_iCountPoint - 10000;
		//	++dwTempTime;
		//}

		Sleep(dwTempTime);
	}

	//if (g_iFramePerSecond >= FPS) {
	//	return false;
	//}

	++g_iFramePerSecond;

	g_dwOldTime = GetTickCount();

	return;
}



//--------------------------------------
// 업데이트
//--------------------------------------
void Update()
{
	CList<Object*>::iterator iter;
	CList<Object*>::iterator iter_end;
	Object * ObjectTemp;

	//--------------------------------------
	// 키 입력 처리
	//--------------------------------------
	KeyProcess();

	//--------------------------------------
	// 정렬
	//--------------------------------------
	InsertionSort();

	//--------------------------------------
	// 오브젝트 처리
	//--------------------------------------
	iter = g_ObjectList->begin();
	iter_end = g_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		if (ObjectTemp->Action()) {
			ObjectTemp = *iter;
			iter = g_ObjectList->erase(iter);
			delete ObjectTemp;
			continue;

		}
		ObjectTemp->Draw();
		++iter;
	}


	//------------------------------------------------------------
	// ScreenDib 를 화면으로 플립
	//------------------------------------------------------------
	g_ScreenDib->DrawBuffer(g_hWnd);
}



//--------------------------------------
// 스프라이트 불러오기
//--------------------------------------
void LoadSprite()
{
	// 메뉴 텝
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_TAB_TILE, L"Image\\UI\\Menu_Tile.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_TAB_OBJECT, L"Image\\UI\\Menu_Object.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_TAB_ENEMY, L"Image\\UI\\Menu_Enemy.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_TAB_LOCATION, L"Image\\UI\\Menu_Location.bmp", 0, 0);

	// 리스트 아이콘
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_LIBRARY_BORDER, L"Image\\UI\\Menu_Library_Border.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_LIBRARY_FLOOR, L"Image\\UI\\Menu_Library_Floor.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_CASTLE_BORDER, L"Image\\UI\\Menu_Castle_Border.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_CASTLE_FLOOR, L"Image\\UI\\Menu_Castle_Floor.bmp", 0, 0);

	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_OBJECT_LANTERN, L"Image\\UI\\Menu_Lantern.bmp", 0, 0);

	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BULLET_KIN, L"Image\\UI\\Menu_Enemy_Bullet_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BANDANA_KIN, L"Image\\UI\\Menu_Enemy_Bullet_Bandana_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_RED_SHOTGUN_KIN, L"Image\\UI\\Menu_Enemy_Red_Shotgun_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BLUE_SHOTGUN_KIN, L"Image\\UI\\Menu_Enemy_Blue_Shotgun_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_MUMMY_KIN, L"Image\\UI\\Menu_Enemy_Mummy_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_SHOTGRUB_KIN, L"Image\\UI\\Menu_Enemy_Shotgrub_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_GHOST_KIN, L"Image\\UI\\Menu_Enemy_Ghost_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_FLY_KIN, L"Image\\UI\\Menu_Enemy_Fly_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BIRD, L"Image\\UI\\Menu_Enemy_Bird.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_CUBE, L"Image\\UI\\Menu_Enemy_Cube.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_GREEN_BOOK, L"Image\\UI\\Menu_Enemy_Green_Book.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BLUE_BOOK, L"Image\\UI\\Menu_Enemy_Blue_Book.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_YELLOW_MAGIC, L"Image\\UI\\Menu_Enemy_Yellow_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_BLUE_MAGIC, L"Image\\UI\\Menu_Enemy_Blue_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_RED_MAGIC, L"Image\\UI\\Menu_Enemy_Red_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_WISP, L"Image\\UI\\Menu_Enemy_Wisp.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ENEMY_MUSHROOM, L"Image\\UI\\Menu_Enemy_Mushroom.bmp", 0, 0);

	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_LOCATION_RECTANGLE, L"Image\\UI\\Menu_Location_Rectangle.bmp", 0, 0);

	g_cSprite->LoadDibSprite(df_SPRITE_UI_MENU_ELEMENT_SELECT, L"Image\\UI\\Menu_Select.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_UI_TILE_POINTER, L"Image\\UI\\Tile_Pointer.bmp", 0, 0);

	// 타일
	g_cSprite->LoadDibSprite(df_SPRITE_BASIC_TILE, L"Image\\Tile\\Basic_Tile.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_WALL, L"Image\\Tile\\Tile_Wall.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_01, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_02, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_03, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_03.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_04, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_04.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_05, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_05.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_06, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_06.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_07, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_07.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_08, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_08.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_09, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_09.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_10, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_10.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_11, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_11.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_12, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_12.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_13, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_13.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_14, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_14.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_15, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_15.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_16, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_16.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_17, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_17.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_18, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_18.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_19, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_19.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_20, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_20.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_21, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_21.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_22, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_22.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_23, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_23.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_24, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_24.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_25, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_25.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_26, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_26.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_27, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_27.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_28, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_28.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_29, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_29.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_30, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_30.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_31, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_31.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_32, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_32.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_33, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_33.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_34, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_34.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_35, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_35.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_36, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_36.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_37, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_37.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_38, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_38.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_39, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_39.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_40, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_40.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_41, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_41.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_42, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_42.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_43, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_43.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_44, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_44.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_45, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_45.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_BORDER_46, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Border_46.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_FLOOR, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Floor.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_WALL_TOP, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Wall_Top.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_LIBRARY_WALL_BOTTOM, L"Image\\Tile\\LeadLordKeep\\Library\\Tile_Library_Wall_Bottom.bmp", 0, 0);

	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_01, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_02, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_03, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_03.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_04, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_04.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_05, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_05.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_06, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_06.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_07, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_07.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_08, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_08.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_09, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_09.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_10, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_10.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_11, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_11.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_12, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_12.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_13, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_13.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_14, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_14.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_15, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_15.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_16, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_16.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_17, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_17.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_18, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_18.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_19, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_19.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_20, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_20.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_21, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_21.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_22, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_22.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_23, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_23.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_24, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_24.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_25, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_25.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_26, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_26.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_27, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_27.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_28, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_28.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_29, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_29.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_30, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_30.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_31, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_31.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_32, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_32.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_33, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_33.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_34, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_34.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_35, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_35.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_36, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_36.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_37, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_37.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_38, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_38.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_39, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_39.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_40, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_40.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_41, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_41.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_42, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_42.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_43, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_43.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_44, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_44.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_45, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_45.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_BORDER_46, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Border_46.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_FLOOR_01, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Floor_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_FLOOR_02, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Floor_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_WALL_TOP, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Wall_Top.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_TILE_CASTLE_WALL_BOTTOM, L"Image\\Tile\\LeadLordKeep\\Castle\\Tile_Castle_Wall_Bottom.bmp", 0, 0);



	// 빛
	//g_cSprite->LoadDibSprite(df_SPRITE_LIGHT_LANTERN_CENTER, L"Image\\Light\\Light_Lantern_Center.bmp", 65, 41);
	g_cSprite->LoadDibSprite(df_SPRITE_LIGHT_LANTERN_CENTER, L"Image\\Light\\Light_Lantern_Center.bmp", 100, 64);

	// 물체
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_CENTER_01, L"Image\\Object\\Object_Lantern_Center_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_CENTER_02, L"Image\\Object\\Object_Lantern_Center_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_CENTER_03, L"Image\\Object\\Object_Lantern_Center_03.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_LEFT_01, L"Image\\Object\\Object_Lantern_Left_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_LEFT_02, L"Image\\Object\\Object_Lantern_Left_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_LEFT_03, L"Image\\Object\\Object_Lantern_Left_03.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_RIGHT_01, L"Image\\Object\\Object_Lantern_Right_01.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_RIGHT_02, L"Image\\Object\\Object_Lantern_Right_02.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_OBJECT_LANTERN_RIGHT_03, L"Image\\Object\\Object_Lantern_Right_03.bmp", 0, 0);

	// 적
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BULLET_KIN, L"Image\\Enemy\\Enemy_Bullet_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BANDANA_KIN, L"Image\\Enemy\\Enemy_Bandana_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_RED_SHOTGUN_KIN, L"Image\\Enemy\\Enemy_Red_Shotgun_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN, L"Image\\Enemy\\Enemy_Blue_Shotgun_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_MUMMY_KIN, L"Image\\Enemy\\Enemy_Mummy_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_SHOTGRUB_KIN, L"Image\\Enemy\\Enemy_Shotgrub_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_GHOST_KIN, L"Image\\Enemy\\Enemy_Ghost_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_FLY_KIN, L"Image\\Enemy\\Enemy_Fly_Kin.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BIRD, L"Image\\Enemy\\Enemy_Bird.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_CUBE, L"Image\\Enemy\\Enemy_Cube.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_GREEN_BOOK, L"Image\\Enemy\\Enemy_Green_Book.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BLUE_BOOK, L"Image\\Enemy\\Enemy_Blue_Book.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_YELLOW_MAGIC, L"Image\\Enemy\\Enemy_Yellow_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_BLUE_MAGIC, L"Image\\Enemy\\Enemy_Blue_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_RED_MAGIC, L"Image\\Enemy\\Enemy_Red_Magic.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_WISP, L"Image\\Enemy\\Enemy_Wisp.bmp", 0, 0);
	g_cSprite->LoadDibSprite(df_SPRITE_ENEMY_MUSHROOM, L"Image\\Enemy\\Enemy_Mushroom.bmp", 0, 0);
}


//--------------------------------------
// 키 처리
//--------------------------------------
void KeyProcess()
{
	int iTileX;		// 마우스 타일 좌표 X
	int iTileY;		// 마우스 타일 좌표 Y

	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (g_CameraRect.top - df_SPEED_CAMERA >= g_WorldRect.top) {
			g_CameraRect.top = g_CameraRect.top - df_SPEED_CAMERA;
			g_CameraRect.bottom = g_CameraRect.bottom - df_SPEED_CAMERA;
			// 화면이 움직이면 마우스 좌표도 이동한다
			g_iMapMouseY = g_iMapMouseY - df_SPEED_CAMERA;
		}
		else {
			g_CameraRect.top = g_WorldRect.top;
			g_CameraRect.bottom = g_WorldRect.top + g_iDestHeight;
		}
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (g_CameraRect.bottom + df_SPEED_CAMERA <= g_WorldRect.bottom) {
			g_CameraRect.top = g_CameraRect.top + df_SPEED_CAMERA;
			g_CameraRect.bottom = g_CameraRect.bottom + df_SPEED_CAMERA;
			// 화면이 움직이면 마우스 좌표도 이동한다
			g_iMapMouseY = g_iMapMouseY + df_SPEED_CAMERA;
		}
		else {
			g_CameraRect.top = g_WorldRect.bottom - g_iDestHeight;
			g_CameraRect.bottom = g_WorldRect.bottom;
		}
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		if (g_CameraRect.left - df_SPEED_CAMERA >= g_WorldRect.left) {
			g_CameraRect.left = g_CameraRect.left - df_SPEED_CAMERA;
			g_CameraRect.right = g_CameraRect.right - df_SPEED_CAMERA;
			// 화면이 움직이면 마우스 좌표도 이동한다
			g_iMapMouseX = g_iMapMouseX - df_SPEED_CAMERA;
		}
		else {
			g_CameraRect.left = g_WorldRect.left;
			g_CameraRect.right = g_WorldRect.left + g_iDestWidth;
		}
	}

	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		if (g_CameraRect.right + df_SPEED_CAMERA <= g_WorldRect.right) {
			g_CameraRect.left = g_CameraRect.left + df_SPEED_CAMERA;
			g_CameraRect.right = g_CameraRect.right + df_SPEED_CAMERA;
			// 화면이 움직이면 마우스 좌표도 이동한다
			g_iMapMouseX = g_iMapMouseX + df_SPEED_CAMERA;
		}
		else {
			g_CameraRect.left = g_WorldRect.right - g_iDestWidth;
			g_CameraRect.right = g_WorldRect.right;
		}
	}

	g_cSprite->SetCameraPosition(g_CameraRect.left, g_CameraRect.top);


	//--------------------------------------
	// ESC
	// 모든 선택 상태를 초기화 하고
	// 기본 선택 상태로 되돌린다.
	//--------------------------------------
	if (GetAsyncKeyState(VK_ESCAPE) & 0x8000) {
		g_SelectedObjectUI = NULL;					// 선택된 오브젝트 UI
		g_iMouseState = df_MOUSE_STATE_EMPT;
		g_MousePointer->m_iSpriteIndex = df_SPRITE_EMPT_SPRITE;
		g_MousePointer->m_iStartSprite = df_SPRITE_EMPT_SPRITE;
		g_MousePointer->m_iEndSprite = df_SPRITE_EMPT_SPRITE;
		g_MousePointer->m_iAnimeDelay = 0;
		g_iSelectedType = df_TILE_TYPE_EMPT;
		g_iSelectedState = df_TILE_STATE_EMPT;

		// 유닛 선택 해제
		if (g_SelectedObjectUnit != NULL) {
			// 선택 해제
			g_SelectedObjectUnit->m_bSelectFlag = false;
		}
		g_SelectedObjectUnit = NULL;				// 선택된 유닛

		// 메뉴 선택 해제
		if (g_ObjectMenu->m_SelectElement != NULL) {
			g_ObjectMenu->m_SelectElement->m_bSelectElement = false;
		}
		g_ObjectMenu->m_SelectElement = NULL;
	}

	//--------------------------------------
	// DELETE
	// 선택된 유닛 삭제 후
	// 기본 선택 상태로 되돌린다.
	//--------------------------------------
	if (GetAsyncKeyState(VK_DELETE) & 0x8000) {
		// 선택된 유닛이 있나?
		if (g_iMouseState == df_MOUSE_STATE_SELECT_UNIT) {
			// 선택한 유닛 삭제
			g_ObjectStageMap->DeleteUnit(g_SelectedObjectUnit);
			// 마우스 상태 변경
			g_iMouseState = df_MOUSE_STATE_EMPT;
		}
		// 로케이션 이냐?
		else if (g_iMouseState == df_MOUSE_STATE_SELECT_LOCATION) {
			// 로케이션 삭제
			g_SelectedObjectUnit->m_bEraseFlag = true;
		}
	}

	//--------------------------------------
	// 트리거 메뉴 열기 끄기
	// T Key
	//--------------------------------------
	if (GetAsyncKeyState(0x54) & 0x0001) {
		if (g_ObjectTriggerMenu->m_bOpenMenu == false) {
			g_ObjectTriggerMenu->m_bOpenMenu = true;
		}
		else {
			g_ObjectTriggerMenu->m_bOpenMenu = false;
		}
	}


	//--------------------------------------
	// 마우스 처리
	//--------------------------------------
	switch (g_iMouseState) {
		break;
	case df_MOUSE_STATE_OBJECT_MAKE:

		switch (g_iSelectedState)
		{
		case df_UNIT_STATE_FLOOR:
			//--------------------------------------
			// 마우스가 자유로운 상황
			//--------------------------------------
			iTileX = g_iMapMouseX;
			iTileY = g_iMapMouseY;

			// 지정된 범위를 벗어나지 않는다
			if (iTileX < 0 || iTileY < 0 || g_iTileWidth < iTileX || g_iTileHeight < iTileY) {
				return;
			}

			// 마우스 좌표 갱신
			g_MousePointer->m_iX = iTileX;
			g_MousePointer->m_iY = iTileY;

			return;
		default:
			break;
		}
		break;
	default:
		break;
	}

	//--------------------------------------
	// 마우스가 자유롭지 못함
	//--------------------------------------
	iTileX = g_iMapMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE;
	iTileY = g_iMapMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE;

	// 지정된 범위를 벗어나지 않는다
	if (iTileX < 0 || iTileY < 0 || g_iTileWidth < iTileX || g_iTileHeight < iTileY) {
		return;
	}

	// 마우스 좌표 갱신
	g_MousePointer->m_iX = iTileX;
	g_MousePointer->m_iY = iTileY;
}


//--------------------------------------
// 삽입 정렬
//--------------------------------------
void InsertionSort()
{
	int iListSize;				// 리스트 크기
	int iListNum;				// 정렬 진행위치
	int iListEnd;				// 리스트 끝
	int iSortNum;				// 정렬 진행 위치
	Object *compareObject;					//비교할 값
	Object *sortObject;						//정렬할 값
	CList<Object *>::iterator iter;			//리스트 순환을 위한 이터레이터
	CList<Object *>::iterator iterSort;		//정렬을 위한 이터레이터


	iListSize = g_ObjectList->size();

	// 리스트 요소가 2개 이상일 때 정렬
	if (iListSize >= 2) {
		iListNum = 0;
		iListEnd = iListSize - 1;

		iter = g_ObjectList->begin();
		while (iListNum != iListEnd) {
			iterSort = iter;
			compareObject = iterSort._node->_Next->_data;

			iSortNum = iListNum;
			while (iSortNum >= 0) {
				sortObject = iterSort._node->_data;

				// 타입 에따라 우선순위 정렬
				// 같으면
				if (sortObject->m_iType == compareObject->m_iType) {
					// 우선순위 값으로 정렬 (값이 높은걸 뒤로)
					if (sortObject->m_iRenderPriority > compareObject->m_iRenderPriority) {
						iterSort._node->_Next->_data = sortObject;
						iterSort._node->_data = compareObject;
					}
					else {
						break;
					}
				}
				// 다르면 타입 큰게 뒤로감
				else if (sortObject->m_iType > compareObject->m_iType) {
					iterSort._node->_Next->_data = sortObject;
					iterSort._node->_data = compareObject;
				}
				else {
					break;
				}

				--iterSort;
				--iSortNum;
			}

			++iListNum;
			++iter;
		}//while (iListNum != iListEnd)
	}//if (iListSize >= 2)
}

void SaveProcess()
{
	CList<Object *>::iterator iter;
	CList<Object *>::iterator iter_end;
	Object *TempObject;

	BYTE *bypTileMapData;		// 타일 맵 정보
	FILE *fp = NULL;			// 파일 포인터
	WCHAR FileName[32];			// 파일 이름이 들어갈 배열
	UINT64 fileSize;			// 파일 크기
	int err;

	int iCnt;
	int iSize;

	StringBuffer StringJSON;
	Writer<StringBuffer, UTF16<>> writer(StringJSON);


	writer.StartObject();

	iter = g_ObjectList->begin();
	iter_end = g_ObjectList->end();
	writer.String(L"Object");
	writer.StartArray();
	while (iter != iter_end) {
		TempObject = *iter;
		
		if (TempObject->m_iType != df_TYPE_OBJECT && TempObject->m_iType != df_TYPE_LOCATION) {
			++iter;
			continue;
		}

		writer.StartObject();

		writer.String(L"iObjectType");
		writer.Uint(TempObject->m_iType);
		writer.String(L"iUnitType");
		writer.Uint(((ObjectUnit*)TempObject)->m_iUnitType);
		writer.String(L"iWidth");
		writer.Uint(TempObject->m_iWidth);
		writer.String(L"iHeight");
		writer.Uint(TempObject->m_iHeight);
		writer.String(L"iPosX");
		writer.Uint(TempObject->m_iX);
		writer.String(L"iPosY");
		writer.Uint(TempObject->m_iY);

		writer.EndObject();
		++iter;
	}
	writer.EndArray();

	// 맵 정보
	writer.String(L"MapData");
	writer.StartArray();
	writer.StartObject();
	writer.String(L"iWidth");
	writer.Uint(g_iMapWidth);
	writer.String(L"iHeight");
	writer.Uint(g_iMapHeight);
	writer.String(L"iTileMapSize");
	// 맵 타일 정보
	iSize = g_ObjectStageMap->GetBufferSize();
	writer.Uint(iSize);
	// 새로 타일 버퍼 만듬
	bypTileMapData = (BYTE*)malloc(iSize);
	memcpy_s(bypTileMapData, iSize, g_ObjectStageMap->m_stpTileMap, iSize);

	//--------------------------
	// 타일 번호 정보 파일 만들기
	//--------------------------
	// 파일 이름
	wsprintfW(FileName, L"TileMap.txt");
	// 파일 쓰기
	err = _wfopen_s(&fp, FileName, L"wt");
	if (err != 0) {
		return;
	}
	fwrite(bypTileMapData, iSize, 1, fp);
	fclose(fp);
	// 타일 속성이 저장되어있는 파일 이름
	writer.String(L"TileMapDataFileName");
	writer.String(FileName);

	writer.EndObject();
	writer.EndArray();

	writer.EndObject();



	const char *pJson = StringJSON.GetString();
	int size = StringJSON.GetSize();

	//WCHAR * wchBuffer = (WCHAR *)new char[size];

	// 파일 크기
	fileSize = size;

	// 파일 이름
	wsprintfW(FileName, L"Stage.txt");

	// 파일 쓰기

	err = _wfopen_s(&fp, FileName, L"wt");
	if (err != 0) {
		return;
	}

	//헤더
	fwrite(&fileSize, sizeof(UINT64), 1, fp);
	//내용물
	fwrite(pJson, size, 1, fp);
	fclose(fp);
}

void LoadProcess()
{
	FILE *fp = NULL;
	WCHAR FileName[32];
	UINT64 fileSize;					// 읽어들일 JSON 사이즈
	int headerSize = sizeof(UINT64);
	int err;

	CList<Object*>::iterator iter;
	CList<Object*>::iterator iter_end;
	Object * ObjectTemp;

	// 현제 맵툴 위에있는 오브젝트는 날려버림
	iter = g_ObjectList->begin();
	iter_end = g_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;

		// 해당 타입 삭제
		switch (ObjectTemp->m_iType) {
		case df_TYPE_OBJECT:
		case df_TYPE_LOCATION:
			ObjectTemp = *iter;
			iter = g_ObjectList->erase(iter);
			delete ObjectTemp;
			continue;
		}

		++iter;
	}

	// 조명 버퍼 비움
	g_ObjectLightDib->ClearLightBuffer();


	// 로케이션 고유 번호인데 이거 세이브할때 줘야함;;;
	// 일단 눈속임으로 0으로 초기화함
	g_iLocationNumber = 0;


	wsprintfW(FileName, L"Stage.txt");

	//파일 읽기
	err = _wfopen_s(&fp, FileName, L"rt");
	if (err != 0) {
		return;
	}

	//헤더 읽기
	fread_s(&fileSize, headerSize, headerSize, 1, fp);

	char *pJson;

	pJson = new char[fileSize + 1];
	fread_s(pJson, fileSize, fileSize, 1, fp);
	pJson[fileSize] = '\0';

	fclose(fp);


	Document Doc;
	Doc.Parse(pJson);

	int iObjectType;
	int iUnitType;
	int iWidth;
	int iHeight;
	int iPosX;
	int iPosY;
	int iPropertiesWidth;
	int iPropertiesHeight;
	int iCnt;
	int iSize;
	char szObjectName[512];
	Object *newObject;
	Value::ConstMemberIterator itr;
	st_SpriteInfo stSpriteInfo;

	itr = Doc.FindMember("MapData");
	if (itr != Doc.MemberEnd()) {
		Value &StageSizeArray = Doc["MapData"];
		for (SizeType i = 0; i < StageSizeArray.Size(); i++) {
			Value &StageMapDataObject = StageSizeArray[i];
			itr = StageMapDataObject.FindMember("iWidth");
			if (itr == StageMapDataObject.MemberEnd()) {
				break;
			}
			itr = StageMapDataObject.FindMember("iHeight");
			if (itr == StageMapDataObject.MemberEnd()) {
				break;
			}
			itr = StageMapDataObject.FindMember("iTileMapSize");
			if (itr == StageMapDataObject.MemberEnd()) {
				break;
			}
			itr = StageMapDataObject.FindMember("TileMapDataFileName");
			if (itr == StageMapDataObject.MemberEnd()) {
				break;
			}
			// 스테이지 크기
			g_iMapWidth = StageMapDataObject["iWidth"].GetUint();
			g_iMapHeight = StageMapDataObject["iHeight"].GetUint();
			iSize = StageMapDataObject["iTileMapSize"].GetUint();
			// 사이즈 변경
			g_ObjectStageMap->ResizeMap(g_iMapWidth, g_iMapHeight);

			// UTF8 -> UTF16 변환
			UTF8toUTF16(StageMapDataObject["TileMapDataFileName"].GetString(), FileName, 32);

			// 파일 읽기
			err = _wfopen_s(&fp, FileName, L"rt");
			if (err != 0) {
				return;
			}

			// 타일 맵 데이터 읽기
			// ResizeMap 함수를 호출해서 버퍼를 동적할당 해놨음
			// 동적 할당한 곳에 파일 데이터 복사
			fread_s(g_ObjectStageMap->m_stpTileMap, iSize, iSize, 1, fp);
			fclose(fp);
		}
	}


	// 물체 정보
	itr = Doc.FindMember("Object");
	if (itr != Doc.MemberEnd()) {
		Value &ObjectArray = Doc["Object"];
		for (SizeType i = 0; i < ObjectArray.Size(); i++) {
			Value &Object = ObjectArray[i];
			itr = Object.FindMember("iObjectType");
			if (itr == Object.MemberEnd()) {
				break;
			}
			itr = Object.FindMember("iUnitType");
			if (itr == Object.MemberEnd()) {
				break;
			}
			itr = Object.FindMember("iWidth");
			if (itr == Object.MemberEnd()) {
				break;
			}
			itr = Object.FindMember("iHeight");
			if (itr == Object.MemberEnd()) {
				break;
			}
			itr = Object.FindMember("iPosX");
			if (itr == Object.MemberEnd()) {
				break;
			}
			itr = Object.FindMember("iPosY");
			if (itr == Object.MemberEnd()) {
				break;
			}
			iObjectType = Object["iObjectType"].GetUint();
			iUnitType = Object["iUnitType"].GetUint();
			iWidth = Object["iWidth"].GetUint();
			iHeight = Object["iHeight"].GetUint();
			iPosX = Object["iPosX"].GetUint();
			iPosY = Object["iPosY"].GetUint();

			// 유닛 생성
			switch (iObjectType) {
			case df_TYPE_OBJECT:
				g_ObjectStageMap->CreateUnit(iPosX, iPosY, iUnitType, 4);
				break;
			case df_TYPE_LOCATION:
				newObject = new UnitLocation(iPosX, iPosY);
				newObject->m_iWidth = iWidth;
				newObject->m_iHeight = iHeight;
				newObject->m_iRight = newObject->m_iLeft + iWidth;
				newObject->m_iBottom = newObject->m_iTop + iHeight;
				//------------------------
				// 2020.03.30
				// 문자열 관련 에러
				//------------------------
				//g_ObjectList->push_back(newObject);
				break;
			default:
				break;
			}
		}
	}
}

void UTF8toUTF16(const char * szText, WCHAR * szBuff, int iBuffLen)
{
	int iRe = MultiByteToWideChar(CP_UTF8, 0, szText, strlen(szText), szBuff, iBuffLen);
	if (iRe < iBuffLen)
		szBuff[iRe] = L'\0';
	return;
}
