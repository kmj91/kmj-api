#include "stdafx.h"
#include "ObjectAlucard.h"
#include "ObjectBullet.h"
#include "ObjectGuidedBullet.h"


ObjectAlucard::ObjectAlucard()
{
	int iCnt;

	m_Type = df_TYPE_OBJECT_PLAYER;
	m_X = df_WIN_RIGHT / 4;
	m_Y = df_GAME_BOTTOM;
	m_HP = 1;

	m_iShotDelay = 0;
	m_iChargeDelay = 0;
	m_iChargeShotCnt = 0;

	m_fChargeAngle = -60;

	m_bSpecialAction = false;
	//m_bShot = false;
	m_bCharge = false;
	//m_bChargeShot = false;
	m_byEnergyShotGauge = 0;
	m_byShotGauge = 0;

	//m_bBoom = false;
	m_chLife = 2;					// 라이프

	m_iShotLevel = 0;				// 일반 공격 레벨
	m_bInvincible = true;			// 무적

	// 초기 키 상태 셋 및 스프라이트 셋팅
	m_State = STATE_RESPAWN;
	m_byKey = df_ACTION_EMPT;
	m_Action = ACTION_END;
	KeyAction(df_ACTION_STAND);
	m_Attack = ATTACK_EMPT;

	iCnt = 0;
	while (iCnt < 4) {
		m_bShotArray[iCnt].flag = false;
		m_bShotArray[iCnt].count = 0;
		++iCnt;
	}

	iCnt = 0;
	while (iCnt < 8) {
		m_bShotArray2[iCnt].flag = false;
		m_bShotArray2[iCnt].count = 0;
		++iCnt;
	}

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;

	// 피격 판정
	m_fLeft = m_fLeft + 38;
	m_fRight = m_fRight - 34;
	m_fTop = m_fTop + 15;
	m_fBottom = m_fBottom - 30;
}


ObjectAlucard::~ObjectAlucard()
{

}

