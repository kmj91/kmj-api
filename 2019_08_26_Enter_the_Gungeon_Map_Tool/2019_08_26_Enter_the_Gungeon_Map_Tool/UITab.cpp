#include "stdafx.h"
#include "UITab.h"
#include "UIMenu.h"


UITab::UITab(int iSpriteIndex, int iPosX, int iPosY, int iMenuType, UIMenu * MenuPtr)
{
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = iSpriteIndex;
	// 가로, 세로 길이
	m_iWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
	m_iHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
	// 오브젝트 범위
	m_iLeft = m_iX - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_iTop = m_iY - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;
	// 텝이 담당하는 메뉴
	m_iMenuType = iMenuType;
	// 메뉴 포인터
	m_MenuPtr = MenuPtr;
}


UITab::~UITab()
{
}

bool UITab::Action()
{
	return false;
}

void UITab::Draw()
{
	g_cSprite->DrawImage(m_iSpriteIndex, m_iX, m_iY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, false);
}

bool UITab::Click()
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

void UITab::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;
}
