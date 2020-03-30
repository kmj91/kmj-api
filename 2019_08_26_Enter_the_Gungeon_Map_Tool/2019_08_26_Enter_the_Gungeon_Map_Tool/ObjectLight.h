#pragma once
#include "Object.h"
class ObjectLight : public Object
{
public:
	ObjectLight(int iSpriteIndex, int iPosX, int iPosY, int iIncreaseValue);
	virtual ~ObjectLight();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iIncreaseValue;						// 곱셈 값
};

