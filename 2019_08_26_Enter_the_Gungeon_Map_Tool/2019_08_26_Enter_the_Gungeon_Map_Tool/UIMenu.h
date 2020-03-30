#pragma once
#include "ObjectUI.h"

class UIList;
class UIElement;

class UIMenu : public ObjectUI
{
public:
	UIMenu();
	virtual ~UIMenu();

	virtual bool Action() = 0;
	virtual void Draw() = 0;
	virtual bool Click() = 0;

	virtual void Move(int iMoveValueX, int iMoveValueY) = 0;	// 메뉴와 오브젝트 요소 전부 이동

	virtual bool CheckOnMouse() = 0;			// 마우스가 올라와있나?

protected:
	CList<ObjectUI *> *m_MenuObjectList;		// 메뉴 오브젝트 리스트
	UIList * m_MenuElementList;					// 메뉴 요소 리스트
	int m_iFocusMenu;							// 선택된 메뉴

public:
	int m_iChangeMenu;							// 교체 메뉴
	UIElement * m_SelectElement;				// 선택된 요소 (요소 클릭시 변경)
	bool m_bOpenMenu;							// 오픈 플래그
};

