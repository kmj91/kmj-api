#include "stdafx.h"
#include "UITriggerElement.h"
#include "UIMenu.h"


UITriggerElement::UITriggerElement(int * iScrollTopPtr, int * iScrollBottomPtr, st_UITriggerElementData * st_ElementData, UIMenu * MenuPtr)
{
	// 위치
	m_iX = 0;
	m_iY = 0;
	// 스프라이트 번호
	m_iSpriteIndex = df_SPRITE_EMPT_SPRITE;
	// 가로 세로 길이
	m_iWidth = 0;
	m_iHeight = 0;
	// 오브젝트 범위
	m_iLeft = 0;
	m_iTop = 0;
	m_iRight = 0;
	m_iBottom = 0;
	// 그리기 우선순위
	m_iRenderPriority = 0;
	// 리스트 스크롤 범위
	m_iScrollTopPtr = iScrollTopPtr;
	m_iScrollBottomPtr = iScrollBottomPtr;
	// 트리거 데이터 포인터
	m_stTriggerElementData = st_ElementData;
	// 메뉴 포인터
	m_MenuPtr = MenuPtr;
	// 요소 선택 플래그
	m_bSelectElement = false;


}


UITriggerElement::~UITriggerElement()
{
	// 데이터 정리
	delete m_stTriggerElementData;
}

bool UITriggerElement::Action()
{
	return false;
}

void UITriggerElement::Draw()
{
	int iPosY;
	RECT SpriteCutOutRect;
	SpriteCutOutRect.left = 0;
	SpriteCutOutRect.top = 0;
	SpriteCutOutRect.right = 0;
	SpriteCutOutRect.bottom = 0;
	iPosY = m_iY;

	//--------------------------------------
	// 목록 요소는 메뉴 사이즈에 종속적임
	// 스크롤은 높이만 존재함
	//--------------------------------------

	// 메뉴를 벗어나면 벗어나는 만큼 스프라이트를 잘라낸다
	// 위
	if (m_iTop < *m_iScrollTopPtr) {
		SpriteCutOutRect.top = *m_iScrollTopPtr - m_iTop;
		//--------------------------------------
		// 위쪽 스프라이트를 잘라낼 경우
		// 스프라이트 출력 위치를 잘라낸 좌표부터 해야되기 때문에 
		// TOP값을 시작 위치로 지정해준다 (m_iScrollTopPtr)
		//--------------------------------------
		iPosY = *m_iScrollTopPtr;
	}

	// 아래
	if (m_iBottom > *m_iScrollBottomPtr) {
		SpriteCutOutRect.bottom = m_iBottom - *m_iScrollBottomPtr;
	}


}

bool UITriggerElement::Click()
{
	int iMouseX;
	int iMouseY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	// 카메라 좌표를 빼준다
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		//------------------------------------
		// 편집, 삭제 버튼이 존재 한다
		//------------------------------------


		m_bSelectElement = true;

		return true;
	}

	return false;
}

void UITriggerElement::Move(int iMoveValueX, int iMoveValueY)
{
}

void UITriggerElement::SetPosition(int iPosX, int iPosY)
{
}
