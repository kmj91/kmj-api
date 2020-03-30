#pragma once
#include "ObjectUI.h"

class UIMenu;

//----------------------------------------------------------------
// 이 클래스는 넘겨받은 트리거 데이터를 (st_UITriggerElementData)
// 소멸할때 메모리 정리를 해줘야 함
// 수정, 삭제 버튼
//----------------------------------------------------------------
class UITriggerElement : public ObjectUI
{
public:
	UITriggerElement(int * iScrollTopPtr, int * iScrollBottomPtr, st_UITriggerElementData * st_ElementData, UIMenu * MenuPtr);
	virtual ~UITriggerElement();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	void SetPosition(int iPosX, int iPosY);

public:
	int * m_iScrollTopPtr;								// 스크롤 위
	int * m_iScrollBottomPtr;							// 스크롤 아래

	bool m_bSelectElement;								// 요소가 선택됨

private:
	st_UITriggerElementData * m_stTriggerElementData;	// 트리거 데이터 포인터

	UIMenu * m_MenuPtr;									// 메뉴 포인터
	FT_Face * m_face;									// 폰트 페이스
};

