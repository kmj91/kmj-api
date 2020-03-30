#include "stdafx.h"
#include "ObjectMenu.h"
#include "ObjectMenuTitleBar.h"
#include "ObjectMenuTile.h"
#include "ObjectMenuStage.h"
#include "ObjectMenuPlayer.h"
#include "ObjectMenuObject.h"
#include "ObjectMenuBroken.h"
#include "ObjectMenuEnemy.h"
#include "ObjectMenuElementList.h"


ObjectMenu::ObjectMenu(int iPosX, int iPosY)
{
	ObjectUI * newObject;
	// 타입
	m_iType = df_TYPE_UI;
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 가로 세로 길이
	m_iWidth = 350;
	m_iHeight = 300;
	// 오브젝트 범위
	m_dLeft = m_dX;
	m_dTop = m_dY;
	m_dRight = m_dLeft + m_iWidth;
	m_dBottom = m_dTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = df_RENDER_PRIORITY_LAST;
	// 메뉴 오브젝트 리스트
	m_MenuObjectList = new CList<ObjectUI *>;

	//-------------------------------------
	// 타이틀 바
	//-------------------------------------
	newObject = new ObjectMenuTitleBar(0xff69c3e2, m_dX, m_dY, m_iWidth, 40, 0.8);
	m_MenuObjectList->push_back(newObject);

	//-------------------------------------
	// 메뉴
	// 디폴트 선택
	//-------------------------------------
	m_FocusMenu = FOCUS_MENU_TILE;
	m_ChangeMenu = FOCUS_MENU_EMPT;

	// 타일 메뉴
	newObject = new ObjectMenuTile(m_dX + 5, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);
	// 스테이지 메뉴
	newObject = new ObjectMenuStage(m_dX + 60, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);
	// 플레이어 메뉴
	newObject = new ObjectMenuPlayer(m_dX + 115, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);
	// 물체 메뉴
	newObject = new ObjectMenuObject(m_dX + 170, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);
	// 부셔지는 물체 메뉴
	newObject = new ObjectMenuBroken(m_dX + 225, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);
	// 적 메뉴
	newObject = new ObjectMenuEnemy(m_dX + 280, m_dY + 45, this);
	m_MenuObjectList->push_back(newObject);

	//-------------------------------------
	// 메뉴 요소 목록
	//-------------------------------------
	m_MenuElementList = new ObjectMenuElementList(m_dX, m_dY + 80, m_iWidth, m_iHeight - 80);
	m_MenuObjectList->push_back(m_MenuElementList);
	g_TestObject = m_MenuElementList;

	// 리스트 요소 갱신
	m_MenuElementList->ChangeElementList(m_FocusMenu);
}


ObjectMenu::~ObjectMenu()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	//--------------------------------------
	// 메모리 정리
	//--------------------------------------
	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = m_MenuObjectList->erase(iter);
		delete ObjectTemp;
	}

	delete m_MenuObjectList;
}

bool ObjectMenu::Action()
{
	if (m_ChangeMenu != FOCUS_MENU_EMPT) {
		m_FocusMenu = m_ChangeMenu;
		// 리스트 요소 갱신
		m_MenuElementList->ChangeElementList(m_FocusMenu);
		m_ChangeMenu = FOCUS_MENU_EMPT;
	}

	return false;
}

void ObjectMenu::Draw()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	// 메뉴 영역
	g_cSprite->DrawBackground(0xffffff, m_dX, m_dY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, false, true);
	
	// 메뉴 내부 오브젝트 처리
	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		if (ObjectTemp->Action()) {
			delete ObjectTemp;
		}
		ObjectTemp->Draw();
		++iter;
	}
}

bool ObjectMenu::Click()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;
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

		iter = m_MenuObjectList->rbegin();
		iter_end = m_MenuObjectList->head();
		while (iter != iter_end) {
			ObjectTemp = *iter;
			if (ObjectTemp->Click()) {
				return true;
			}
			--iter;
		}
		return true;
	}

	return false;
}

void ObjectMenu::Move(int iMoveValueX, int iMoveValueY)
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	m_dX = m_dX - iMoveValueX;
	m_dY = m_dY - iMoveValueY;
	m_dLeft = m_dLeft - iMoveValueX;
	m_dTop = m_dTop - iMoveValueY;
	m_dRight = m_dRight - iMoveValueX;
	m_dBottom = m_dBottom - iMoveValueY;

	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		ObjectTemp->Move(iMoveValueX, iMoveValueY);
		++iter;
	}
}

bool ObjectMenu::CheckOnMouse()
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

		return true;
	}

	return false;
}
