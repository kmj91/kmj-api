#pragma once
#include "ObjectUI.h"

class UIMenu;

//--------------------------------------
// 스프라이트 16 x 16 짜리를 쓰고있는게 있음
// 64 x 64로 변경하기 때문
// 그에 따른 곱셈 연산 필요
//--------------------------------------
class UIElement : public ObjectUI
{
public:
	UIElement(int iSpriteIndex, int iPosX, int iPosY, int * iScrollTopPtr, int * iScrollBottomPtr,
		st_UITileUnitElementData * st_ElementData, UIMenu * MenuPtr, int iIncreaseValue = 1);
	virtual ~UIElement();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	void SetPosition(int iPosX, int iPosY);

public:
	int m_iSpriteWidth;									// 스프라이트 가로 길이
	int m_iSpriteHeight;								// 스프라이트 세로 길이

	int * m_iScrollTopPtr;								// 스크롤 위
	int * m_iScrollBottomPtr;							// 스크롤 아래

	bool m_bSelectElement;								// 요소가 선택됨

private:
	int m_iIncreaseValue;								// 곱셈 값
	int m_iMouseState;									// 마우스 상태
	st_SpriteInfo m_stMousePointerSprite;				// 마우스 포인터 스프라이트
	int m_iMouseSpriteIncreaseVlaue;					// 마우스 스프라이트 증가 값
	int m_iElementType;									// 종류
	int m_iElementState;								// 상태
	int m_iElementSpriteIncreaseVlaue;					// 요소 스프라이트 증가 값

	UIMenu * m_MenuPtr;									// 메뉴 포인터
};

