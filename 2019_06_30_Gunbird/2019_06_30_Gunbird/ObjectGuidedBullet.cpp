#include "stdafx.h"
#include "ObjectGuidedBullet.h"


ObjectGuidedBullet::ObjectGuidedBullet(int iType, int iPosX, int iPosY, float fAngle, bool bOption)
{
	int iAngle;

	m_Type = iType;
	m_X = iPosX;
	m_Y = iPosY;
	m_fRadian = PI * fAngle / 180;
	m_bOption = bOption;


	switch (iType)
	{
	case df_TYPE_OBJECT_ALUCARD_GUIDED_BULLET:
		iAngle = (fAngle / 15) * 15;

		if (fAngle < 10) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_0;
		}
		else if (fAngle < 35) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_15;
		}
		else if (fAngle < 55) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_45;
		}
		else if (fAngle < 65) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_60;
		}
		else if (fAngle < 85) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_75;
		}
		else if (fAngle < 95) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_90;
		}
		else if (fAngle < 115) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_105;
		}
		else if (fAngle < 125) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_120;
		}
		else if (fAngle < 145) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_135;
		}
		else if (fAngle < 170) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_165;
		}
		else if (fAngle < 190) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_180;
		}
		else if (fAngle < 215) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_195;
		}
		else if (fAngle < 235) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_225;
		}
		else if (fAngle < 260) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_255;
		}
		else if (fAngle < 280) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_270;
		}
		else if (fAngle < 290) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_285;
		}
		else if (fAngle < 310) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_300;
		}
		else if (fAngle < 320) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_315;
		}
		else if (fAngle < 350) {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_345;
		}
		else {
			m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_BULLET_0;
		}

		m_fSpeed = df_SPEED_ALUCARD_BULLET_3;

		m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
		m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
		m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
		m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;
		// 피격 판정
		m_fLeft = m_fLeft + 33;
		m_fRight = m_fRight - 33;
		m_fTop = m_fTop + 50;
		m_fBottom = m_fBottom - 16;
		break;
	}
}


ObjectGuidedBullet::~ObjectGuidedBullet()
{
}

bool ObjectGuidedBullet::Action()
{
	float vx;
	float vy;
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

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
			return true;
		}
		if (m_Y > df_GAME_BOTTOM) {
			return true;
		}
		if (m_X < df_GAME_LEFT) {
			return true;
		}
		if (m_X > df_GAME_RIGHT) {
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
						return true;
					}
				}
			}
			++iter;
		}
	}

	

	return false;
}

void ObjectGuidedBullet::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}
