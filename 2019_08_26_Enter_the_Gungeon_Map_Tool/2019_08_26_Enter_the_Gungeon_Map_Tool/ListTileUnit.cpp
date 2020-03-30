#include "stdafx.h"
#include "ListTileUnit.h"
#include "UIElement.h"

//--------------------------------------------------
// ListTileUnit 생성자
// 위치 좌표 X, Y, 가로 세로 길이, 스크롤 바 길이
//--------------------------------------------------
ListTileUnit::ListTileUnit(int iPosX, int iPosY, int iWidth, int iHeight, int iScrollBarWidth)
{
	ObjectUI * newObject;

	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	//-------------------------------------
	// 메뉴 요소 목록 가로 세로 길이
	// 스크롤바 가로 길이를 뺌
	//-------------------------------------
	m_iElementListWidth = iWidth - iScrollBarWidth;
	m_iElementListHeight = iHeight;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + iWidth;
	m_iBottom = m_iTop + iHeight;
	// 그리기 우선순위
	m_iRenderPriority = 0;

	// UI 리스트가 가지고있는 요소 정보 구조체를 저장하는 벡터
	m_ElmentInfoVector = new vector<st_UIElementInfo>;

	// 스크롤 범위
	m_iScrollTop = m_iTop;
	m_iScrollBottom = m_iBottom;
}


ListTileUnit::~ListTileUnit()
{
	int iCnt;
	int iSize;
	int iElementCnt;
	int iElementSize;
	vector<ObjectUI*> * ElementVector;
	ObjectUI ** ObjectArray;

	//--------------------------------------
	// 메모리 정리
	//--------------------------------------
	iSize = m_ElmentInfoVector->size();
	iCnt = 0;
	while (iCnt < iSize) {
		ElementVector = (*m_ElmentInfoVector)[iCnt].ElementObjectVectorPtr;
		
		iElementSize = ElementVector->size();
		ObjectArray = ElementVector->data();
		iElementCnt = 0;
		while (iElementCnt < iElementSize) {
			// UIElement 메모리 정리
			delete ObjectArray[iElementCnt];
			++iElementCnt;
		}

		// UIElement 벡터 정리
		delete ElementVector;

		++iCnt;
	}
	// 구조체 벡터 정리
	delete m_ElmentInfoVector;
}

bool ListTileUnit::Action()
{
	return false;
}

void ListTileUnit::Draw()
{
	int iCnt;
	int iSize;
	ObjectUI **ObjectArray;
	ObjectUI *Object;


	// 스크롤 바
	m_FocusScrollBar->Action();
	m_FocusScrollBar->Draw();


	// 메뉴 내부 오브젝트 처리
	iSize = m_UIElementObjectVector->size();
	ObjectArray = m_UIElementObjectVector->data();
	iCnt = 0;
	while (iCnt < iSize) {
		Object = ObjectArray[iCnt];

		// 메뉴 리스트 스크롤 벗어나면 출력 ㄴㄴ
		if (Object->m_iBottom < m_iScrollTop) {
			++iCnt;
			continue;
		}
		else if (Object->m_iTop > m_iScrollBottom) {
			++iCnt;
			continue;
		}

		Object->Action();
		Object->Draw();
		++iCnt;
	}
}

bool ListTileUnit::Click()
{
	int iSize;
	int iMouseX;
	int iMouseY;
	ObjectUI **ObjectArray;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	//--------------------------------------
	iMouseX = g_iUIMouseX;
	iMouseY = g_iUIMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		m_FocusScrollBar->Click();

		iSize = m_UIElementObjectVector->size();
		--iSize;
		ObjectArray = m_UIElementObjectVector->data();
		while (iSize >= 0) {
			if (ObjectArray[iSize]->Click()) {
				return true;
			}
			--iSize;
		}

		return true;
	}

	return false;
}

void ListTileUnit::Move(int iMoveValueX, int iMoveValueY)
{
	int iCnt;
	int iSize;
	ObjectUI **ObjectArray;
	st_UIElementInfo *stpElemenInfoArray;

	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;

	// 스크롤 범위
	m_iScrollTop = m_iTop;
	m_iScrollBottom = m_iBottom;

	// 스크롤 바
	////m_FocusScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_TileScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_StageScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_PlayerScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_ObjectScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_BrokenScrollBar->Move(iMoveValueX, iMoveValueY);
	//m_EnemyScrollBar->Move(iMoveValueX, iMoveValueY);

	// 스크롤 바
	iSize = m_ElmentInfoVector->size();
	stpElemenInfoArray = m_ElmentInfoVector->data();
	iCnt = 0;
	while (iCnt < iSize) {
		stpElemenInfoArray[iCnt].ScrollBarPtr->Move(iMoveValueX, iMoveValueY);
		++iCnt;
	}

	iSize = m_UIElementObjectVector->size();
	ObjectArray = m_UIElementObjectVector->data();
	iCnt = 0;
	while (iCnt < iSize) {
		ObjectArray[iCnt]->Move(iMoveValueX, iMoveValueY);
		++iCnt;
	}
}

