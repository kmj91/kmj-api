#include "stdafx.h"
#include "ObjectPicture.h"


ObjectPicture::ObjectPicture(int iPosX, int iPosY, int iSpriteIndex, bool bOption)
{
	m_ComprehensiveType = df_COMPREHENSIVE_TYPE_NEUTRAL;
	m_Type = df_TYPE_OBJECT_PICTURE;
	m_X = iPosX;
	m_Y = iPosY;

	m_iSpriteIndex = iSpriteIndex;
	m_bOption = bOption;
	m_bDrawFlag = true;
}


ObjectPicture::~ObjectPicture()
{
}

bool ObjectPicture::Action()
{
	return false;
}

void ObjectPicture::Draw()
{
	if (m_bDrawFlag) {
		
		if (m_bOption) {
			// 합성 출력
			g_cSprite.DrawSpriteCompose(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
		}
		else {
			// 일반 출력
			g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
		}
		
	}
}
