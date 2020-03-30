#include "stdafx.h"
#include "EventTitle.h"
#include "SceneTitleAndSelect.h"
#include "CQueue.h"
#include "Object.h"
#include "ObjectPicture.h"

EventTitle::EventTitle(SceneTitleAndSelect * ScenePtr)
{
	Object *newObject;

	// 타이틀 화면임
	g_GameState = TITLE;

	// 멤버변수 초기화
	m_type = df_TYPE_EVENT_TITLE;
	m_iCount = 0;
	m_bNum_5 = true;
	m_bNum_1 = true;
	m_bStartFlag = false;
	m_ScenePtr = ScenePtr;
	m_ObjectList = new list<Object *>;
	m_KeyQueue = new CQueue;

	// 타이틀 그림
	newObject = new ObjectPicture(0, 0, df_SPRITE_TITLE_1);
	m_ObjectList->push_back(newObject);
	// 게임 스타트
	newObject = new ObjectPicture(df_WIN_RIGHT / 2, 700, df_SPRITE_START);
	m_ObjectList->push_back(newObject);
	m_GameStartPicture = (ObjectPicture *)newObject;
}


EventTitle::~EventTitle()
{
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	iter = m_ObjectList->begin();
	iter_end = m_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = m_ObjectList->erase(iter);
		delete ObjectTemp;
	}

	delete m_ObjectList;
	delete m_KeyQueue;
}

void EventTitle::KeyProcess()
{
	//윈도우 활성화 확인
	if (!g_bActiveApp) {
		return;
	}

	// 5 키
	if (GetAsyncKeyState(0x35) & 0x8000) {
		if (!m_bNum_5) {
			m_bNum_5 = true;
			m_KeyQueue->Enqueue(df_NUM_5);
		}
		
	}
	else {
		m_bNum_5 = false;
	}
	// 1 키
	if (GetAsyncKeyState(0x31) & 0x8000) {
		if (!m_bNum_1) {
			m_bNum_1 = true;
			m_KeyQueue->Enqueue(df_NUM_1);
		}
	}
	else {
		m_bNum_1 = false;
	}
}

bool EventTitle::Update()
{
	int iKey;

	// 키입력 처리
	// 메세지가 없을때까지 반복
	while (m_KeyQueue->Dequeue(&iKey)) {
		switch (iKey) {
		case df_NUM_5:
			++g_iCoin;
			break;
		case df_NUM_1:
			// 코인이 있으면 게임 시작
			if (g_iCoin > 0) {
				--g_iCoin;
				m_ScenePtr->m_RequestEventQueue->push(TITLE_SELECT);
				return true;
			}
			break;
		}
	}

	// 게임 스타트 이미지 글자 띄울지 말지 여부
	if (g_iCoin > 0) {
		++m_iCount;

		if (m_iCount >= 100) {
		m_iCount = 0;
		m_GameStartPicture->m_bDrawFlag = true;
		}
		else if (m_iCount > 50) {
			m_GameStartPicture->m_bDrawFlag = false;
		}
		else {
			m_GameStartPicture->m_bDrawFlag = true;
		}
	}
	else {
		m_GameStartPicture->m_bDrawFlag = false;
	}


	return false;
}

void EventTitle::Draw()
{
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	iter = m_ObjectList->begin();
	iter_end = m_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		if (ObjectTemp->Action()) {
			iter = m_ObjectList->erase(iter);
			delete ObjectTemp;
			continue;
		}
		ObjectTemp->Draw();
		++iter;
	}
}
