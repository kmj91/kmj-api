#pragma once
#include "ObjectUnit.h"

class UnitEnemy : public ObjectUnit
{
public:
	UnitEnemy(st_SpriteInfo stSpritInfo, int iUnitType, int iPosX, int iPosY, int iIncreaseValue);
	virtual ~UnitEnemy();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iIncreaseValue;					// 곱셈 값
};

