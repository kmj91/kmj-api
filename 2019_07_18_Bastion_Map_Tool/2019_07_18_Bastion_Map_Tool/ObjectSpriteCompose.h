#pragma once
#include "Object.h"
class ObjectSpriteCompose : public Object
{
public:
	ObjectSpriteCompose(int iSpriteIndex, int iPosX, int iPosY, double dCompose);
	virtual ~ObjectSpriteCompose();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iSpriteIndex;
	
	double m_dCompose;

	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;
};

