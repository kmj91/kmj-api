#pragma once
#include "ObjectUI.h"

class ObjectMenu;

class ObjectMenuObject : public ObjectUI
{
public:
	ObjectMenuObject(int iPosX, int iPosY, ObjectMenu * MenuPtr);
	virtual ~ObjectMenuObject();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	ObjectMenu * m_MenuPtr;			// 메뉴 포인터
};

