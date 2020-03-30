#include "stdafx.h"
#include "ObjectMenuElementListScrollBar.h"
#include "ObjectMenuElementList.h"


ObjectMenuElementListScrollBar::ObjectMenuElementListScrollBar(int iPosX, int iPosY, int iWidth, int iHeight, ObjectMenuElementList * MenuListPtr)
{
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 스크롤 바 영역
	m_dScrollBarLeft = iPosX;
	m_dScrollBarRight = iPosX + iWidth;
	m_dScrollBarTop = iPosY;
	m_dScrollBarBottom = iPosY;
	// 오브젝트 범위
	m_dLeft = m_dX;
	m_dTop = m_dY;
	m_dRight = m_dLeft + m_iWidth;
	m_dBottom = m_dTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 50;
	// 메뉴 목록 포인터
	m_MenuListPtr = MenuListPtr;
	// 스크롤 바 크기
	m_iScrollBarSize = 0;
	// 스크롤 바 사용 여부
	m_bEmployScrollBar = false;
}


ObjectMenuElementListScrollBar::~ObjectMenuElementListScrollBar()
{
}

bool ObjectMenuElementListScrollBar::Action()
{
	return false;
}

void ObjectMenuElementListScrollBar::Draw()
{
	if (m_bEmployScrollBar) {
		g_cSprite->DrawBackground(0xffffff, m_dX, m_dY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, NULL, false, false);
		g_cSprite->DrawBackground(0x00bfff, m_dScrollBarLeft, m_dScrollBarTop, m_iWidth, m_iScrollBarSize, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, NULL, false, false);
	}
	
}

bool ObjectMenuElementListScrollBar::Click()
{
	int iMouseX;
	int iMouseY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	// 카메라 좌표를 빼준다
	//--------------------------------------
	iMouseX = g_iMouseX - g_CameraRect.left;
	iMouseY = g_iMouseY - g_CameraRect.top;

	if (m_dLeft < iMouseX && iMouseX < m_dRight &&
		m_dTop < iMouseY && iMouseY < m_dBottom) {

		g_MouseClickObject = MOUSE_CLICK_MENU_SCROLL_BAR;
		return true;
	}

	return false;
}

void ObjectMenuElementListScrollBar::Move(int iMoveValueX, int iMoveValueY)
{
	m_dX = m_dX - iMoveValueX;
	m_dY = m_dY - iMoveValueY;
	m_dLeft = m_dLeft - iMoveValueX;
	m_dTop = m_dTop - iMoveValueY;
	m_dRight = m_dRight - iMoveValueX;
	m_dBottom = m_dBottom - iMoveValueY;

	m_dScrollBarLeft = m_dScrollBarLeft - iMoveValueX;
	m_dScrollBarTop = m_dScrollBarTop - iMoveValueY;
	m_dScrollBarRight = m_dScrollBarRight - iMoveValueX;
	m_dScrollBarBottom = m_dScrollBarBottom - iMoveValueY;
}

void ObjectMenuElementListScrollBar::ScrollMove(int iMoveValueY)
{
	// 위쪽
	if (m_dScrollBarTop - iMoveValueY < m_dTop) {
		iMoveValueY = m_dTop - m_dScrollBarTop;
	}

	// 아래쪽
	if (m_dScrollBarBottom - iMoveValueY > m_dBottom) {
		iMoveValueY = m_dBottom - m_dScrollBarBottom;
	}

	m_dScrollBarTop = m_dScrollBarTop - iMoveValueY;
	m_dScrollBarBottom = m_dScrollBarBottom - iMoveValueY;

	// 메뉴 리스트 목록 스크롤 좌표 변경
	m_MenuListPtr->ScrollMove(iMoveValueY);
}


void ObjectMenuElementListScrollBar::ChangeScrollBarSize(int iScrollBarSize, bool bEmployScrollBar)
{
	// 사용 여부
	m_bEmployScrollBar = bEmployScrollBar;
	// 스크롤 바 크기
	m_iScrollBarSize = iScrollBarSize;
	// 스크롤 바 영역
	m_dScrollBarBottom = m_dScrollBarTop + m_iScrollBarSize;
}