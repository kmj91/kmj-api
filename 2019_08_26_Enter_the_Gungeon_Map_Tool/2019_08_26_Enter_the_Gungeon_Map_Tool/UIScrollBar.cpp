#include "stdafx.h"
#include "UIScrollBar.h"
#include "ListTileUnit.h"


UIScrollBar::UIScrollBar(int iPosX, int iPosY, int iWidth, int iHeight, UIList * MenuListPtr)
{
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 스크롤 바 영역
	m_iScrollBarLeft = iPosX;
	m_iScrollBarRight = iPosX + iWidth;
	m_iScrollBarTop = iPosY;
	m_iScrollBarBottom = iPosY;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 50;
	// 메뉴 목록 포인터
	m_MenuListPtr = MenuListPtr;
	// 스크롤 바 크기
	m_iScrollBarSize = 0;
	// 스크롤 바 사용 여부
	m_bEmployScrollBar = false;
}


UIScrollBar::~UIScrollBar()
{
}

bool UIScrollBar::Action()
{
	return false;
}

void UIScrollBar::Draw()
{
	if (m_bEmployScrollBar) {
		g_cSprite->DrawBackground(0xffffff, m_iX, m_iY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, NULL, false, false);
		g_cSprite->DrawBackground(0x00bfff, m_iScrollBarLeft, m_iScrollBarTop, m_iWidth, m_iScrollBarSize, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, NULL, false, false);
	}
}

bool UIScrollBar::Click()
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

		g_iMouseClickObjectType = df_MOUSE_CLICK_MENU_SCROLL_BAR;
		g_SelectedObjectUI = this;
		return true;
	}

	return false;
}

void UIScrollBar::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;

	m_iScrollBarLeft = m_iScrollBarLeft - iMoveValueX;
	m_iScrollBarTop = m_iScrollBarTop - iMoveValueY;
	m_iScrollBarRight = m_iScrollBarRight - iMoveValueX;
	m_iScrollBarBottom = m_iScrollBarBottom - iMoveValueY;
}

void UIScrollBar::ScrollMove(int iMoveValueY)
{
	// 위쪽
	if (m_iScrollBarTop - iMoveValueY < m_iTop) {
		iMoveValueY = m_iTop - m_iScrollBarTop;
	}

	// 아래쪽
	if (m_iScrollBarBottom - iMoveValueY > m_iBottom) {
		iMoveValueY = m_iBottom - m_iScrollBarBottom;
	}

	m_iScrollBarTop = m_iScrollBarTop - iMoveValueY;
	m_iScrollBarBottom = m_iScrollBarBottom - iMoveValueY;

	// 메뉴 리스트 목록 스크롤 좌표 변경
	m_MenuListPtr->ScrollMove(iMoveValueY);
}

void UIScrollBar::ChangeScrollBarSize(int iScrollBarSize, bool bEmployScrollBar)
{
	// 사용 여부
	m_bEmployScrollBar = bEmployScrollBar;
	// 스크롤 바 크기
	m_iScrollBarSize = iScrollBarSize;
	// 스크롤 바 영역
	m_iScrollBarBottom = m_iScrollBarTop + m_iScrollBarSize;
}
