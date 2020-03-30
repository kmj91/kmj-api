#pragma once
#include "Object.h"

struct Bullet;

class ObjectBullet :public Object
{
public:
	ObjectBullet();
	// Option : false 적군 탄	true 아군 탄
	ObjectBullet(int iType, int iPosX, int iPosY, float fAngle, float fSpeed, 
		int iStartIndex, int iEndIndex, bool bOption = false);
	ObjectBullet(int iType, int iPosX, int iPosY, float fAngle, float fSpeed, 
		int iStartIndex, int iEndIndex, Bullet * bShotArrayPtr, bool bOption = false);
	virtual ~ObjectBullet();

	virtual bool Action();
	virtual void Draw();

	void Init(int iType, int iPosX, int iPosY, float fAngle, float fSpeed,
		int iStartIndex, int iEndIndex, bool bOption = false);

private:
	bool Animation(int iAnimeDelay);		// 애니메이션 처리

private:
	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;

	float m_fSpeed;

	float m_fRadian;
	Bullet * m_bShotArrayPtr;
	bool m_bOption;

	bool m_bReleaseFlag;
};

