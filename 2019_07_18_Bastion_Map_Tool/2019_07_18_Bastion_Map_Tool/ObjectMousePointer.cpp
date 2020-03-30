#include "stdafx.h"
#include "ObjectMousePointer.h"


ObjectMousePointer::ObjectMousePointer(int iPosX, int iPosY)
{
	// 타입
	m_iType = df_TYPE_UI;
	// 위치
	m_dX = iPosX;
	m_dY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = df_SPRITE_EMPT_SPRITE;
	// 그리기 우선순위
	m_iRenderPriority = df_RENDER_PRIORITY_LAST;
	// 빨간색 그리기 옵션
	bRedOption = false;
}


ObjectMousePointer::~ObjectMousePointer()
{
}

bool ObjectMousePointer::Action()
{
	return false;
}

void ObjectMousePointer::Draw()
{
	int iCenterX;
	int iCenterY;

	if (!g_bOnUIMouse) {
		g_cSprite->DrawSprite(m_iSpriteIndex, m_dX, m_dY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
		if (bRedOption) {
			// 스프라이트 크기
			m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
			m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
			iCenterX = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
			iCenterY = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
			g_cSprite->DrawBackground(0xff0000, m_dX - iCenterX, m_dY - iCenterY, m_iSpriteWidth, m_iSpriteHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, true, true);
		}
		else {
			// 스프라이트 크기
			m_iSpriteWidth = g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth;
			m_iSpriteHeight = g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight;
			iCenterX = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX;
			iCenterY = g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY;
			g_cSprite->DrawBackground(0x7ffd4, m_dX - iCenterX, m_dY - iCenterY, m_iSpriteWidth, m_iSpriteHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, true, true);
		}
	}
	
}

bool ObjectMousePointer::Click()
{
	return false;
}
