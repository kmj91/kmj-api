#include "stdafx.h"
#include "ObjectBullet.h"


ObjectBullet::ObjectBullet()
{
}

ObjectBullet::ObjectBullet(int iType, int iPosX, int iPosY, float fAngle, float fSpeed,
	int iStartIndex, int iEndIndex, bool bOption)
{
	if (bOption) {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_PLAYER_BULLET;
	}
	else {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_ENEMY_BULLET;
	}
	
	m_Type = iType;
	m_X = iPosX;
	m_Y = iPosY;
	m_HP = 1;

	m_iSpriteIndex = iStartIndex;
	m_iAnimeStart = iStartIndex;
	m_iAnimeEnd = iEndIndex;
	m_iAnimeCount = 0;
	m_fSpeed = fSpeed;

	m_fRadian = PI * fAngle / 180;
	m_bShotArrayPtr = NULL;
	m_bOption = bOption;

	m_bReleaseFlag = false;

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;
}

ObjectBullet::ObjectBullet(int iType, int iPosX, int iPosY, float fAngle, float fSpeed,
	int iStartIndex, int iEndIndex, Bullet * bShotArrayPtr, bool bOption)
{
	if (bOption) {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_PLAYER_BULLET;
	}
	else {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_ENEMY_BULLET;
	}
	m_Type = iType;
	m_X = iPosX;
	m_Y = iPosY;
	m_HP = 1;

	m_iSpriteIndex = iStartIndex;
	m_iAnimeStart = iStartIndex;
	m_iAnimeEnd = iEndIndex;
	m_iAnimeCount = 0;
	m_fSpeed = fSpeed;

	m_fRadian = PI * fAngle / 180;
	m_bShotArrayPtr = bShotArrayPtr;
	(*m_bShotArrayPtr).flag = true;
	++(*m_bShotArrayPtr).count;
	m_bOption = bOption;

	m_bReleaseFlag = false;

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;

	switch (iType) {
	case df_TYPE_OBJECT_ALUCARD_BULLET_1:
		// 피격 판정
		m_fLeft = m_fLeft + 24;
		m_fRight = m_fRight - 23;
		m_fTop = m_fTop + 49;
		m_fBottom = m_fBottom;
		break;
	case df_TYPE_OBJECT_ALUCARD_BULLET_2:
		// 피격 판정
		m_fLeft = m_fLeft;
		m_fRight = m_fRight;
		m_fTop = m_fTop + 49;
		m_fBottom = m_fBottom;
		break;
	case df_TYPE_OBJECT_ALUCARD_BULLET_3:
		// 피격 판정
		m_fLeft = m_fLeft + 23;
		m_fRight = m_fRight - 24;
		m_fTop = m_fTop + 44;
		m_fBottom = m_fBottom;
		break;
	}

	
}


ObjectBullet::~ObjectBullet()
{
}

