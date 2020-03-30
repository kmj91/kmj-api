#pragma once
#include "Object.h"

class ObjectItemPowerUp : public Object
{
public:
	ObjectItemPowerUp(int iPosX, int iPosY, float fAngle);
	virtual ~ObjectItemPowerUp();

	virtual bool Action();
	virtual void Draw();

private:
	bool Animation(int iAnimeDelay);		//플레이어 움직임 처리

private:
	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;

	float m_fRadian;

	int m_iReleaseCount;
	bool m_bOnFlag;							// 플레이어가 습득
};

