#pragma once
#include "ObjectUI.h"

class UIList;

class UIScrollBar : public ObjectUI
{
public:
	UIScrollBar(int iPosX, int iPosY, int iWidth, int iHeight, UIList * MenuListPtr);
	virtual ~UIScrollBar();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	void ScrollMove(int iMoveValueY);
	void ChangeScrollBarSize(int iScrollBarSize, bool bEmployScrollBar);

public:
	// 스크롤 바 영역
	int m_iScrollBarLeft;
	int m_iScrollBarRight;
	int m_iScrollBarTop;
	int m_iScrollBarBottom;

private:
	int m_iScrollBarSize;						// 스크롤 바 크기

	UIList * m_MenuListPtr;						// 메뉴 목록 포인터

	bool m_bEmployScrollBar;					// 스크롤 바 사용 여부
};

