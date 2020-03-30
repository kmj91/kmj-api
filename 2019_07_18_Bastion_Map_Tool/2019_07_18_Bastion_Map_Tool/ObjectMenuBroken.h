#pragma once
#include "ObjectUI.h"

class ObjectMenu;

class ObjectMenuBroken : public ObjectUI
{
public:
	ObjectMenuBroken(int iPosX, int iPosY, ObjectMenu * MenuPtr);
	virtual ~ObjectMenuBroken();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	ObjectMenu * m_MenuPtr;			// 메뉴 포인터
};

