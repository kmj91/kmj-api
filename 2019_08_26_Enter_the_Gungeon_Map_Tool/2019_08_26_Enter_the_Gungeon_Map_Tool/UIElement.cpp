#include "stdafx.h"
#include "UIElement.h"
#include "UIMenu.h"


UIElement::UIElement(int iSpriteIndex, int iPosX, int iPosY, int * iScrollTopPtr, int * iScrollBottomPtr,
	st_UITileUnitElementData * st_ElementData, UIMenu * MenuPtr, int iIncreaseValue)
{
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = iSpriteIndex;
	// 스프라이트 길이
	m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth * iIncreaseValue;
	m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight * iIncreaseValue;
	// 오브젝트 범위
	m_iLeft = m_iX - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX * iIncreaseValue);
	m_iTop = m_iY - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY * iIncreaseValue);
	m_iRight = m_iLeft + m_iSpriteWidth;
	m_iBottom = m_iTop + m_iSpriteHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;
	// 리스트 스크롤 범위
	m_iScrollTopPtr = iScrollTopPtr;
	m_iScrollBottomPtr = iScrollBottomPtr;
	// 메뉴 포인터
	m_MenuPtr = MenuPtr;
	// 곱셈 값
	m_iIncreaseValue = iIncreaseValue;
	// 요소 데이터
	m_iMouseState = st_ElementData->iMouseState;
	m_stMousePointerSprite = st_ElementData->stMousePointerSprite;
	m_iMouseSpriteIncreaseVlaue = st_ElementData->iMouseSpriteIncreaseVlaue;
	m_iElementType = st_ElementData->iElementType;
	m_iElementState = st_ElementData->iElementState;
	m_iElementSpriteIncreaseVlaue = st_ElementData->iElementSpriteIncreaseVlaue;
	// 요소 선택 플래그
	m_bSelectElement = false;
}


UIElement::~UIElement()
{
}

bool UIElement::Action()
{
	return false;
}

void UIElement::Draw()
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

	g_cSprite->DrawCutOutIncreaseImage(m_iSpriteIndex, m_iX, iPosY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_iIncreaseValue, SpriteCutOutRect, false);

	if (m_bSelectElement) {
		g_cSprite->DrawCutOutSprite(df_SPRITE_UI_MENU_ELEMENT_SELECT, m_iX, iPosY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, SpriteCutOutRect, false);
	}
}

bool UIElement::Click()
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

		//---------------------------------------------------------------------
		// 이 객체 요소가 가지고있는
		// 마우스 상태, 마우스 포인터 스프라이트, 타일 타입의 값을 셋팅 해준다
		//---------------------------------------------------------------------
		g_iMouseState = m_iMouseState;
		g_MousePointer->m_iSpriteIndex = m_stMousePointerSprite.iStartSprite;
		g_MousePointer->m_iStartSprite = m_stMousePointerSprite.iStartSprite;
		g_MousePointer->m_iEndSprite = m_stMousePointerSprite.iEndSprite;
		g_MousePointer->m_iAnimeDelay = m_stMousePointerSprite.iDelay;
		g_MousePointer->m_iIncreaseVlaue = m_iMouseSpriteIncreaseVlaue;
		g_iSelectedType = m_iElementType;
		g_iSelectedState = m_iElementState;
		g_iSelectedSpriteIncreaseVlaue = m_iElementSpriteIncreaseVlaue;
		
		// 내가 선택된걸로 바꿈
		if (m_MenuPtr->m_SelectElement != NULL) {
			m_MenuPtr->m_SelectElement->m_bSelectElement = false;
		}
		m_MenuPtr->m_SelectElement = this;
		m_bSelectElement = true;

		return true;
	}

	return false;
}

void UIElement::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;
}

void UIElement::SetPosition(int iPosX, int iPosY)
{
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 오브젝트 범위
	m_iLeft = m_iX - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX * m_iIncreaseValue);
	m_iTop = m_iY - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY * m_iIncreaseValue);
	m_iRight = m_iLeft + m_iSpriteWidth;
	m_iBottom = m_iTop + m_iSpriteHeight;
}
