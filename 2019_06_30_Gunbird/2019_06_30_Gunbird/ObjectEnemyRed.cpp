#include "stdafx.h"
#include "ObjectEnemyRed.h"
#include "ObjectBullet.h"
#include "ObjectItemPowerUp.h"
#include "ObjectEffect.h"


ObjectEnemyRed::ObjectEnemyRed(int iAppearance, int iOutTime, int iPosX, int iPosY, int iDestPosX, int iDestPosY, float fDestAngle,
	int iOutPosX, int iOutPosY, float fOutAngle)
{
	m_ComprehensiveType = df_COMPREHENSIVE_TYPE_ENEMY;
	m_Type = df_TYPE_OBJECT_ENEMY_RED;
	m_iAppearance = iAppearance;
	m_iOutTime = iOutTime;
	m_X = iPosX;
	m_Y = iPosY;
	m_HP = 1;

	m_fDestPosX = iDestPosX;
	m_fDestPosY = iDestPosY;
	m_fDestAngle = fDestAngle;
	m_fOutPosX = iOutPosX;
	m_fOutPosY = iOutPosY;
	m_fOutAngle = fOutAngle;

	m_iSpriteIndex = df_SPRITE_ENEMY_RED_CENTER_1;
	m_iAnimeStart = df_SPRITE_ENEMY_RED_CENTER_1;
	m_iAnimeEnd = df_SPRITE_ENEMY_RED_CENTER_2;
	m_iAnimeCount = 0;

	m_iActionTime = GetTickCount();
	m_iOnShotCnt = 0;

	m_Action = STATE_STAND;
	m_bOnFlag = false;
	m_bOutFlag = false;
	m_bInFlag = false;

	m_fLeft = m_X - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointX;
	m_fTop = m_Y - g_cSprite.m_stpSprite[m_iSpriteIndex].iCenterPointY;
	m_fRight = m_fLeft + g_cSprite.m_stpSprite[m_iSpriteIndex].iWidth;
	m_fBottom = m_fTop + g_cSprite.m_stpSprite[m_iSpriteIndex].iHeight;

	// 피격 판정
	m_fLeft = m_fLeft + 19;
	m_fRight = m_fRight - 18;
	m_fTop = m_fTop + 22;
	m_fBottom = m_fBottom - 15;
}


ObjectEnemyRed::~ObjectEnemyRed()
{
}

