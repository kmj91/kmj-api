#include "stdafx.h"
#include "MenuTileUnit.h"
#include "UITitleBar.h"
#include "UITab.h"
#include "ListTileUnit.h"
#include "UIElement.h"

MenuTileUnit::MenuTileUnit(int iPosX, int iPosY, int iWidth, int iHeight)
{
	ObjectUI * newObject;
	vector<ObjectUI *> * ElementVector;				// 요소 벡터
	UIScrollBar * ScrollBar;						// 스크롤 바
	st_UIElementInfo stElementInfo;					// Element 리스트 정보 구조체
	st_UITileUnitElementData stElementData;			// Element 데이터

	// 타입
	m_iType = df_TYPE_UI;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 가로 세로 길이
	m_iWidth = iWidth;
	m_iHeight = iHeight;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = df_RENDER_PRIORITY_LAST;
	// 선택된 요소
	m_SelectElement = NULL;

	//-------------------------------------
	// 메뉴 오브젝트 리스트
	//-------------------------------------
	m_MenuObjectList = new CList<ObjectUI *>;

	//-------------------------------------
	// 타이틀 바
	//-------------------------------------
	newObject = new UITitleBar(0xff69c3e2, m_iX, m_iY, m_iWidth, 40, 0.8, this);
	m_MenuObjectList->push_back(newObject);

	//-------------------------------------
	// 메뉴
	// 디폴트 선택
	//-------------------------------------
	m_iFocusMenu = df_FOCUS_MENU_TILE;
	m_iChangeMenu = df_FOCUS_MENU_EMPT;

	// 타일 탭
	newObject = new UITab(df_SPRITE_UI_MENU_TAB_TILE, m_iX + 5, m_iY + 45, df_FOCUS_MENU_TILE, this);
	m_MenuObjectList->push_back(newObject);
	// 물체 탭
	newObject = new UITab(df_SPRITE_UI_MENU_TAB_OBJECT, newObject->m_iRight + 5, m_iY + 45, df_FOCUS_MENU_OBJECT, this);
	m_MenuObjectList->push_back(newObject);
	// 적 탭
	newObject = new UITab(df_SPRITE_UI_MENU_TAB_ENEMY, newObject->m_iRight + 5, m_iY + 45, df_FOCUS_MENU_ENEMY, this);
	m_MenuObjectList->push_back(newObject);
	// 로케이션 탭
	newObject = new UITab(df_SPRITE_UI_MENU_TAB_LOCATION, newObject->m_iRight + 5, m_iY + 45, df_FOCUS_MENU_LOCATION, this);
	m_MenuObjectList->push_back(newObject);


	//-------------------------------------
	// UI 요소 리스트 클래스
	//-------------------------------------
	m_MenuElementList = new ListTileUnit(m_iX, m_iY + 80, m_iWidth, m_iHeight - 80, 25);
	m_MenuObjectList->push_back(m_MenuElementList);

	//-------------------------------------
	// UI 클래스에 리스트 추가
	//-------------------------------------
	//-------------------------------------
	// 타일 탭 요소들
	//-------------------------------------
	// 여기서 메모리 생성을 했지만 메모리 해제는 UI 리스트 클래스 에서 해제
	ElementVector = new vector<ObjectUI *>;
	// 리스트에 들어갈 요소 생성
	// 요소 데이터
	stElementData.iMouseState = df_MOUSE_STATE_TILE_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 1;
	stElementData.iElementType = df_TILE_TYPE_LIBRARY;
	stElementData.iElementState = df_TILE_STATE_BORDER;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	// UI 스프라이트 번호, 좌표 X, Y, 스크롤 top, bottop 값 변수 포인터, 스프라이트 사이즈 4배로 변경
	newObject = new UIElement(df_SPRITE_UI_MENU_LIBRARY_BORDER, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 4);
	ElementVector->push_back(newObject);
	stElementData.iMouseState = df_MOUSE_STATE_TILE_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 1;
	stElementData.iElementType = df_TILE_TYPE_LIBRARY;
	stElementData.iElementState = df_TILE_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_LIBRARY_FLOOR, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 4);
	ElementVector->push_back(newObject);
	stElementData.iMouseState = df_MOUSE_STATE_TILE_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 1;
	stElementData.iElementType = df_TILE_TYPE_CASTLE;
	stElementData.iElementState = df_TILE_STATE_BORDER;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_CASTLE_BORDER, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 4);
	ElementVector->push_back(newObject);
	stElementData.iMouseState = df_MOUSE_STATE_TILE_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_UI_TILE_POINTER;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 1;
	stElementData.iElementType = df_TILE_TYPE_CASTLE;
	stElementData.iElementState = df_TILE_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_CASTLE_FLOOR, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 4);
	ElementVector->push_back(newObject);

	// 스크롤 바 생성
	//위치 좌표 X, Y, 너비, 높이, 리스트 클래스 포인터
	ScrollBar = new UIScrollBar(m_MenuElementList->m_iRight - 25, m_MenuElementList->m_iTop, 25, m_MenuElementList->m_iElementListHeight, m_MenuElementList);
	// 구조체 값 셋
	stElementInfo.iListType = df_FOCUS_MENU_TILE;
	stElementInfo.ScrollBarPtr = ScrollBar;
	stElementInfo.ElementObjectVectorPtr = ElementVector;
	// 리스트 추가
	m_MenuElementList->AddElementListPtr(stElementInfo);

	//-------------------------------------
	// 물체 탭 요소들
	//-------------------------------------
	// 여기서 메모리 생성을 했지만 메모리 해제는 UI 리스트 클래스 에서 해제
	ElementVector = new vector<ObjectUI *>;
	// 리스트에 들어갈 요소 생성
	// 요소 데이터
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_OBJECT_LANTERN_CENTER_01;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_OBJECT_LANTERN_CENTER_03;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_LANTERN;
	stElementData.iElementState = df_UNIT_STATE_WALL;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	// UI 스프라이트 번호, 좌표 X, Y, 스크롤 top, bottop 값 변수 포인터, 스프라이트 사이즈 4배로 변경
	newObject = new UIElement(df_SPRITE_UI_MENU_OBJECT_LANTERN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 4);
	ElementVector->push_back(newObject);
	// 스크롤 바 생성
	//위치 좌표 X, Y, 너비, 높이, 리스트 클래스 포인터
	ScrollBar = new UIScrollBar(m_MenuElementList->m_iRight - 25, m_MenuElementList->m_iTop, 25, m_MenuElementList->m_iElementListHeight, m_MenuElementList);
	// 구조체 값 셋
	stElementInfo.iListType = df_FOCUS_MENU_OBJECT;
	stElementInfo.ScrollBarPtr = ScrollBar;
	stElementInfo.ElementObjectVectorPtr = ElementVector;
	// 리스트 추가
	m_MenuElementList->AddElementListPtr(stElementInfo);

	//-------------------------------------
	// 적 탭 요소들
	//-------------------------------------
	// 여기서 메모리 생성을 했지만 메모리 해제는 UI 리스트 클래스 에서 해제
	ElementVector = new vector<ObjectUI *>;
	// 리스트에 들어갈 요소 생성
	// 요소 데이터
	// 총탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BULLET_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BULLET_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BULLET_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BULLET_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 반다나
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BANDANA_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BANDANA_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BANDANA_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BANDANA_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 레드 산탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_RED_SHOTGUN_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_RED_SHOTGUN_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 블루 산탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BLUE_SHOTGUN_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BLUE_SHOTGUN_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 미라
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_MUMMY_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_MUMMY_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_MUMMY_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_MUMMY_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 벌레 산탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_SHOTGRUB_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_SHOTGRUB_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 유령 총탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_GHOST_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_GHOST_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_GHOST_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_GHOST_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 날라다니는 상어 총탄
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_FLY_KIN;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_FLY_KIN;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_FLY_KIN;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_FLY_KIN, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 새
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BIRD;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BIRD;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BIRD;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BIRD, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 큐브
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_CUBE;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_CUBE;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_CUBE;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_CUBE, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 녹색 책
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_GREEN_BOOK;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_GREEN_BOOK;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_GREEN_BOOK;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_GREEN_BOOK, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 파란 책
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BLUE_BOOK;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BLUE_BOOK;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BLUE_BOOK;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BLUE_BOOK, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 노란색 마술사
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_YELLOW_MAGIC;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_YELLOW_MAGIC, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 파란색 마술사
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_BLUE_MAGIC;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_BLUE_MAGIC, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 빨간색 마술사
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_RED_MAGIC;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_RED_MAGIC;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_RED_MAGIC;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_RED_MAGIC, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 위스프
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_WISP;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_WISP;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_WISP;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_WISP, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 버섯
	stElementData.iMouseState = df_MOUSE_STATE_OBJECT_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_ENEMY_MUSHROOM;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_ENEMY_MUSHROOM;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 4;
	stElementData.iElementType = df_UNIT_TYPE_ENEMY_MUSHROOM;
	stElementData.iElementState = df_UNIT_STATE_FLOOR;
	stElementData.iElementSpriteIncreaseVlaue = 4;
	newObject = new UIElement(df_SPRITE_UI_MENU_ENEMY_MUSHROOM, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 2);
	ElementVector->push_back(newObject);
	// 스크롤 바 생성
	//위치 좌표 X, Y, 너비, 높이, 리스트 클래스 포인터
	ScrollBar = new UIScrollBar(m_MenuElementList->m_iRight - 25, m_MenuElementList->m_iTop, 25, m_MenuElementList->m_iElementListHeight, m_MenuElementList);
	// 구조체 값 셋
	stElementInfo.iListType = df_FOCUS_MENU_ENEMY;
	stElementInfo.ScrollBarPtr = ScrollBar;
	stElementInfo.ElementObjectVectorPtr = ElementVector;
	// 리스트 추가
	m_MenuElementList->AddElementListPtr(stElementInfo);

	//-------------------------------------
	// 로케이션 탭 요소들
	// 로케이션은 스프라이트가 없습니다
	// 직접 사각형을 그립니다
	//-------------------------------------
	// 여기서 메모리 생성을 했지만 메모리 해제는 UI 리스트 클래스 에서 해제
	ElementVector = new vector<ObjectUI *>;
	// 리스트에 들어갈 요소 생성
	// 요소 데이터
	stElementData.iMouseState = df_MOUSE_STATE_LOCATION_MAKE;
	stElementData.stMousePointerSprite.iStartSprite = df_SPRITE_EMPT_SPRITE;
	stElementData.stMousePointerSprite.iEndSprite = df_SPRITE_EMPT_SPRITE;
	stElementData.stMousePointerSprite.iDelay = 0;
	stElementData.iMouseSpriteIncreaseVlaue = 1;
	stElementData.iElementType = df_UNIT_TYPE_LOCATION_RECTANGLE;
	stElementData.iElementState = df_UNIT_STATE_ALL;
	stElementData.iElementSpriteIncreaseVlaue = 1;
	// UI 스프라이트 번호, 좌표 X, Y, 스크롤 top, bottop 값 변수 포인터, 스프라이트 사이즈 1배로 변경
	newObject = new UIElement(df_SPRITE_UI_MENU_LOCATION_RECTANGLE, m_iX, m_iY, &m_MenuElementList->m_iScrollTop, &m_MenuElementList->m_iScrollBottom, &stElementData, this, 1);
	ElementVector->push_back(newObject);
	// 스크롤 바 생성
	//위치 좌표 X, Y, 너비, 높이, 리스트 클래스 포인터
	ScrollBar = new UIScrollBar(m_MenuElementList->m_iRight - 25, m_MenuElementList->m_iTop, 25, m_MenuElementList->m_iElementListHeight, m_MenuElementList);
	// 구조체 값 셋
	stElementInfo.iListType = df_FOCUS_MENU_LOCATION;
	stElementInfo.ScrollBarPtr = ScrollBar;
	stElementInfo.ElementObjectVectorPtr = ElementVector;
	// 리스트 추가
	m_MenuElementList->AddElementListPtr(stElementInfo);



	// 리스트 요소 갱신
	m_MenuElementList->ChangeElementList(m_iFocusMenu);
}


