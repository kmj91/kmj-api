#include "stdafx.h"
#include "ObjectSprite.h"


ObjectSprite::ObjectSprite(int iSpriteIndex, int iPosX, int iPosY, bool bCameraOption)
{
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
	m_iRenderPriority = 0;

	m_bCameraOption = bCameraOption;
}


ObjectSprite::~ObjectSprite()
{
}

bool ObjectSprite::Action()
{
	return false;
}

void ObjectSprite::Draw()
{
	g_cSprite->DrawSprite(m_iSpriteIndex, m_dX, m_dY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, m_bCameraOption);
}

bool ObjectSprite::Click()
{
	return false;
}
