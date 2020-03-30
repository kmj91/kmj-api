#pragma once
#include "UIList.h"

class ListTrigger : public UIList
{
public:
	ListTrigger(int iPosX, int iPosY, int iWidth, int iHeight, int iScrollBarWidth);
	virtual ~ListTrigger();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	virtual void AddElementListPtr(st_UIElementInfo stElementListInfo);		// 리스트 영역에서 표현할 요소 리스트들을 추가합니다
	virtual void ScrollMove(int iMoveValueY);
	virtual void ChangeElementList(int iFocusMenu);							// 메뉴의 리스트 영역을 교체 한다
};

