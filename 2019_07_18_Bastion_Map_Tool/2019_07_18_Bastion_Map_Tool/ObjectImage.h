#pragma once
#include "Object.h"
class ObjectImage : public Object
{
public:
	ObjectImage(int iSpriteIndex, int iPosX, int iPosY);
	virtual ~ObjectImage();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iSpriteIndex;

	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;
};

