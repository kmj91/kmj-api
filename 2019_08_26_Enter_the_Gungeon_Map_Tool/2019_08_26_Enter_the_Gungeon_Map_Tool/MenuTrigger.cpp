#include "stdafx.h"
#include "MenuTrigger.h"
#include "UITitleBar.h"
#include "UIButton.h"
#include "ListTrigger.h"
#include "UITriggerElement.h"


MenuTrigger::MenuTrigger(int iPosX, int iPosY, int iWidth, int iHeight)
{
	ObjectUI * newObject;
	vector<ObjectUI *> * ElementVector;				// 요소 벡터
	UIScrollBar * ScrollBar;						// 스크롤 바
	st_UIElementInfo stElementInfo;					// Element 리스트 정보 구조체 ( 리스트 구성 객체 정보 )
	st_UITriggerElementData * stElementData;		// Element 데이터 ( 조건 + 액션)
	st_Trigger stTrigger;							// 트리거

	// 타입
	m_iType = df_TYPE_UI;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = df_RENDER_PRIORITY_LAST;
	// 선택된 요소
	m_SelectElement = NULL;
	// 메뉴 보이기 끄기
	m_bOpenMenu = false;

	//-------------------------------------
	// 메뉴 오브젝트 리스트
	//-------------------------------------
	m_MenuObjectList = new CList<ObjectUI *>;

	//-------------------------------------
	// 타이틀 바
	//-------------------------------------
	newObject = new UITitleBar(0xff69c3e2, m_iX, m_iY, m_iWidth, 40, 0.8, this);
	m_MenuObjectList->push_back(newObject);

	//-------------------------------------
	// 메뉴
	// 디폴트 선택
	//-------------------------------------
	m_iFocusMenu = df_FOCUS_TRIGGER_MENU_TRIGGER_LIST;
	m_iChangeMenu = df_FOCUS_TRIGGER_MENU_EMPT;

	//-------------------------------------
	// 버튼
	//-------------------------------------
	// 새로 만들기
	newObject = new UIButton(L"새로 만들기", m_iX + 5, m_iY + 45, 120, 40, df_FOCUS_TRIGGER_MENU_CREATE_TRIGGER, this);
	m_MenuObjectList->push_back(newObject);
	// 편집
	newObject = new UIButton(L"편집", newObject->m_iRight + 5, m_iY + 45, 60, 40, df_FOCUS_TRIGGER_MENU_EDIT_TRIGGER, this);
	m_MenuObjectList->push_back(newObject);
	// 삭제
	newObject = new UIButton(L"삭제", newObject->m_iRight + 5, m_iY + 45, 60, 40, df_FOCUS_TRIGGER_MENU_DELETE_TRIGGER, this);
	m_MenuObjectList->push_back(newObject);


	//-------------------------------------
	// UI 요소 리스트 클래스
	//-------------------------------------
	m_MenuElementList = new ListTrigger(m_iX, m_iY + 80, m_iWidth, m_iHeight - 80, 25);
	m_MenuObjectList->push_back(m_MenuElementList);

	//-------------------------------------
	// UI 클래스에 리스트 추가
	//-------------------------------------
	//-------------------------------------
	// 트리거 메뉴 요소들
	//-------------------------------------
	// 여기서 메모리 생성을 했지만 메모리 해제는 UI 리스트 클래스 에서 해제
	ElementVector = new vector<ObjectUI *>;
	stElementData = new st_UITriggerElementData;
	// 트리거 리스트에 들어갈 기본 요소 생성
	// 조건
	// 무조건, 토큰 없음
	stTrigger.iTriggerType = df_TRIGGER_TYPE_CONDITIONS_ALWAYS;
	stElementData->TriggerConditionsVector.push_back(stTrigger);
	// 액션
	// 문 닫기, 토큰 없음
	stTrigger.iTriggerType = df_TRIGGER_TYPE_ACTIONS_CLOSE_DOOR;
	stElementData->TriggerActionsVector.push_back(stTrigger);
	// 트리거 리스트 구성 객체 생성
	newObject = new UITriggerElement(&m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, stElementData, this);
	ElementVector->push_back(newObject);
	// 스크롤 바 생성
	//위치 좌표 X, Y, 너비, 높이, 리스트 클래스 포인터
	ScrollBar = new UIScrollBar(m_MenuElementList->m_iRight - 25, m_MenuElementList->m_iTop, 25, m_MenuElementList->m_iElementListHeight, m_MenuElementList);
	// 구조체 값 셋
	stElementInfo.iListType = df_FOCUS_TRIGGER_MENU_TRIGGER_LIST;
	stElementInfo.ScrollBarPtr = ScrollBar;
	stElementInfo.ElementObjectVectorPtr = ElementVector;
	// 리스트 추가
	m_MenuElementList->AddElementListPtr(stElementInfo);


	// 리스트 요소 갱신
	m_MenuElementList->ChangeElementList(m_iFocusMenu);
}


MenuTrigger::~MenuTrigger()
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

bool MenuTrigger::Action()
{
	if (m_iChangeMenu != df_FOCUS_MENU_EMPT) {
		m_iFocusMenu = m_iChangeMenu;
		// 리스트 요소 갱신
		m_MenuElementList->ChangeElementList(m_iFocusMenu);
		m_iChangeMenu = df_FOCUS_MENU_EMPT;
	}

	return false;
}

void MenuTrigger::Draw()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	// 오픈 상태가 아니면 나감
	if (!m_bOpenMenu) {
		return;
	}

	// 메뉴 영역
	g_cSprite->DrawBackground(0xffffff, m_iX, m_iY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, false, true);

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

bool MenuTrigger::Click()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;
	int iMouseX;
	int iMouseY;

	// 오픈 상태가 아니면 나감
	if (!m_bOpenMenu) {
		return false;
	}

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

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

void MenuTrigger::Move(int iMoveValueX, int iMoveValueY)
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;

	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		ObjectTemp->Move(iMoveValueX, iMoveValueY);
		++iter;
	}
}

bool MenuTrigger::CheckOnMouse()
{
	int iMouseX;
	int iMouseY;

	// 오픈 상태가 아니면 나감
	if (!m_bOpenMenu) {
		return false;
	}

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		return true;
	}

	return false;
}
