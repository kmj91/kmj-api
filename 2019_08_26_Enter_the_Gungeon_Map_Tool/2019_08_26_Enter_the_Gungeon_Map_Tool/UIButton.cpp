#include "stdafx.h"
#include "UIButton.h"
#include "UIMenu.h"


UIButton::UIButton(const wchar_t * wchpButtonName, int iPosX, int iPosY, int iWidth, int iHeight, int iMenuType, UIMenu * MenuPtr)
{
	int iCnt;
	int ierror;

	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 가로, 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;
	// 텝이 담당하는 메뉴
	m_iMenuType = iMenuType;
	// 메뉴 포인터
	m_MenuPtr = MenuPtr;
	
	// 버튼 이름 길이
	m_iNameSize = wcsnlen_s(wchpButtonName, 20);
	// 이름 저장할 메모리 동적 생성
	m_wchpName = (wchar_t *)malloc(m_iNameSize * 2);

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		m_wchpName[iCnt] = wchpButtonName[iCnt];
		++iCnt;
	}

	// 폰트 페이스 생성
	m_face = new FT_Face[m_iNameSize];

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		// 폰트 읽음
		ierror = FT_New_Face(g_library, "font\\malgun.ttf", 0, &m_face[iCnt]);
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_New_Face 실패"), TEXT("에러"), MB_OK);
		}

		// 크기 설정
		// 글리프가 저장된 페이스, 폭, 높이, 출력장비 해상도 가로, 세로
		ierror = FT_Set_Char_Size(m_face[iCnt], 20 * 64, 0, GetDeviceCaps(g_hdc, LOGPIXELSX), GetDeviceCaps(g_hdc, LOGPIXELSY));
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_Set_Char_Size 실패"), TEXT("에러"), MB_OK);
		}

		// 문자의 글리프 비트맵 구함
		ierror = FT_Load_Char(m_face[iCnt], m_wchpName[iCnt], FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_New_Face 실패"), TEXT("에러"), MB_OK);
			return;
		}

		++iCnt;
	}
}


UIButton::~UIButton()
{
	int iCnt;

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		// 폰트 페이스 정리
		FT_Done_Face(m_face[iCnt]);
		++iCnt;
	}
	
	// 메모리 정리
	free(m_wchpName);
}

bool UIButton::Action()
{
	return false;
}

void UIButton::Draw()
{
	int iStrCnt;
	int iWidth;
	int iHeight;
	int iCntY;
	int iCntX;
	int penx;
	int peny;
	int ierror;
	int iX;
	int iY;
	DWORD dwColor;
	BYTE byColor;
	DWORD *dwpDest;
	BYTE *bypSprite;
	BYTE *bypTempDest;

	// 테두리
	//g_cSprite->DrawRectangleBorder(0x375681, 4, m_iLeft, m_iTop, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, false);
	// 사각형
	g_cSprite->DrawBackground(0xffffff, m_iLeft, m_iTop, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 1, false, true);

	penx = 0 * 64;
	peny = 200 * 64;

	// 위치 좌표
	iX = m_iX + 10;
	iY = m_iY + 10;

	iStrCnt = 0;
	while (iStrCnt < m_iNameSize) {
		// 화면으로 출력
		iWidth = m_face[iStrCnt]->glyph->bitmap.width;
		iHeight = m_face[iStrCnt]->glyph->bitmap.rows;
		
		// 문자 버퍼
		dwpDest = (DWORD *)g_bypDest;
		dwpDest = (DWORD *)((BYTE *)(dwpDest + iX + (penx>>6)) + (iY * g_iDestPitch));
		bypSprite = m_face[iStrCnt]->glyph->bitmap.buffer;
		bypTempDest = (BYTE *)dwpDest;

		iCntY = 0;
		while (iCntY < iHeight) {
			iCntX = 0;
			while (iCntX < iWidth) {
				// 값이 클수록 검정색을 나타낸다
				dwColor = 255 - *bypSprite;
				if (dwColor == 0) {
					*dwpDest = RGB(dwColor, dwColor, dwColor);
				}
				else if(dwColor != 255){
					dwColor = 255 - *bypSprite;
					*((BYTE *)dwpDest + 2) = (dwColor * 0.5) + (0xffffff * 0.5);
					*((BYTE *)dwpDest + 1) = (dwColor * 0.5) + (0xffffff * 0.5);
					*((BYTE *)dwpDest + 0) = (dwColor * 0.5) + (0xffffff * 0.5);
				}

				//다음칸 이동
				++dwpDest;
				++bypSprite;
				++iCntX;
			}
			//-----------------------------------------------------------------
			// 다음줄로 이동, 화면과 스프라이트 모두..
			//-----------------------------------------------------------------
			++iCntY;
			bypTempDest = bypTempDest + g_iDestPitch;
			dwpDest = (DWORD *)bypTempDest;
			bypSprite = &m_face[iStrCnt]->glyph->bitmap.buffer[iCntY * iWidth];
			
		}

		penx += m_face[iStrCnt]->glyph->advance.x;
		peny += m_face[iStrCnt]->glyph->advance.y;

		++iStrCnt;
	}
}

bool UIButton::Click()
{
	int iMouseX;
	int iMouseY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		m_MenuPtr->m_iChangeMenu = m_iMenuType;

		return true;
	}

	return false;
}

void UIButton::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;
}
