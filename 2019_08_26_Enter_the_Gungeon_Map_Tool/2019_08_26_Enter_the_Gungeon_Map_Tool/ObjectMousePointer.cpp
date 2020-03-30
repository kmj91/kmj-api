#include "stdafx.h"
#include "ObjectMousePointer.h"


ObjectMousePointer::ObjectMousePointer(int iPosX, int iPosY)
{
	// 타입
	m_iType = df_TYPE_UI;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = df_SPRITE_EMPT_SPRITE;
	m_iStartSprite = df_SPRITE_EMPT_SPRITE;
	m_iEndSprite = df_SPRITE_EMPT_SPRITE;
	m_iAnimeDelay = 0;
	m_iAnimeCount = 0;
	// 그리기 우선순위
	m_iRenderPriority = df_RENDER_PRIORITY_LAST;
	// 스프라이트 증가 값
	m_iIncreaseVlaue = 1;
	// 빨간색 그리기 옵션
	m_bRedOption = false;
	// 파란색
	m_bBlueOption = false;
}


ObjectMousePointer::~ObjectMousePointer()
{
}

bool ObjectMousePointer::Action()
{
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

void ObjectMousePointer::Draw()
{
	int iCenterX;
	int iCenterY;

	if (m_iSpriteIndex == df_SPRITE_EMPT_SPRITE) {
		return;
	}

	if (!m_bOnUIMouse) {
		g_cSprite->DrawIncreaseSprite(m_iSpriteIndex, m_iX, m_iY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_iIncreaseVlaue);
		if (m_bRedOption) {
			// 스프라이트 크기
			m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
			m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
			iCenterX = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
			iCenterY = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
			g_cSprite->DrawBackground(0xff0000, m_iX - iCenterX, m_iY - iCenterY, m_iSpriteWidth * m_iIncreaseVlaue,
				m_iSpriteHeight * m_iIncreaseVlaue, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, true, true);
		}
		if (m_bBlueOption) {
			// 스프라이트 크기
			m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
			m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
			iCenterX = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
			iCenterY = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
			g_cSprite->DrawBackground(0x7ffd4, m_iX - iCenterX, m_iY - iCenterY, m_iSpriteWidth * m_iIncreaseVlaue,
				m_iSpriteHeight * m_iIncreaseVlaue, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, true, true);
		}
	}
}

bool ObjectMousePointer::Click()
{
	return false;
}