void ListTileUnit::AddElementListPtr(st_UIElementInfo stElementListInfo)
{
	m_ElmentInfoVector->push_back(stElementListInfo);
}

void ListTileUnit::ScrollMove(int iMoveValueY)
{
	int iCnt;
	int iSize;
	ObjectUI **ObjectArray;
	int iMoveValue;

	iMoveValue = -(iMoveValueY * m_dScrollMoveValue);

	iSize = m_UIElementObjectVector->size();
	ObjectArray = m_UIElementObjectVector->data();
	iCnt = 0;
	while (iCnt < iSize) {
		ObjectArray[iCnt]->Move(0, iMoveValue);
		++iCnt;
	}
}

// 메뉴의 리스트 영역을 교체 한다
void ListTileUnit::ChangeElementList(int iFocusMenu)
{
	int iSize;
	int iCnt;
	int iElementPosX;				// 아이콘 위치 X
	int iElementPosY;				// 아이콘 위치 Y
	int iElementWidth;				// 가로 길이
	int iElementHeight;				// 세로 길이
	int iLineFeedCnt;				// 메뉴 요소 줄바꿈
	int iCntHeight;					// 요소 세로 개수 (메뉴 크기 안에 들어오는 요소 세로 개수)
	int iScrollBarSize;				// 메뉴 요소 세로축 사이즈에 비례한 스크롤 크기
	double dTemp;
	st_UIElementInfo *stpElemenInfoArray;
	UIElement** ObjectElementArray;

	iSize = m_ElmentInfoVector->size();
	stpElemenInfoArray = m_ElmentInfoVector->data();
	iCnt = 0;
	while (iCnt < iSize) {
		// 교체 리스트 타입이 아니면 
		if (stpElemenInfoArray[iCnt].iListType != iFocusMenu) {
			++iCnt;
			continue;
		}

		//--------------------------------------
		// 메뉴 요소 목록
		// 매개변수의 요청받은 타입으로
		// 스크롤 바 교체해줌
		// 리스트 포인터 교체해줌
		//--------------------------------------
		m_FocusScrollBar = stpElemenInfoArray[iCnt].ScrollBarPtr;
		m_UIElementObjectVector = stpElemenInfoArray[iCnt].ElementObjectVectorPtr;

		iSize = m_UIElementObjectVector->size();
		ObjectElementArray = (UIElement**)m_UIElementObjectVector->data();

		// 요소가 하나도 없음
		if (iSize == 0) {
			m_FocusScrollBar->ChangeScrollBarSize(0, false);
			break;
		}

		// 처음 아이콘 위치
		iElementPosX = 5;
		iElementPosY = 5;


		//--------------------------------------
		// 루프를 돌면서
		// 리스트 범위 안에 Element 좌표 배치
		//--------------------------------------
		iCnt = 0;
		iLineFeedCnt = 1;
		iCntHeight = 1;
		iElementHeight = 0;		// 초기값 (max() 때문에)
		while (iCnt < iSize) {

			iElementWidth = 5 + ObjectElementArray[iCnt]->m_iSpriteWidth;
			//iElementHeight = 5 + ObjectElementArray[iCnt]->m_iSpriteHeight;
			iElementHeight = max(iElementHeight, 5 + ObjectElementArray[iCnt]->m_iSpriteHeight);

			if (iElementPosX + iElementWidth >= m_iElementListWidth) {
				iElementPosX = 5;
				iElementPosY = iElementPosY + iElementHeight;
				++iLineFeedCnt;

				// 요소 세로 개수 (메뉴 크기 안에 들어오는 요소 세로 개수)
				// 메뉴 세로 크기를 넘어가면 증감하지 않음
				if (iElementPosY < m_iElementListHeight) {
					// 증감
					++iCntHeight;
				}
			}

			ObjectElementArray[iCnt]->SetPosition(m_iX + iElementPosX, m_iY + iElementPosY);
			iElementPosX = iElementPosX + iElementWidth;

			++iCnt;
		}

		// 스크롤 바 크기
		if (iLineFeedCnt <= iCntHeight) {
			iScrollBarSize = m_iElementListHeight;
		}
		else {
			dTemp = (double)iLineFeedCnt / (double)iCntHeight;
			if (dTemp < 2) {
				iScrollBarSize = m_iElementListHeight / 2;
				iScrollBarSize = iScrollBarSize * dTemp;
			}
			else {
				iScrollBarSize = m_iElementListHeight / dTemp;
			}

		}

		// 스크롤 이동 값
		m_dScrollMoveValue = (double)iLineFeedCnt / (double)iCntHeight;

		if (iScrollBarSize != m_iElementListHeight) {
			m_FocusScrollBar->ChangeScrollBarSize(iScrollBarSize, true);
		}
		else {
			m_FocusScrollBar->ChangeScrollBarSize(iScrollBarSize, false);
		}

		// 스크롤 위치로 목록 요소 재 배치
		ScrollMove(m_iScrollTop - m_FocusScrollBar->m_iScrollBarTop);

		// 찾았음 반복문 종료
		break;
	}
}
