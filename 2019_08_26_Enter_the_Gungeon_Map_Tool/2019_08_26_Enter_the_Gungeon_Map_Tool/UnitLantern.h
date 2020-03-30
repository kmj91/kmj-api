#pragma once
#include "ObjectUnit.h"

class ObjectLight;

class UnitLantern : public ObjectUnit
{
public:
	UnitLantern(st_SpriteInfo stSpritInfo, int iUnitType, int iPosX, int iPosY, int iIncreaseValue);
	virtual ~UnitLantern();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iIncreaseValue;					// 곱셈 값

	ObjectLight * m_ObjectLightPtr;			// 랜턴 빛
};

