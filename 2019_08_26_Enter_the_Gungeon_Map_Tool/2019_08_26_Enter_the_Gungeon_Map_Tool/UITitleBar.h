#pragma once
#include "ObjectUI.h"

class UITitleBar : public ObjectUI
{
public:
	UITitleBar(DWORD dwRGBColor, int iPosX, int iPosY, int iWidth, int iHeight, double dCompose, ObjectUI * MenuPtr);
	virtual ~UITitleBar();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	DWORD m_dwRGBColor;				// 색상

	double m_dCompose;				// 합성 값

	ObjectUI * m_MenuPtr;			// 메뉴 포인터
};

