#pragma once
#include "ObjectUI.h"

class UIMenu;

class UIButton : public ObjectUI
{
public:
	UIButton(const wchar_t * wchpButtonName, int iPosX, int iPosY, int iWidth, int iHeight, int iMenuType, UIMenu * MenuPtr);
	virtual ~UIButton();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

private:
	int m_iMenuType;
	int m_iNameSize;			// 버튼 이름 길이
	wchar_t * m_wchpName;		// 버튼 이름 배열
	UIMenu * m_MenuPtr;			// 메뉴 포인터
	FT_Face * m_face;			// 폰트 페이스
};

