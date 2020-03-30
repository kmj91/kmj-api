#include "stdafx.h"
#include "ObjectMenuTitleBar.h"
#include "ObjectMenu.h"

ObjectMenuTitleBar::ObjectMenuTitleBar(DWORD dwRGBColor, int iPosX, int iPosY, int iWidth, int iHeight, double dCompose)
{
	// 색깔
	m_dwRGBColor = dwRGBColor;
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 합성 값
	m_dCompose = dCompose;
	// 오브젝트 범위
	m_dLeft = m_dX;
	m_dTop = m_dY;
	m_dRight = m_dLeft + m_iWidth;
	m_dBottom = m_dTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;
}


ObjectMenuTitleBar::~ObjectMenuTitleBar()
{
}

bool ObjectMenuTitleBar::Action()
{

	return false;
}

void ObjectMenuTitleBar::Draw()
{
	g_cSprite->DrawBackground(m_dwRGBColor, m_dX, m_dY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_dCompose, false, true);
}

bool ObjectMenuTitleBar::Click()
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

		g_MouseClickObject = MOUSE_CLICK_MENU_TITLE_BAR;
		return true;
	}

	return false;
}

void ObjectMenuTitleBar::Move(int iMoveValueX, int iMoveValueY)
{
	m_dX = m_dX - iMoveValueX;
	m_dY = m_dY - iMoveValueY;
	m_dLeft = m_dLeft - iMoveValueX;
	m_dTop = m_dTop - iMoveValueY;
	m_dRight = m_dRight - iMoveValueX;
	m_dBottom = m_dBottom - iMoveValueY;
}