bool ObjectEnemyRed::Action()
{
	int iTime;
	int iPattern;
	int iRand;
	Object *newObject;
	ObjectBullet * newBullet;

	// 죽음
	if (m_HP <= 0) {
		newObject = new ObjectItemPowerUp(m_X, m_Y, -70);
		g_ObjectList->push_back(newObject);
		newObject = new ObjectEffect(df_TYPE_OBJECT_EFFECT_KABOOM_1, m_X, m_Y);
		g_ObjectList->push_back(newObject);
		return true;
	}

	// 등장할 스크롤 좌표
	if (m_iAppearance > g_StageBackground->m_Y) {
		return false;
	}

	//-------------------------------------------------------------------------
	// 화면 안에 들어왔나?
	// 충돌 처리 할 때 화면에 들어오기도 전에 죽는거 방지 하기 위해 필요하다
	//-------------------------------------------------------------------------
	if (!m_bInFlag) {
		if (m_X > df_IN_GAME_LEFT && m_X < df_IN_GAME_RIGHT && m_Y > df_IN_GAME_TOP && m_Y < df_IN_GAME_BOTTOM) {
			m_bInFlag = true;
		}
	}

	//-------------------------------------------
	// 적 배치
	// 맵 바깥에서 화면 안으로 배치된다
	//-------------------------------------------
	if (!m_bOnFlag) {
		//-------------------------------------------
		// 배치 되면서 공격
		// 공격후 재 공격 초 : 게임 프레임 * (조건문 N)
		//-------------------------------------------
		++m_iOnShotCnt;
		if (m_iOnShotCnt > 50) {
			m_iOnShotCnt = 0;
			//newBullet = new ObjectBullet(df_TYPE_OBJECT_ENEMY_BULLET_1, m_X, m_Y, 90, df_SPEED_ENEMY_BULLET_1,
			//	df_SPRITE_ENEMY_BULLET_1_1, df_SPRITE_ENEMY_BULLET_1_3);
			
			// 폭탄 사용중이 아니면
			if (!g_bNoBullet) {
				// 오브젝트 풀
				newBullet = g_ObjectBulletPool.Alloc();
				newBullet->Init(df_TYPE_OBJECT_ENEMY_BULLET_1, m_X, m_Y, -90, df_SPEED_ENEMY_BULLET_1,
					df_SPRITE_ENEMY_BULLET_1_1, df_SPRITE_ENEMY_BULLET_1_3);
				g_ObjectList->push_back(newBullet);
			}
		}

		// 이동 처리
		if (Move(m_fDestPosX, m_fDestPosY, m_fDestAngle, df_SPEED_ENEMY_RED_MOVE)) {
			// 배치 완료
			m_bOnFlag = true;
			if (m_Action != STATE_STAND) {
				m_Action = STATE_STAND;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_CENTER_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_CENTER_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_CENTER_2;
				m_iAnimeCount = 0;
			}
			// 배치 시간
			m_iOnTime = GetTickCount();
		}
		return false;
	}
	
	//-------------------------------------------
	// 퇴장
	// 화면 밖으로 나간다
	//-------------------------------------------
	if (m_bOutFlag) {
		// 이동 처리
		if (Move(m_fOutPosX, m_fOutPosY, m_fOutAngle, df_SPEED_ENEMY_RED_MOVE)) {
			// 퇴장 완료
			return true;
		}
		return false;
	}

	// 퇴장 시간인가?
	iTime = GetTickCount();
	if ((iTime - m_iOnTime) > m_iOutTime) {
		m_bOutFlag = true;
	}
	else {
		if ((iTime - m_iActionTime) > df_ENEMY_ACTION_DELAY) {
			m_iActionTime = iTime;
			// 행동 패턴
			iPattern = rand() % 100;
			
			// 대기
			if (iPattern < 50) {
				iRand = (rand() % 200) - 100;
				m_fDestPosX = m_X + iRand;
				iRand = (rand() % 30) - 15;
				m_fDestPosY = m_Y + iRand;
			}
			else {
				//newBullet = new ObjectBullet(df_TYPE_OBJECT_ENEMY_BULLET_1, m_X, m_Y, 90, df_SPEED_ENEMY_BULLET_1,
				//	df_SPRITE_ENEMY_BULLET_1_1, df_SPRITE_ENEMY_BULLET_1_3);
				
				// 폭탄 사용중이 아니면
				if (!g_bNoBullet) {
					// 오브젝트 풀
					newBullet = g_ObjectBulletPool.Alloc();
					newBullet->Init(df_TYPE_OBJECT_ENEMY_BULLET_1, m_X, m_Y, -90, df_SPEED_ENEMY_BULLET_1,
						df_SPRITE_ENEMY_BULLET_1_1, df_SPRITE_ENEMY_BULLET_1_3);
					g_ObjectList->push_back(newBullet);
				}
			}
		}
	}

	//-------------------------------------------
	// 처음 배치받은 자리에 있거나
	// 다음 이동 장소로 가거나
	//-------------------------------------------
	if (Move(m_fDestPosX, m_fDestPosY, 0, df_SPEED_ENEMY_RED_STAND, true)) {
		if (m_Action != STATE_STAND) {
			m_Action = STATE_STAND;
			m_iSpriteIndex = df_SPRITE_ENEMY_RED_CENTER_1;
			m_iAnimeStart = df_SPRITE_ENEMY_RED_CENTER_1;
			m_iAnimeEnd = df_SPRITE_ENEMY_RED_CENTER_2;
			m_iAnimeCount = 0;
		}
	}
	

	if (Animation(df_DELAY_ACTION)) {
		m_iSpriteIndex = m_iAnimeStart;
	}

	

	return false;
}

