#include "stdafx.h"
#include <windows.h>
#include "CScreenDib.h"

CScreenDib::CScreenDib(int iWidth, int iHeight, int iColorBit) {
	//스크린 버퍼 생성
	CreateDibBuffer(iWidth, iHeight, iColorBit);
}

CScreenDib::~CScreenDib() {
	ReleaseDibBuffer();
}


void CScreenDib::CreateDibBuffer(int iWidth, int iHeight, int iColorBit) {
	m_iWidth = iWidth;
	m_iHeight = iHeight;

	m_stDibInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_stDibInfo.bmiHeader.biWidth = iWidth;
	m_stDibInfo.bmiHeader.biHeight = -iHeight;
	m_stDibInfo.bmiHeader.biPlanes = 1;
	m_stDibInfo.bmiHeader.biBitCount = iColorBit;
	m_stDibInfo.bmiHeader.biSizeImage = 0;
	m_stDibInfo.bmiHeader.biClrUsed = 0;
	m_stDibInfo.bmiHeader.biClrImportant = 0;
	/*
	m_stDibInfo.bmiColors[0].rgbRed = 0;
	m_stDibInfo.bmiColors[0].rgbGreen = 0;
	m_stDibInfo.bmiColors[0].rgbBlue = 0;
	m_stDibInfo.bmiColors[0].rgbReserved = 0;
	*/

	m_iPitch = (iWidth * (iColorBit / 8) + 3) & ~3;
	m_iBufferSize = m_iPitch * m_iHeight;

	m_bypBuffer = (BYTE*)malloc(m_iBufferSize);
}


void CScreenDib::ReleaseDibBuffer(void) {
	free(m_bypBuffer);
}


