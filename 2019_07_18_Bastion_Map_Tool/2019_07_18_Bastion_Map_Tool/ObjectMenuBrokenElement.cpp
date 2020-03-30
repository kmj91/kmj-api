#include "stdafx.h"
#include "ObjectMenuBrokenElement.h"


ObjectMenuBrokenElement::ObjectMenuBrokenElement(int iSpriteIndex, int iPosX, int iPosY, int * iScrollTopPtr, int * iScrollBottomPtr)
{
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = iSpriteIndex;
	// 스프라이트 길이
	m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
	m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
	// 센터 포인터
	// 해당 요소 스프라이트 인덱스에 -100을 뺀 값이 실제 출력할 타일 스프라이트 번호 임
	m_iPropertiesCenterPointX = g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iCenterPointX / df_TILE_PROPERTIES_SIZE;
	m_iPropertiesCenterPointY = g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iCenterPointY / df_TILE_PROPERTIES_SIZE;
	// 속성 크기
	m_iPropertiesSizeX = g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iWidth / df_TILE_PROPERTIES_SIZE;
	// 소수점이 있다면 +1
	if (g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iWidth % df_TILE_PROPERTIES_SIZE) {
		++m_iPropertiesSizeX;
	}
	m_iPropertiesSizeY = g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iHeight / df_TILE_PROPERTIES_SIZE;
	if (g_cSprite->m_stpSprite[m_iSpriteIndex - 100].iHeight % df_TILE_PROPERTIES_SIZE) {
		++m_iPropertiesSizeY;
	}
	// 오브젝트 범위
	m_dLeft = m_dX - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_dTop = m_dY - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_dRight = m_dLeft + m_iSpriteWidth;
	m_dBottom = m_dTop + m_iSpriteHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;
	// 리스트 스크롤 범위
	m_iScrollTopPtr = iScrollTopPtr;
	m_iScrollBottomPtr = iScrollBottomPtr;
}


ObjectMenuBrokenElement::~ObjectMenuBrokenElement()
{
}

bool ObjectMenuBrokenElement::Action()
{
	return false;
}

void ObjectMenuBrokenElement::Draw()
{
	int iPosY;
	RECT SpriteCutOutRect;
	SpriteCutOutRect.left = 0;
	SpriteCutOutRect.top = 0;
	SpriteCutOutRect.right = 0;
	SpriteCutOutRect.bottom = 0;
	iPosY = m_dY;

	//--------------------------------------
	// 목록 요소는 메뉴 사이즈에 종속적임
	// 스크롤은 높이만 존재함
	//--------------------------------------

	// 메뉴를 벗어나면 벗어나는 만큼 스프라이트를 잘라낸다
	// 위
	if (m_dTop < *m_iScrollTopPtr) {
		SpriteCutOutRect.top = *m_iScrollTopPtr - m_dTop;
		//--------------------------------------
		// 위쪽 스프라이트를 잘라낼 경우
		// 스프라이트 출력 위치를 잘라낸 좌표부터 해야되기 때문에 
		// TOP값을 시작 위치로 지정해준다 (m_iScrollTopPtr)
		//--------------------------------------
		iPosY = *m_iScrollTopPtr;
	}

	// 아래
	if (m_dBottom > *m_iScrollBottomPtr) {
		SpriteCutOutRect.bottom = m_dBottom - *m_iScrollBottomPtr;
	}

	g_cSprite->DrawCutOutImage(m_iSpriteIndex, m_dX, iPosY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, SpriteCutOutRect, false);
}

bool ObjectMenuBrokenElement::Click()
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

		g_MouseState = MOUSE_STATE_BROKEN_MAKE;
		g_MousePointer->m_iSpriteIndex = m_iSpriteIndex - 100;

		switch (m_iSpriteIndex) {
		case df_SPRITE_MENU_BROKEN_27:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_27;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_28:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_28;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_29:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_29;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_30:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_30;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_31:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_31;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_32:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_32;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_33:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_33;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_34:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_34;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_35:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_35;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_36:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_36;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_37:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_37;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_38:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_38;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_39:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_39;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_40:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_40;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		case df_SPRITE_MENU_BROKEN_41:
			g_iPropertiesCenterPointX = m_iPropertiesCenterPointX;
			g_iPropertiesCenterPointY = m_iPropertiesCenterPointY;
			g_bypArrayMAkeTileProperties = g_byArrayMakePropertiesBroken_41;
			g_iMakePropertiesSizeX = m_iPropertiesSizeX;
			g_iMakePropertiesSizeY = m_iPropertiesSizeY;
			g_byCheckMakeProperties = 3;
			g_byCheckMapProperties = 0;
			break;
		default:
			// 없음
			throw;
		}

		g_iMakeTileIndex = m_iSpriteIndex - 100;
		return true;
	}

	return false;
}

void ObjectMenuBrokenElement::Move(int iMoveValueX, int iMoveValueY)
{
	m_dX = m_dX - iMoveValueX;
	m_dY = m_dY - iMoveValueY;
	m_dLeft = m_dLeft - iMoveValueX;
	m_dTop = m_dTop - iMoveValueY;
	m_dRight = m_dRight - iMoveValueX;
	m_dBottom = m_dBottom - iMoveValueY;
}

void ObjectMenuBrokenElement::SetPosition(int iPosX, int iPosY)
{
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 오브젝트 범위
	m_dLeft = m_dX - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_dTop = m_dY - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_dRight = m_dLeft + m_iSpriteWidth;
	m_dBottom = m_dTop + m_iSpriteHeight;
}