void ObjectEnemyRed::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}

//------------------------------------
// 애니메이션 처리
// iAnimeDelay : 프레임 딜레이 값
//------------------------------------
bool ObjectEnemyRed::Animation(int iAnimeDelay)
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

// 해당 좌표로 이동 처리
bool ObjectEnemyRed::Move(float fDestPosX, float fDestPosY, float fAngle, float fSpeed, bool bOption)
{
	float fDest;	// 목표 까지의 거리
	float vx;
	float vy;
	float rad;
	float vx2;
	float vy2;

	// 목적지까지 거리 구하기
	fDest = sqrt((fDestPosX - m_X)*(fDestPosX - m_X) + (fDestPosY - m_Y)*(fDestPosY - m_Y));

	if (fDest) {
		vx = (fDestPosX - m_X) / fDest * fSpeed;
		vy = (fDestPosY - m_Y) / fDest * fSpeed;
	}

	// 목적지 도착
	if (fDest < fSpeed) {
		return true;
	}
	else {
		//float fx;
		//float fy;
		//float fAngle;
		//fx = (m_fDestPosX - m_X);
		//fy = (m_fDestPosY - m_Y);
		//fAngle = atan(-fy / fx);

		// 선회 각도
		rad = PI / 180 * fAngle;
		vx2 = cos(rad) * vx - sin(rad)*vy;
		vy2 = sin(rad) * vx + cos(rad)*vy;

		m_X = m_X + vx2;
		m_Y = m_Y + vy2;

		m_fLeft = m_fLeft + vx2;
		m_fRight = m_fRight + vx2;
		m_fTop = m_fTop + vy2;
		m_fBottom = m_fBottom + vy2;
		
		// 스프라이트 변경 안하는 옵션
		if (bOption) {
			return false;
		}

		//-------------------------------------------
		// 스프라이트
		// 이동 방향으로 바꺼준다
		//-------------------------------------------
		// STATE_RR
		if (vx2 > 4) {
			if (m_Action != STATE_RR) {
				m_Action = STATE_RR;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_RIGHT_2_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_RIGHT_2_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_RIGHT_2_2;
				m_iAnimeCount = 0;
			}
		}
		// STATE_R
		else if (vx2 > 1) {
			if (m_Action != STATE_R) {
				m_Action = STATE_R;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_RIGHT_1_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_RIGHT_1_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_RIGHT_1_2;
				m_iAnimeCount = 0;
			}

		}
		else if (vx2 < -4) {
			if (m_Action != STATE_LL) {
				m_Action = STATE_LL;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_LEFT_2_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_LEFT_2_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_LEFT_2_2;
				m_iAnimeCount = 0;
			}
		}
		else if (vx2 < -1) {
			if (m_Action != STATE_L) {
				m_Action = STATE_L;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_LEFT_1_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_LEFT_1_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_LEFT_1_2;
				m_iAnimeCount = 0;
			}
		}
		else {
			if (m_Action != STATE_STAND) {
				m_Action = STATE_STAND;
				m_iSpriteIndex = df_SPRITE_ENEMY_RED_CENTER_1;
				m_iAnimeStart = df_SPRITE_ENEMY_RED_CENTER_1;
				m_iAnimeEnd = df_SPRITE_ENEMY_RED_CENTER_2;
				m_iAnimeCount = 0;
			}
		}
	}// if (fDest < df_SPEED_ENEMY_MOVE) {

	if (Animation(df_DELAY_ACTION)) {
		m_iSpriteIndex = m_iAnimeStart;
	}

	return false;
}
