#pragma once
#include "ObjectUI.h"

class UIMenu;

class UITab : public ObjectUI
{
public:
	UITab(int iSpriteIndex , int iPosX, int iPosY, int iMenuType, UIMenu * MenuPtr);
	virtual ~UITab();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	int m_iMenuType;
	UIMenu * m_MenuPtr;			// 메뉴 포인터
};

