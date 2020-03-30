#pragma once
#include "ObjectUI.h"

class ObjectMenu;

class ObjectMenuEnemy : public ObjectUI
{
public:
	ObjectMenuEnemy(int iPosX, int iPosY, ObjectMenu * MenuPtr);
	virtual ~ObjectMenuEnemy();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	ObjectMenu * m_MenuPtr;			// 메뉴 포인터
};

