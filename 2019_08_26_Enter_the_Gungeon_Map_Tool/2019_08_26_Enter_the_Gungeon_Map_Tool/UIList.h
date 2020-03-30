#pragma once
#include "ObjectUI.h"

#include <vector>

using namespace std;

class UIScrollBar;

class UIList : public ObjectUI
{
public:
	UIList();
	virtual ~UIList();

	virtual bool Action() = 0;
	virtual void Draw() = 0;
	virtual bool Click() = 0;

	virtual void Move(int iMoveValueX, int iMoveValueY) = 0;

	virtual void AddElementListPtr(st_UIElementInfo stElementListInfo) = 0;
	virtual void ScrollMove(int iMoveValueY) = 0;
	virtual void ChangeElementList(int iFocusMenu) = 0;

public:
	int m_iScrollTop;										// 화면상의 실제 스크롤 위
	int m_iScrollBottom;									// 화면상의 실제 스크롤 아래
	int m_iElementListWidth;								// 메뉴 요소 목록 가로 길이
	int m_iElementListHeight;								// 메뉴 요소 목록 세로 길이

protected:
	UIScrollBar * m_FocusScrollBar;							// 현제 스크롤 바
	vector<ObjectUI *> * m_UIElementObjectVector;			// 메뉴 요소 오브젝트 벡터 (실제 출력할 오브젝트)
	vector<st_UIElementInfo> *m_ElmentInfoVector;			// UI 리스트가 가지고있는 요소 정보 구조체를 저장하는 벡터

	double m_dScrollMoveValue;								// 스크롤 바 이동 값
};

