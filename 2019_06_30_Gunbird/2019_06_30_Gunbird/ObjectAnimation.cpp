#include "stdafx.h"
#include "ObjectAnimation.h"


ObjectAnimation::ObjectAnimation(int iPosX, int iPosY, int iStartIndex, int iEndIndex, int iAnimeDelay)
{
	m_ComprehensiveType = df_COMPREHENSIVE_TYPE_NEUTRAL;
	m_Type = df_TYPE_OBJECT_PICTURE;
	m_X = iPosX;
	m_Y = iPosY;

	m_iSpriteIndex = iStartIndex;
	m_iAnimeStart = iStartIndex;
	m_iAnimeEnd = iEndIndex;
	m_iAnimeCount = 0;
	m_iAnimeDelay = iAnimeDelay;
}


ObjectAnimation::~ObjectAnimation()
{
}

bool ObjectAnimation::Action()
{
	// 애니메이션 처리
	if (Animation(m_iAnimeDelay)) {
		m_iSpriteIndex = m_iAnimeStart;
	}

	return false;
}

void ObjectAnimation::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

//------------------------------------
// 애니메이션 처리
// iAnimeDelay : 프레임 딜레이 값
//------------------------------------
bool ObjectAnimation::Animation(int iAnimeDelay)
{
	if (m_iAnimeCount < iAnimeDelay) {
		++m_iAnimeCount;
	}
	else {
		m_iAnimeCount = 0;
		++m_iSpriteIndex;
		if (m_iSpriteIndex > m_iAnimeEnd) {
			return true;
		}
	}
	return false;
}
