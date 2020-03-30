#include "stdafx.h"
#include "UITitleBar.h"


UITitleBar::UITitleBar(DWORD dwRGBColor, int iPosX, int iPosY, int iWidth, int iHeight, double dCompose, ObjectUI * MenuPtr)
{
	// 색깔
	m_dwRGBColor = dwRGBColor;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 합성 값
	m_dCompose = dCompose;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 메뉴 포인터
	m_MenuPtr = MenuPtr;
}


UITitleBar::~UITitleBar()
{
}

bool UITitleBar::Action()
{
	return false;
}

void UITitleBar::Draw()
{
	g_cSprite->DrawBackground(m_dwRGBColor, m_iX, m_iY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_dCompose, false, true);
}

bool UITitleBar::Click()
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

		g_iMouseClickObjectType = df_MOUSE_CLICK_MENU_TITLE_BAR;
		g_SelectedObjectUI = m_MenuPtr;
		return true;
	}

	return false;
}

void UITitleBar::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;
}
