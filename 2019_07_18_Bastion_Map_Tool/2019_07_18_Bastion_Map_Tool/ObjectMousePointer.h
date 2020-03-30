#pragma once
#include "Object.h"
class ObjectMousePointer : public Object
{
public:
	ObjectMousePointer(int iPosX, int iPosY);
	virtual ~ObjectMousePointer();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

public:
	int m_iSpriteWidth;
	int m_iSpriteHeight;

	bool bRedOption;
};