bool ObjectAlucard::Action()
{
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	
	switch (m_State)
	{
	case STATE_RESPAWN:
		m_Y = m_Y - df_SPEED_ALUCARD_MOVE;
		m_fTop = m_fTop - df_SPEED_ALUCARD_MOVE;
		m_fBottom = m_fBottom - df_SPEED_ALUCARD_MOVE;

		if (m_Y < 750) {
			m_State = STATE_PLAY;
			m_bInvincible = false;		// 무적 끝
		}

		return false;
	case STATE_PLAY:
		break;
	case STATE_DIE:
		m_Y = m_Y + 10;
		if (m_Y > df_GAME_BOTTOM) {
			// 부활
			if (m_chLife - 1 >= 0) {
				--m_chLife;
				m_State = STATE_RESPAWN;
				// 초기 키 상태 셋 및 스프라이트 셋팅
				m_byKey = df_ACTION_EMPT;
				m_Action = ACTION_END;
				KeyAction(df_ACTION_STAND);
				m_Attack = ATTACK_EMPT;
				// 일반 공격 레벨
				m_iShotLevel = 0;
				// 좌표 및 체력
				m_X = df_WIN_RIGHT / 4;
				m_Y = df_GAME_BOTTOM;
				m_HP = 1;
				// 무적
				m_bInvincible = true;
				// 피격 판정
				m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
				m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
				m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
				m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;

				m_fLeft = m_fLeft + 38;
				m_fRight = m_fRight - 34;
				m_fTop = m_fTop + 15;
				m_fBottom = m_fBottom - 30;

				return false;
			}
			g_Player = NULL;
			return true;
		}
		return false;
	default:
		break;
	}

	// 죽음
	if (m_HP <= 0) {
		m_State = STATE_DIE;

		m_iSpriteIndex = df_SPRITE_ALUCARD_DIE;
		m_fLeft = -100;
		m_fRight = -100;
		m_fTop = -100;
		m_fBottom = -100;
		return false;
	}

	// 키처리
	switch (m_byKey) {
	case df_ACTION_STAND:
		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_LU:
		if ((m_X - df_SPEED_ALUCARD_MOVE) >= df_WIN_LEFT) {
			m_X = m_X - df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft - df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight - df_SPEED_ALUCARD_MOVE;
		}
		if ((m_Y - df_SPEED_ALUCARD_MOVE) >= df_WIN_TOP) {
			m_Y = m_Y - df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop - df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom - df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_RU:
		if ((m_X + df_SPEED_ALUCARD_MOVE) <= df_WIN_RIGHT) {
			m_X = m_X + df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft + df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight + df_SPEED_ALUCARD_MOVE;
		}
		if ((m_Y - df_SPEED_ALUCARD_MOVE) >= df_WIN_TOP) {
			m_Y = m_Y - df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop - df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom - df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_LD:
		if ((m_X - df_SPEED_ALUCARD_MOVE) >= df_WIN_LEFT) {
			m_X = m_X - df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft - df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight - df_SPEED_ALUCARD_MOVE;
		}
		if ((m_Y + df_SPEED_ALUCARD_MOVE) <= df_MOVE_BOTTOM) {
			m_Y = m_Y + df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop + df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom + df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_RD:
		if ((m_X + df_SPEED_ALUCARD_MOVE) <= df_WIN_RIGHT) {
			m_X = m_X + df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft + df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight + df_SPEED_ALUCARD_MOVE;
		}
		if ((m_Y + df_SPEED_ALUCARD_MOVE) <= df_MOVE_BOTTOM) {
			m_Y = m_Y + df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop + df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom + df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_LL:
		if ((m_X - df_SPEED_ALUCARD_MOVE) >= df_WIN_LEFT) {
			m_X = m_X - df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft - df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight - df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_RR:
		if ((m_X + df_SPEED_ALUCARD_MOVE) <= df_WIN_RIGHT) {
			m_X = m_X + df_SPEED_ALUCARD_MOVE;
			m_fLeft = m_fLeft + df_SPEED_ALUCARD_MOVE;
			m_fRight = m_fRight + df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_UU:
		if ((m_Y - df_SPEED_ALUCARD_MOVE) >= df_WIN_TOP) {
			m_Y = m_Y - df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop - df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom - df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	case df_ACTION_MOVE_DD:
		if ((m_Y + df_SPEED_ALUCARD_MOVE) <= df_MOVE_BOTTOM) {
			m_Y = m_Y + df_SPEED_ALUCARD_MOVE;
			m_fTop = m_fTop + df_SPEED_ALUCARD_MOVE;
			m_fBottom = m_fBottom + df_SPEED_ALUCARD_MOVE;
		}

		// 폭탄 사용중이면 애니메이션 처리 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if (Animation(df_DELAY_ACTION)) {
			m_iSpriteIndex = m_iAnimeStart;
		}
		break;
	}


	// 폭탄 사용중
	//if (m_bBoom) {
	if (m_Attack == ATTACK_BOOM) {
		if (g_bNoBullet) {
			// 적 탄 제거 및 폭탄 데미지 처리
			iter = g_ObjectList->begin();
			iter_end = g_ObjectList->end();
			while (iter != iter_end) {
				ObjectTemp = *iter;
				if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY_BULLET) {
					--ObjectTemp->m_HP;
				}
				else if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY) {
					if (ObjectTemp->m_bInFlag) {
						ObjectTemp->m_HP = ObjectTemp->m_HP - 0.1;
					}
				}
				++iter;
			}
		}

		if (df_SPRITE_ALUCARD_BOOM_16 < m_iSpriteIndex) {
			g_bNoBullet = false;
		}


		if (Animation(df_DELAY_BOOM)) {
			//m_bBoom = false;
			m_Attack = ATTACK_EMPT;
			m_bSpecialAction = false;
			m_Action = ACTION_END;
			KeyAction(df_ACTION_STAND);
			m_byEnergyShotGauge = 0;
			m_byShotGauge = 0;
		}
		return false;
	}


	// 공격 처리
	//if (m_bShot) {}
	//else {}

	
	if (m_Attack == ATTACK_SHOT) {
		if (m_byEnergyShotGauge < df_ALUCARD_ENERGY_SHOT_GAUGE) {
			++m_byEnergyShotGauge;
			--m_iShotDelay;
			if (m_iShotDelay <= 0) {
				m_iShotDelay = 7;
				Shot();
				--m_byShotGauge;
			}

		}
		else {
			// 충전 공격
			m_byShotGauge = 0;
			if (!m_bCharge) {
				m_bSpecialAction = true;
				m_bCharge = true;
				m_iSpriteIndex = df_SPRITE_ALUCARD_CHARGE_1;
				m_iAnimeStart = df_SPRITE_ALUCARD_CHARGE_1;
				m_iAnimeEnd = df_SPRITE_ALUCARD_CHARGE_8;
				m_iAnimeCount = 0;
				m_iChargeDelay = 0;
				m_iChargeShotCnt = 0;
				m_fChargeAngle = -60;
			}
		}
	}
	else {
		if (m_byShotGauge > 0) {
			--m_iShotDelay;
			if (m_iShotDelay <= 0) {
				m_iShotDelay = 7;
				Shot();
				--m_byShotGauge;
			}
		}

		if (m_bCharge) {
			//m_bChargeShot = true;
			m_Attack = ATTACK_CHARGE_SHOT;
		}
	}


	if (m_Attack == ATTACK_CHARGE_SHOT) {
	//if (m_bChargeShot) {
		if (m_iChargeDelay < 15) {
			++m_iChargeDelay;
		}
		else {
			if (m_iChargeShotCnt < 3) {
				ChargeShot();
			}
			m_iChargeDelay = 0;
			++m_iChargeShotCnt;

		}


		if (m_iChargeShotCnt >= 5) {
			m_bCharge = false;
			//m_bChargeShot = false;
			m_bSpecialAction = false;
			m_Action = ACTION_END;
			KeyAction(df_ACTION_STAND);
		}
	}

	return false;
}

void ObjectAlucard::Draw()
{
	int iRand;

	// 폭탄 스프라이트면 폭탄 이펙트도 같이 그려줌 
	if (df_SPRITE_ALUCARD_BOOM_05 <= m_iSpriteIndex && m_iSpriteIndex <= df_SPRITE_ALUCARD_BOOM_16) {
		iRand = rand() % 2;
		switch (iRand) {
		case 0:
			// 그냥 출력
			g_cSprite.DrawSprite(df_SPRITE_ALUCARD_BOOM_EFFECT_1, 350, m_Y + 30, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
			g_cSprite.DrawSprite(df_SPRITE_ALUCARD_BOOM_EFFECT_2, m_X, 500, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
			break;
		case 1:
			// 합성 출력
			g_cSprite.DrawSpriteCompose(df_SPRITE_ALUCARD_BOOM_EFFECT_1, 350, m_Y + 30, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
			g_cSprite.DrawSpriteCompose(df_SPRITE_ALUCARD_BOOM_EFFECT_2, m_X, 500, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
			break;
		}
		
	}

	// 남은 라이프 있으면 그려줌
	if (m_chLife > 0) {
		g_cSprite.DrawSprite(df_SPRITE_ALUCARD_LIFE, 10, 55, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
		if (m_chLife > 1) {
			g_cSprite.DrawSprite(df_SPRITE_ALUCARD_LIFE, 52, 55, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
		}
	}


	// 플레이 캐릭터
	if (m_bInvincible) {
		g_cSprite.DrawSpriteCompose(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}
	else {
		g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}
}

//------------------------------------------
// 키 입력 처리
// 입력받은 키에 맞게 스프라이트 설정
//------------------------------------------
void ObjectAlucard::KeyAction(BYTE byKey)
{
	// 죽어서 키 입력 안받습니다
	if (m_State == STATE_DIE) {
		return;
	}

	switch (byKey) {
	case df_ACTION_STAND:
	case df_ACTION_MOVE_UU:
	case df_ACTION_MOVE_DD:
		// 현제 입력된 키값 저장
		m_byKey = byKey;

		// 정지 상태가 아니면
		if (m_Action != ACTION_STAND) {
			m_Action = ACTION_STAND;
			// 특수 공격 상태면
			if (m_bSpecialAction) {
				// 스프라이트 변경 안함
				break;
			}
			m_iSpriteIndex = df_SPRITE_ALUCARD_STAND_1;
			m_iAnimeStart = df_SPRITE_ALUCARD_STAND_1;
			m_iAnimeEnd = df_SPRITE_ALUCARD_STAND_6;
			m_iAnimeCount = 0;
		}
		break;
	case df_ACTION_MOVE_LU:
	case df_ACTION_MOVE_LD:
	case df_ACTION_MOVE_LL:
		// 현제 입력된 키값 저장
		m_byKey = byKey;

		// 왼쪽으로 걷는 중이 아니라면
		if (m_Action != ACTION_LMOVE) {
			m_Action = ACTION_LMOVE;
			// 특수 공격 상태면
			if (m_bSpecialAction) {
				// 스프라이트 변경 안함
				break;
			}
			m_iSpriteIndex = df_SPRITE_ALUCARD_LEFT_1_1;
			m_iAnimeStart = df_SPRITE_ALUCARD_LEFT_2_1;
			m_iAnimeEnd = df_SPRITE_ALUCARD_LEFT_2_6;
			m_iAnimeCount = 0;
		}
		break;
	case df_ACTION_MOVE_RU:
	case df_ACTION_MOVE_RD:
	case df_ACTION_MOVE_RR:
		// 현제 입력된 키값 저장
		m_byKey = byKey;

		// 오른쪽으로 걷는 중이 아니라면
		if (m_Action != ACTION_RMOVE) {
			m_Action = ACTION_RMOVE;
			// 특수 공격 상태면
			if (m_bSpecialAction) {
				// 스프라이트 변경 안함
				break;
			}
			m_iSpriteIndex = df_SPRITE_ALUCARD_RIGHT_1_1;
			m_iAnimeStart = df_SPRITE_ALUCARD_RIGHT_2_1;
			m_iAnimeEnd = df_SPRITE_ALUCARD_RIGHT_2_6;
			m_iAnimeCount = 0;
		}
		break;
	case df_ACTION_ATTACK1:
		// 일반 공격
		// 폭탄 사용 중이면 공격 상태 교체 안함
		if (m_Attack == ATTACK_BOOM) {
			break;
		}
		if(m_Attack != ATTACK_SHOT) {
		//if (!m_bShot) {
			//m_bShot = true;
			m_Attack = ATTACK_SHOT;
			m_byEnergyShotGauge = 0;
		}
		m_byShotGauge = 4;
		break;
	case df_ACTION_ATTACK1_END:
		// 충전 공격 및 폭탄 사용중이라면 교체 안함
		if (m_Attack == ATTACK_CHARGE_SHOT || m_Attack == ATTACK_BOOM) {
			break;
		}
		//m_bShot = false;
		m_Attack = ATTACK_EMPT;
		break;
	case df_ACTION_ATTACK2:
		// 폭탄
		// 공격 상태가 아니면
		if (m_Attack != ATTACK_BOOM) {
		//if (!m_bBoom) {
			//m_bBoom = true;
			m_Attack = ATTACK_BOOM;
			m_bSpecialAction = true;
			g_bNoBullet = true;
			m_iSpriteIndex = df_SPRITE_ALUCARD_BOOM_01;
			m_iAnimeStart = df_SPRITE_ALUCARD_BOOM_01;
			m_iAnimeEnd = df_SPRITE_ALUCARD_BOOM_19;
			m_iAnimeCount = 0;
		}
		break;
	case df_ACTION_ATTACK3:
		// 근접 공격
		// 공격 상태가 아니면
		
		break;;
	}
	

	return;
}

//플레이어 움직임 처리
//iAnimeDelay : 프레임 딜레이 값
bool ObjectAlucard::Animation(int iAnimeDelay)
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

void ObjectAlucard::Shot()
{
	int iCnt;
	Object * newObject;

	switch (m_iShotLevel)
	{
	case 0:		// 기본 공격
		iCnt = 0;
		while (iCnt < 3) {
			if (m_bShotArray[iCnt].flag == false) {
				//m_bShotArray[iCnt].count = 1;
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_1, m_X, m_Y, 90, df_SPEED_ALUCARD_BULLET_1,
					df_SPRITE_ALUCARD_BULLET_1_1, df_SPRITE_ALUCARD_BULLET_1_3, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		break;
	case 1:		// 1단
		iCnt = 0;
		while (iCnt < 4) {
			if (m_bShotArray[iCnt].flag == false) {
				//m_bShotArray[iCnt].count = 1;
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_2, m_X, m_Y, 90, df_SPEED_ALUCARD_BULLET_2,
					df_SPRITE_ALUCARD_BULLET_2_1, df_SPRITE_ALUCARD_BULLET_2_2, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		break;
	case 2:		// 2단
		iCnt = 0;
		while (iCnt < 3) {
			if (m_bShotArray[iCnt].flag == false) {
				//m_bShotArray[iCnt].count = 3;
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_1, m_X, m_Y, 90, df_SPEED_ALUCARD_BULLET_1,
					df_SPRITE_ALUCARD_BULLET_1_1, df_SPRITE_ALUCARD_BULLET_1_3, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_1, m_X - 30, m_Y + 20, 93.5, df_SPEED_ALUCARD_BULLET_1,
					df_SPRITE_ALUCARD_BULLET_1_1, df_SPRITE_ALUCARD_BULLET_1_3, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_1, m_X + 30, m_Y + 20, 86.5, df_SPEED_ALUCARD_BULLET_1,
					df_SPRITE_ALUCARD_BULLET_1_1, df_SPRITE_ALUCARD_BULLET_1_3, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		iCnt = 0;
		while (iCnt < 2) {
			if (m_bShotArray2[iCnt].flag == false) {
				//m_bShotArray2[iCnt].count = 2;
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_3, m_X - 50, m_Y + 50, 90.55, df_SPEED_ALUCARD_BULLET_3,
					df_SPRITE_ALUCARD_BULLET_3_1, df_SPRITE_ALUCARD_BULLET_3_4, &m_bShotArray2[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_3, m_X + 50, m_Y + 50, 89.45, df_SPEED_ALUCARD_BULLET_3,
					df_SPRITE_ALUCARD_BULLET_3_1, df_SPRITE_ALUCARD_BULLET_3_4, &m_bShotArray2[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		break;
	case 3:
		iCnt = 0;
		while (iCnt < 4) {
			if (m_bShotArray[iCnt].flag == false) {
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_2, m_X, m_Y, 90, df_SPEED_ALUCARD_BULLET_2,
					df_SPRITE_ALUCARD_BULLET_2_1, df_SPRITE_ALUCARD_BULLET_2_2, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_2, m_X - 50, m_Y + 20, 93.5, df_SPEED_ALUCARD_BULLET_2,
					df_SPRITE_ALUCARD_BULLET_2_1, df_SPRITE_ALUCARD_BULLET_2_2, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_2, m_X + 50, m_Y + 20, 86.5, df_SPEED_ALUCARD_BULLET_2,
					df_SPRITE_ALUCARD_BULLET_2_1, df_SPRITE_ALUCARD_BULLET_2_2, &m_bShotArray[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		iCnt = 0;
		while (iCnt < 4) {
			if (m_bShotArray2[iCnt].flag == false) {
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_3, m_X - 50, m_Y + 50, 90.55, df_SPEED_ALUCARD_BULLET_3,
					df_SPRITE_ALUCARD_BULLET_3_1, df_SPRITE_ALUCARD_BULLET_3_4, &m_bShotArray2[iCnt], true);
				g_ObjectList->push_back(newObject);
				newObject = new ObjectBullet(df_TYPE_OBJECT_ALUCARD_BULLET_3, m_X + 50, m_Y + 50, 89.45, df_SPEED_ALUCARD_BULLET_3,
					df_SPRITE_ALUCARD_BULLET_3_1, df_SPRITE_ALUCARD_BULLET_3_4, &m_bShotArray2[iCnt], true);
				g_ObjectList->push_back(newObject);
				break;
			}
			++iCnt;
		}
		break;
	default:
		break;
	}
}


void ObjectAlucard::ChargeShot()
{
	int iCnt;
	Object * newObject;
	float fCopyAngle;
	float fAngle;
	float fHorizontalAngle;

	fCopyAngle = m_fChargeAngle;
	
	iCnt = 0;
	while (iCnt < 5) {
		fHorizontalAngle = 180 - fCopyAngle;

		if (fCopyAngle < 0) {
			fAngle = fCopyAngle + 360;
		}
		else {
			fAngle = fCopyAngle;
		}

		newObject = new ObjectGuidedBullet(df_TYPE_OBJECT_ALUCARD_GUIDED_BULLET, m_X, m_Y + 100, fHorizontalAngle, true);
		g_ObjectList->push_back(newObject);
		newObject = new ObjectGuidedBullet(df_TYPE_OBJECT_ALUCARD_GUIDED_BULLET, m_X, m_Y + 100, fAngle, true);
		g_ObjectList->push_back(newObject);
		fCopyAngle = fCopyAngle + 30;
		++iCnt;
	}

	m_fChargeAngle = m_fChargeAngle + 5;
	

}
