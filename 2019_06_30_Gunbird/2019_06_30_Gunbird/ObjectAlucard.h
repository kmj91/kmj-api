#pragma once
#include "ObjectPlayer.h"

class ObjectAlucard : public ObjectPlayer
{
private:
	enum PlayerState {
		STATE_RESPAWN,			// 리스폰 중
		STATE_PLAY,				// 게임 중
		STATE_DIE				// 죽음
	};

	enum PlayerAction {
		ACTION_STAND,			// 대기
		ACTION_LMOVE,			// 왼쪽 이동
		ACTION_RMOVE,			// 오른쪽 이동
		ACTION_END				// 특수 행동 끝남
	};

	enum PlayerAttack {
		ATTACK_EMPT,			// 공격 안함
		ATTACK_SHOT,			// 일반 공격
		ATTACK_CHARGE_SHOT,		// 충전 공격
		ATTACK_BOOM				// 폭탄
	};

public:
	ObjectAlucard();
	virtual ~ObjectAlucard();

	virtual bool Action();
	virtual void Draw();

	virtual void KeyAction(BYTE byKey);				//키입력 처리

private:
	virtual bool Animation(int iAnimeDelay);		//플레이어 움직임 처리

	void Shot();
	void ChargeShot();

private:
	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;

	int m_iShotDelay;
	int m_iChargeDelay;
	int m_iChargeShotCnt;

	float m_fChargeAngle;

	PlayerState m_State;		// 플레이어 상태
	PlayerAction m_Action;		// 플레이어 키 입력 행동
	PlayerAttack m_Attack;		// 공격 상태
	BYTE m_byKey;				// 현제 입력된 키값
	bool m_bSpecialAction;		// 특수 행동 여부
	//bool m_bShot;				// 공격 버튼 확인
	bool m_bCharge;				// 충전
	//bool m_bChargeShot;			// 충전 후 버튼 땜
	BYTE m_byEnergyShotGauge;	// 충전 공격 게이지
	BYTE m_byShotGauge;			// 연사 게이지
	
	//bool m_bBoom;				// 폭탄

	Bullet m_bShotArray[4];
	Bullet m_bShotArray2[8];

	char m_chLife;				// 라이프
};