void CScreenDib::DrawBuffer(HWND hWnd, int iX, int iY) {
	HDC hdc;
	RECT DCRect;
	wchar_t szStr[256];
	HFONT myFont;
	HFONT oldFont;
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	GetClientRect(hWnd, &DCRect);
	hdc = GetDC(hWnd);

	StretchDIBits(hdc, iX, iY, m_iWidth, m_iHeight, iX, iY, m_iWidth, m_iHeight, m_bypBuffer,
					&m_stDibInfo, DIB_RGB_COLORS, SRCCOPY);

	switch (g_GameState) {
	case TITLE:
		SetTextAlign(hdc, TA_CENTER);				// 가운데 정렬
		SetBkMode(hdc, TRANSPARENT);				// 텍스트 배경 투명

		if (g_iCoin == 0) {
			myFont = CreateFont(50, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"굴림체");
			oldFont = (HFONT)SelectObject(hdc, myFont);
			swprintf_s(szStr, L"INSERT COINS!");
			// 테두리
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, (df_WIN_RIGHT / 2) - 2, 700, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, (df_WIN_RIGHT / 2) + 2, 700, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, (df_WIN_RIGHT / 2), 700 - 2, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, (df_WIN_RIGHT / 2), 700 + 2, szStr, wcsnlen_s(szStr, 256));

			SetTextColor(hdc, RGB(255, 255, 255));
			TextOut(hdc, df_WIN_RIGHT / 2, 700, szStr, wcsnlen_s(szStr, 256));

			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}
		

		myFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"굴림체");
		oldFont = (HFONT)SelectObject(hdc, myFont);
		// 코인 수
		swprintf_s(szStr, L"CREDIT : %d", g_iCoin);
		// 테두리
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, (df_WIN_RIGHT / 2) - 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, (df_WIN_RIGHT / 2) + 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, (df_WIN_RIGHT / 2), 960 - 2, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, (df_WIN_RIGHT / 2), 960 + 2, szStr, wcsnlen_s(szStr, 256));

		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, df_WIN_RIGHT / 2, 960, szStr, wcsnlen_s(szStr, 256));

		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
		break;
	case SELECT:
		SetBkMode(hdc, TRANSPARENT);				// 텍스트 배경 투명

		myFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"굴림체");
		oldFont = (HFONT)SelectObject(hdc, myFont);
		// 코인 수
		swprintf_s(szStr, L"CREDIT : %d", g_iCoin);
		// 테두리
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 500 - 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500 + 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500, 960 - 2, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500, 960 + 2, szStr, wcsnlen_s(szStr, 256));

		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 500, 960, szStr, wcsnlen_s(szStr, 256));

		SelectObject(hdc, oldFont);
		DeleteObject(myFont);
		break;
	case GAME:
		myFont = CreateFont(20, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"굴림체");
		oldFont = (HFONT)SelectObject(hdc, myFont);
		SetTextColor(hdc, RGB(255, 255, 255));
		SetBkMode(hdc, TRANSPARENT);
		// 마우스 좌표
		swprintf_s(szStr, L"X : %d,  Y : %d", g_iMouseX, g_iMouseY);
		TextOut(hdc, 480, 10, szStr, wcsnlen_s(szStr, 256));
		// 스테이지 스크롤 좌표
		swprintf_s(szStr, L"MapScroll  Y : %d", (int)g_StageBackground->m_Y);
		TextOut(hdc, 480, 30, szStr, wcsnlen_s(szStr, 256));
		// 오브젝트 리스트 사이즈
		swprintf_s(szStr, L"object count : %d", (int)g_ObjectList->size());
		TextOut(hdc, 480, 50, szStr, wcsnlen_s(szStr, 256));
		// FPS
		swprintf_s(szStr, L"%d fps", g_Frame);
		TextOut(hdc, 480, 70, szStr, wcsnlen_s(szStr, 256));
		// 스킵 프레임
		swprintf_s(szStr, L"g_skipFrame : %d", g_skipFrame);
		TextOut(hdc, 480, 90, szStr, wcsnlen_s(szStr, 256));

		// 코인 수
		swprintf_s(szStr, L"CREDIT : %d", g_iCoin);
		// 테두리
		SetTextColor(hdc, RGB(0, 0, 0));
		TextOut(hdc, 500 - 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500 + 2, 960, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500, 960 - 2, szStr, wcsnlen_s(szStr, 256));
		TextOut(hdc, 500, 960 + 2, szStr, wcsnlen_s(szStr, 256));

		SetTextColor(hdc, RGB(255, 255, 255));
		TextOut(hdc, 500, 960, szStr, wcsnlen_s(szStr, 256));

		SelectObject(hdc, oldFont);
		DeleteObject(myFont);

		// 게임 오버
		if (g_bGameOver) {
			SetTextAlign(hdc, TA_CENTER);				// 가운데 정렬
			myFont = CreateFont(60, 0, 0, 0, 0, 0, 0, 0, DEFAULT_CHARSET, 0, 0, 0, 0, L"굴림체");
			oldFont = (HFONT)SelectObject(hdc, myFont);
			// 컨티뉴?
			swprintf_s(szStr, L"CONTINUE? %d", g_iContinueCnt);
			// 테두리
			SetTextColor(hdc, RGB(0, 0, 0));
			TextOut(hdc, df_WIN_RIGHT / 2 - 2, df_WIN_BOTTOM / 2, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, df_WIN_RIGHT / 2 + 2, df_WIN_BOTTOM / 2, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, df_WIN_RIGHT / 2, df_WIN_BOTTOM / 2 - 2, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, df_WIN_RIGHT / 2, df_WIN_BOTTOM / 2 + 2, szStr, wcsnlen_s(szStr, 256));
			TextOut(hdc, df_WIN_RIGHT / 2, df_WIN_BOTTOM / 2, szStr, wcsnlen_s(szStr, 256));

			SetTextColor(hdc, RGB(255, 255, 255));
			TextOut(hdc, df_WIN_RIGHT / 2, df_WIN_BOTTOM / 2, szStr, wcsnlen_s(szStr, 256));

			SelectObject(hdc, oldFont);
			DeleteObject(myFont);
		}


		

		/*
		iter = g_ObjectList->begin();
		iter_end = g_ObjectList->end();
		while (iter != iter_end) {
			ObjectTemp = *iter;
			Rectangle(hdc, ObjectTemp->m_fLeft, ObjectTemp->m_fTop, ObjectTemp->m_fRight, ObjectTemp->m_fBottom);
			++iter;
		}
		*/
		
		break;
	}

	ReleaseDC(hWnd, hdc);
}


BYTE * CScreenDib::GetDibBuffer(void) {
	return m_bypBuffer;
}


int CScreenDib::GetWidth(void) {
	return m_iWidth;
}


int CScreenDib::GetHeight(void) {
	return m_iHeight;
}


int CScreenDib::GetPitch(void) {
	return m_iPitch;
}