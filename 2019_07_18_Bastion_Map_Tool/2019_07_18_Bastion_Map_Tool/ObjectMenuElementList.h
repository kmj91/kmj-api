#pragma once
#include "ObjectUI.h"

#include <vector>

using namespace std;

class ObjectMenuElement;
class ObjectMenuElementListScrollBar;
enum FocusMenu;

class ObjectMenuElementList : public ObjectUI
{
public:
	ObjectMenuElementList(int iPosX, int iPosY, int iWidth, int iHeight);
	virtual ~ObjectMenuElementList();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	void ScrollMove(int iMoveValueY);
	void ChangeElementList(FocusMenu enFocusMenu);
	void SetElementList(ObjectMenuElement** ObjectElementArray, int iSize,
		ObjectMenuElementListScrollBar * SetScrollBar);

private:
	CList<ObjectUI *> * m_MenuElementObjectList;			// 메뉴 요소 오브젝트 리스트 (실제 출력할 오브젝트)
	vector<Object *> * m_ObjectMenuTileElementArr;			// 타일 아이콘 오브젝트 벡터
	vector<Object *> * m_ObjectMenuStageElementArr;			// 스테이지 아이콘 오브젝트 벡터
	vector<Object *> * m_ObjectMenuPlayerElementArr;		// 플레이어 아이콘 오브젝트 벡터
	vector<Object *> * m_ObjectMenuObjectElementArr;		// 물체 아이콘 오브젝트 벡터
	vector<Object *> * m_ObjectMenuBrokenElementArr;		// 부서지는 물체 아이콘 오브젝트 벡터
	vector<Object *> * m_ObjectMenuEnemyElementArr;			// 적 아이콘 오브젝트 벡터
	
	ObjectMenuElementListScrollBar * m_FocusScrollBar;		// 현제 스크롤 바
	ObjectMenuElementListScrollBar * m_TileScrollBar;		// 타일 메뉴 스크롤 바
	ObjectMenuElementListScrollBar * m_StageScrollBar;		// 스테이지 메뉴 스크롤 바
	ObjectMenuElementListScrollBar * m_PlayerScrollBar;		// 플레이어 메뉴 스크롤 바
	ObjectMenuElementListScrollBar * m_ObjectScrollBar;		// 물체 메뉴 스크롤 바
	ObjectMenuElementListScrollBar * m_BrokenScrollBar;		// 부서지는 물체 메뉴 스크롤 바
	ObjectMenuElementListScrollBar * m_EnemyScrollBar;		// 적 메뉴 스크롤 바

	int m_iScrollTop;										// 화면상의 실제 스크롤 위
	int m_iScrollBottom;									// 화면상의 실제 스크롤 아래
	int m_iElementListWidth;								// 메뉴 요소 목록 가로 길이
	int m_iElementListHeight;								// 메뉴 요소 목록 세로 길이
	double m_dScrollMoveValue;									// 스크롤 바 이동 값
};