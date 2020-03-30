#include "stdafx.h"
#include "ObjectItemPowerUp.h"


ObjectItemPowerUp::ObjectItemPowerUp(int iPosX, int iPosY, float fAngle)
{
	m_ComprehensiveType = df_COMPREHENSIVE_TYPE_ITEM;
	m_Type = df_TYPE_OBJECT_ITEM_POWER_UP;
	m_X = iPosX;
	m_Y = iPosY;
	m_iSpriteIndex = df_SPRITE_ITEM_POWER_UP_1;
	m_iAnimeStart = df_SPRITE_ITEM_POWER_UP_1;
	m_iAnimeEnd = df_SPRITE_ITEM_POWER_UP_7;
	m_iAnimeCount = 0;

	m_fRadian = PI * fAngle / 180;

	m_iReleaseCount = 50;
	m_bOnFlag = false;

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;
}


ObjectItemPowerUp::~ObjectItemPowerUp()
{
}

bool ObjectItemPowerUp::Action()
{
	float vx;
	float vy;
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	// 습득 됨
	if (m_bOnFlag) {
		m_Y = m_Y - 2;
		--m_iReleaseCount;
		if (m_iReleaseCount < 0) {
			return true;
		}

		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		return false;
	}

	vx = cosf(m_fRadian) * df_SPEED_ITEM;
	vy = -sinf(m_fRadian) * df_SPEED_ITEM;
	m_X = m_X + vx;
	m_Y = m_Y + vy;
	m_fLeft = m_fLeft + vx;
	m_fRight = m_fRight + vx;
	m_fTop = m_fTop + vy;
	m_fBottom = m_fBottom + vy;

	//---------------------------
	// 화면 밖을 나가지 않는다
	//---------------------------
	if (m_X < df_IN_GAME_LEFT) {
		m_fRadian = PI - m_fRadian;
	}
	if (m_X > df_IN_GAME_RIGHT) {
		m_fRadian = PI - m_fRadian;
	}
	if(m_Y < df_IN_GAME_TOP) {
		m_fRadian = 2 * PI - m_fRadian;
	}
	if (m_Y > df_IN_GAME_BOTTOM - 200) {
		m_fRadian = 2 * PI - m_fRadian;
	}

	
	if (g_Player != NULL) {
		// 충돌 처리
		if (g_Player->m_fLeft < m_fRight && g_Player->m_fRight > m_fLeft &&
			g_Player->m_fTop < m_fBottom && g_Player->m_fBottom > m_fTop) {
			if (g_Player->m_iShotLevel + 1 <= 3) {
				++g_Player->m_iShotLevel;
			}
			m_bOnFlag = true;
			m_iSpriteIndex = df_SPRITE_ITEM_POWER_UP_STRING_1;
			m_iAnimeStart = df_SPRITE_ITEM_POWER_UP_STRING_1;
			m_iAnimeEnd = df_SPRITE_ITEM_POWER_UP_STRING_2;
			m_iAnimeCount = 0;
		}
	}


	if (Animation(df_DELAY_ACTION)) {
		m_iSpriteIndex = m_iAnimeStart;
	}

	return false;
}

void ObjectItemPowerUp::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

//------------------------------------
// 애니메이션 처리
// iAnimeDelay : 프레임 딜레이 값
//------------------------------------
bool ObjectItemPowerUp::Animation(int iAnimeDelay)
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
