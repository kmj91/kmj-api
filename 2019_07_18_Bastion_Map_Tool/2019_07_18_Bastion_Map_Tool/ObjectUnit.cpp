#include "stdafx.h"
#include "ObjectUnit.h"


ObjectUnit::ObjectUnit(int iType, int iSpriteIndex, int iPosX, int iPosY)
{
	// 타입
	m_iType = iType;
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = iSpriteIndex;
	// 오브젝트 범위
	m_dLeft = m_dX - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_dTop = m_dY - g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_dRight = m_dLeft + g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
	m_dBottom = m_dTop + g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
	// 그리기 우선순위
	m_iRenderPriority = iPosY;
	// 지우기 플래그
	m_bEraseFlag = false;
}


ObjectUnit::~ObjectUnit()
{
}

bool ObjectUnit::Action()
{
	// 지우기 옵션
	if (m_bEraseFlag) {
		return true;
	}

	return false;
}

void ObjectUnit::Draw()
{
	g_cSprite->DrawSprite(m_iSpriteIndex, m_dX, m_dY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

bool ObjectUnit::Click()
{
	return false;
}

bool ObjectUnit::Erase(int iEraseX, int iEraseY)
{
	if (m_dLeft < iEraseX && iEraseX < m_dRight &&
		m_dTop < iEraseY && iEraseY < m_dBottom) {

		m_bEraseFlag = true;

		return true;
	}

	return false;
}
