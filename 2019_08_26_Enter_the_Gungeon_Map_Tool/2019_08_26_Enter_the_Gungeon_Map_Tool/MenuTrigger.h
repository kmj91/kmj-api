#pragma once
#include "UIMenu.h"

class MenuTrigger : public UIMenu
{
public:
	MenuTrigger(int iPosX, int iPosY, int iWidth, int iHeight);
	virtual ~MenuTrigger();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);	// 메뉴와 오브젝트 요소 전부 이동

	virtual bool CheckOnMouse();							// 마우스가 올라와있나?
};