bool ObjectBullet::Action()
{
	float vx;
	float vy;
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;


	if (m_bReleaseFlag) {
		// 좀더 입체감이 느껴짐
		m_Y = m_Y + df_SPEED_SCROLL + 2;

		if (Animation(df_DELAY_BOOM)) {
			return true;
		}
		return false;
	}

	// 폭탄에 제거됨
	if (m_HP <= 0) {
		m_bReleaseFlag = true;

		m_iSpriteIndex = df_SPRITE_ENEMY_BULLET_RELEASE_1_1;
		m_iAnimeStart = df_SPRITE_ENEMY_BULLET_RELEASE_1_1;
		m_iAnimeEnd = df_SPRITE_ENEMY_BULLET_RELEASE_1_5;
		m_iAnimeCount = 0;

		m_fLeft = -100;
		m_fRight = -100;
		m_fTop = -100;
		m_fBottom = -100;
		return false;
	}

	// 아군 총알
	if (m_bOption) {
		vx = cosf(m_fRadian) * m_fSpeed;
		vy = -sinf(m_fRadian) * m_fSpeed;
		m_X = m_X + vx;
		m_Y = m_Y + vy;
		m_fLeft = m_fLeft + vx;
		m_fRight = m_fRight + vx;
		m_fTop = m_fTop + vy;
		m_fBottom = m_fBottom + vy;

		if (m_Y < df_GAME_TOP) {
			--(*m_bShotArrayPtr).count;
			if ((*m_bShotArrayPtr).count == 0) {
				(*m_bShotArrayPtr).flag = false;
			}
			return true;
		}
		if (m_Y > df_GAME_BOTTOM) {
			--(*m_bShotArrayPtr).count;
			if ((*m_bShotArrayPtr).count == 0) {
				(*m_bShotArrayPtr).flag = false;
			}
			return true;
		}
		if (m_X < df_GAME_LEFT) {
			--(*m_bShotArrayPtr).count;
			if ((*m_bShotArrayPtr).count == 0) {
				(*m_bShotArrayPtr).flag = false;
			}
			return true;
		}
		if (m_X > df_GAME_RIGHT) {
			--(*m_bShotArrayPtr).count;
			if ((*m_bShotArrayPtr).count == 0) {
				(*m_bShotArrayPtr).flag = false;
			}
			return true;
		}

		// 아군 총알 충돌 처리
		iter = g_ObjectList->begin();
		iter_end = g_ObjectList->end();
		while (iter != iter_end) {
			ObjectTemp = *iter;
			if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY) {
				if (ObjectTemp->m_bInFlag) {
					if (ObjectTemp->m_fLeft < m_fRight && ObjectTemp->m_fRight > m_fLeft &&
						ObjectTemp->m_fTop < m_fBottom && ObjectTemp->m_fBottom > m_fTop) {
						--ObjectTemp->m_HP;
						--(*m_bShotArrayPtr).count;
						if ((*m_bShotArrayPtr).count == 0) {
							(*m_bShotArrayPtr).flag = false;
						}
						return true;
					}
				}
			}
			++iter;
		}

	}
	// 적군 총알
	else {
		vx = cosf(m_fRadian) * m_fSpeed;
		vy = -sinf(m_fRadian) * m_fSpeed;
		m_X = m_X + vx;
		m_Y = m_Y + vy;
		m_fLeft = m_fLeft + vx;
		m_fRight = m_fRight + vx;
		m_fTop = m_fTop + vy;
		m_fBottom = m_fBottom + vy;

		if (m_Y < df_GAME_TOP) {
			return true;
			//return false;
		}
		if (m_Y > df_GAME_BOTTOM) {
			return true;
			//return false;
		}
		if (m_X < df_GAME_LEFT) {
			return true;
			//return false;
		}
		if (m_X > df_GAME_RIGHT) {
			return true;
			//return false;
		}

		if (g_Player != NULL) {
			if (g_Player->m_bInvincible != true) {
				// 적 총알 충돌 처리
				if (g_Player->m_fLeft < m_fRight && g_Player->m_fRight > m_fLeft &&
					g_Player->m_fTop < m_fBottom && g_Player->m_fBottom > m_fTop) {
					--g_Player->m_HP;
					return true;
				}
			}
		}
	}

	


	// 애니메이션 처리
	if (Animation(df_DELAY_BULLET)) {
		m_iSpriteIndex = m_iAnimeStart;
	}


	return false;
}

void ObjectBullet::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

void ObjectBullet::Init(int iType, int iPosX, int iPosY, float fAngle, float fSpeed, int iStartIndex, int iEndIndex, bool bOption)
{
	if (bOption) {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_PLAYER_BULLET;
	}
	else {
		m_ComprehensiveType = df_COMPREHENSIVE_TYPE_ENEMY_BULLET;
	}
	m_Type = iType;
	m_X = iPosX;
	m_Y = iPosY;
	m_HP = 1;

	m_iSpriteIndex = iStartIndex;
	m_iAnimeStart = iStartIndex;
	m_iAnimeEnd = iEndIndex;
	m_iAnimeCount = 0;
	m_fSpeed = fSpeed;

	m_fRadian = PI * fAngle / 180;
	m_bShotArrayPtr = NULL;
	m_bOption = bOption;

	m_bReleaseFlag = false;

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;
}

//------------------------------------
// 애니메이션 처리
// iAnimeDelay : 프레임 딜레이 값
//------------------------------------
bool ObjectBullet::Animation(int iAnimeDelay)
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
