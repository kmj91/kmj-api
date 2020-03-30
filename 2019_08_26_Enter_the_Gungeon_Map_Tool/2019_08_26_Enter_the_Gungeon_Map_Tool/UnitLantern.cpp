#include "stdafx.h"
#include "UnitLantern.h"
#include "ObjectLight.h"


UnitLantern::UnitLantern(st_SpriteInfo stSpritInfo, int iUnitType, int iPosX, int iPosY, int iIncreaseValue)
{
	int iLightSprite;
	int iLightX;
	int iLightY;

	// 타입
	m_iType = df_TYPE_OBJECT;
	m_iUnitType = iUnitType;

	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = stSpritInfo.iStartSprite;
	// 가로, 세로 길이
	m_iWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth * iIncreaseValue;
	m_iHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight * iIncreaseValue;
	// 오브젝트 범위
	m_iLeft = m_iX - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX * iIncreaseValue);
	m_iTop = m_iY - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY * iIncreaseValue);
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = iPosY;
	// 곱셈 값
	m_iIncreaseValue = iIncreaseValue;
	// 지우기 플래그
	m_bEraseFlag = false;
	// 선택 플래그
	m_bSelectFlag = false;
	// 시작 스프라이트
	m_iStartSprite = stSpritInfo.iStartSprite;
	m_iEndSprite = stSpritInfo.iEndSprite;
	m_iAnimeDelay = stSpritInfo.iDelay;
	m_iAnimeCount = 0;

	iLightX = m_iX + ((g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth / 2) * iIncreaseValue);
	iLightY = m_iY + ((g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight / 2) * iIncreaseValue);

	// 가운데 랜턴 빛
	if (m_iSpriteIndex == df_SPRITE_OBJECT_LANTERN_CENTER_01) {
		iLightSprite = df_SPRITE_LIGHT_LANTERN_CENTER;
	}
	// 왼쪽 랜턴 빛
	else if (m_iSpriteIndex == df_SPRITE_OBJECT_LANTERN_LEFT_01) {
		iLightSprite = df_SPRITE_LIGHT_LANTERN_CENTER;
	}
	// 오른쪽 랜턴 빛
	else if (m_iSpriteIndex == df_SPRITE_OBJECT_LANTERN_RIGHT_01) {
		iLightSprite = df_SPRITE_LIGHT_LANTERN_CENTER;
	}

	//----------------------------------------
	// 여기서 메모리를 생성하지만
	// 삭제는 g_ObjectLightDib가 관리할것임
	//----------------------------------------
	// 랜턴 빛
	m_ObjectLightPtr = new ObjectLight(iLightSprite, iLightX, iLightY, 4);
	g_ObjectLightList->push_back(m_ObjectLightPtr);
}


UnitLantern::~UnitLantern()
{
}

bool UnitLantern::Action()
{
	// 지우기 옵션
	if (m_bEraseFlag) {
		// 빛, 조명을 삭제 후 랜턴을 삭제 한다
		g_ObjectLightDib->DeleteLight(m_ObjectLightPtr);
		return true;
	}

	if (m_iAnimeCount < m_iAnimeDelay) {
		++m_iAnimeCount;
	}
	else {
		m_iAnimeCount = 0;
		++m_iSpriteIndex;
		if (m_iSpriteIndex > m_iEndSprite) {
			m_iSpriteIndex = m_iStartSprite;
		}
	}

	return false;
}

void UnitLantern::Draw()
{
	g_cSprite->DrawIncreaseSprite(m_iSpriteIndex, m_iX, m_iY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_iIncreaseValue);

	// 선택되어있나?
	if (m_bSelectFlag) {
		g_cSprite->DrawRectangleBorder(0x1e96c8, 4, m_iX, m_iY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, true);
	}
}

bool UnitLantern::Click()
{
	int iMouseX;
	int iMouseY;

	// 클릭 가능 상태가 아니면
	if (g_iMouseState != df_MOUSE_STATE_EMPT && g_iMouseState != df_MOUSE_STATE_SELECT_UNIT) {
		return false;
	}


	//--------------------------------------
	// 전체 맵의 범위
	//--------------------------------------
	iMouseX = g_iMapMouseX;
	iMouseY = g_iMapMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		// 선택된 유닛이 있나?
		if (g_iMouseState == df_MOUSE_STATE_SELECT_UNIT) {
			// 선택된 유닛 해제
			g_SelectedObjectUnit->m_bSelectFlag = false;
		}
		else {
			// 마우스 상태 변경
			g_iMouseState = df_MOUSE_STATE_SELECT_UNIT;
		}
		
		// 선택됨
		m_bSelectFlag = true;
		// 내가 선택됨 내 포인터 저장
		g_SelectedObjectUnit = this;

		return true;
	}

	return false;
}