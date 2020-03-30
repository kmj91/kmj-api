#include "stdafx.h"
#include "ObjectEffect.h"


ObjectEffect::ObjectEffect(int iType, int iPosX, int iPosY)
{
	m_ComprehensiveType = df_COMPREHENSIVE_TYPE_NEUTRAL;
	m_Type = iType;
	m_X = iPosX;
	m_Y = iPosY;

	switch (m_Type) {
	case df_TYPE_OBJECT_EFFECT_KABOOM_1:
		m_iSpriteIndex = df_SPRITE_ITEM_KABOOM_1_1;
		m_iAnimeStart = df_SPRITE_ITEM_KABOOM_1_1;
		m_iAnimeEnd = df_SPRITE_ITEM_KABOOM_1_9;
		m_iAnimeCount = 0;
		break;
	}
}


ObjectEffect::~ObjectEffect()
{
}

bool ObjectEffect::Action()
{
	//m_Y = m_Y + df_SPEED_SCROLL;
	
	// 좀더 입체감이 느껴짐
	m_Y = m_Y + df_SPEED_SCROLL + 4;

	if (Animation(df_DELAY_EFFECT)) {
		return true;
	}

	return false;
}

void ObjectEffect::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

//------------------------------------
// 애니메이션 처리
// iAnimeDelay : 프레임 딜레이 값
//------------------------------------
bool ObjectEffect::Animation(int iAnimeDelay)
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
