#pragma once
#include "Object.h"
class ObjectPlayer : public Object
{
public:
	ObjectPlayer();
	virtual ~ObjectPlayer();

	virtual void KeyAction(BYTE byKey) = 0;				//키입력 처리

private:
	virtual bool Animation(int iAnimeDelay) = 0;		//플레이어 움직임 처리
	
public:
	int m_iShotLevel;			// 총알 레벨
	bool m_bInvincible;			// 무적
};

