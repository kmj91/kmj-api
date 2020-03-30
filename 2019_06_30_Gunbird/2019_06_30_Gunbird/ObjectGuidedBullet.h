#pragma once
#include "Object.h"

class ObjectGuidedBullet : public Object
{
public:
	// Option : false 적군 탄	true 아군 탄
	ObjectGuidedBullet(int iType, int iPosX, int iPosY, float fAngle, bool bOption = false);
	virtual ~ObjectGuidedBullet();

	virtual bool Action();
	virtual void Draw();

private:
	int m_iSpriteIndex;
	int m_fSpeed;

	float m_fRadian;
	bool m_bOption;
};