MenuTileUnit::~MenuTileUnit()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	//--------------------------------------
	// 메모리 정리
	//--------------------------------------
	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = m_MenuObjectList->erase(iter);
		delete ObjectTemp;
	}

	delete m_MenuObjectList;
}

bool MenuTileUnit::Action()
{
	if (m_iChangeMenu != df_FOCUS_MENU_EMPT) {
		m_iFocusMenu = m_iChangeMenu;
		// 리스트 요소 갱신
		m_MenuElementList->ChangeElementList(m_iFocusMenu);
		m_iChangeMenu = df_FOCUS_MENU_EMPT;
	}

	return false;
}

void MenuTileUnit::Draw()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	// 메뉴 영역
	g_cSprite->DrawBackground(0xffffff, m_iX, m_iY, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 0.5, false, true);

	// 메뉴 내부 오브젝트 처리
	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		if (ObjectTemp->Action()) {
			delete ObjectTemp;
		}
		ObjectTemp->Draw();
		++iter;
	}
}

bool MenuTileUnit::Click()
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;
	int iMouseX;
	int iMouseY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		iter = m_MenuObjectList->rbegin();
		iter_end = m_MenuObjectList->head();
		while (iter != iter_end) {
			ObjectTemp = *iter;
			if (ObjectTemp->Click()) {
				return true;
			}
			--iter;
		}
		return true;
	}

	return false;
}

void MenuTileUnit::Move(int iMoveValueX, int iMoveValueY)
{
	CList<ObjectUI*>::iterator iter;
	CList<ObjectUI*>::iterator iter_end;
	ObjectUI * ObjectTemp;

	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;

	iter = m_MenuObjectList->begin();
	iter_end = m_MenuObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		ObjectTemp->Move(iMoveValueX, iMoveValueY);
		++iter;
	}
}

bool MenuTileUnit::CheckOnMouse()
{
	int iMouseX;
	int iMouseY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		return true;
	}

	return false;
}
