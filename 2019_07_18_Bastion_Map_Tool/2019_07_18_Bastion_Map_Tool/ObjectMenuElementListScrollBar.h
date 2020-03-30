#pragma once
#include "ObjectUI.h"

class ObjectMenuElementList;

class ObjectMenuElementListScrollBar : public ObjectUI
{
public:
	ObjectMenuElementListScrollBar(int iPosX, int iPosY, int iWidth, int iHeight, ObjectMenuElementList * MenuListPtr);
	virtual ~ObjectMenuElementListScrollBar();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	void ScrollMove(int iMoveValueY);
	void ChangeScrollBarSize(int iScrollBarSize, bool bEmployScrollBar);

public:
	// 스크롤 바 영역
	double m_dScrollBarLeft;
	double m_dScrollBarRight;
	double m_dScrollBarTop;
	double m_dScrollBarBottom;

private:
	int m_iWidth;								// 스크롤 바 영역 너비
	int m_iHeight;								// 스크롤 바 영역 높이
	int m_iScrollBarSize;						// 스크롤 바 크기

	ObjectMenuElementList * m_MenuListPtr;		// 메뉴 목록 포인터

	bool m_bEmployScrollBar;					// 스크롤 바 사용 여부
};

