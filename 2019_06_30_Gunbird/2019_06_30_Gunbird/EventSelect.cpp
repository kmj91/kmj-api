#include "stdafx.h"
#include "EventSelect.h"
#include "SceneTitleAndSelect.h"
#include "CQueue.h"
#include "Object.h"
#include "ObjectPicture.h"
#include "ObjectAnimation.h"
#include "ObjectAlucard.h"

EventSelect::EventSelect(SceneTitleAndSelect * ScenePtr)
{
	Object *newObject;

	// 타이틀 화면임
	g_GameState = SELECT;

	// 멤버변수 초기화
	m_type = df_TYPE_EVENT_SELECT;
	m_bNum_5 = true;
	m_bNum_1 = true;
	m_ScenePtr = ScenePtr;
	m_ObjectList = new list<Object *>;
	m_KeyQueue = new CQueue;

	// 셀렉트 화면
	newObject = new ObjectPicture(0, 0, df_SPRITE_TITLE_1);
	m_ObjectList->push_back(newObject);

	// 하늘 배경
	newObject = new ObjectPicture(0, 0, df_SPRITE_SELECT_SKY);
	m_ObjectList->push_back(newObject);

	// 선택한 캐릭터
	newObject = new ObjectAnimation(0, 0, df_SPRITE_SELECT_ALUCARD_1, df_SPRITE_SELECT_ALUCARD_5, df_DELAY_SELECT);
	m_ObjectList->push_back(newObject);
	m_Select = (ObjectAnimation *)newObject;

	// 선택 창
	newObject = new ObjectPicture(0, 0, df_SPRITE_SELECT_BAR, true);
	m_ObjectList->push_back(newObject);

	// 알루카드
	newObject = new ObjectAnimation(40, 800, df_SPRITE_SELECT_ALUCARD_STAND_1, df_SPRITE_SELECT_ALUCARD_STAND_6, df_DELAY_ACTION);
	m_ObjectList->push_back(newObject);

	// 마리온
	newObject = new ObjectPicture(180, 800, df_SPRITE_SELECT_MARION_STAND);
	m_ObjectList->push_back(newObject);

	// 1P 선택
	newObject = new ObjectPicture(40, 770, df_SPRITE_SELECT_1P);
	m_ObjectList->push_back(newObject);
	m_1P = (ObjectPicture *)newObject;
}


EventSelect::~EventSelect()
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

void EventSelect::KeyProcess()
{
	//윈도우 활성화 확인
	if (!g_bActiveApp) {
		return;
	}

	// 왼쪽
	if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_LL);
	}
	// 오른쪽
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_RR);
	}

	// A 키
	if (GetAsyncKeyState(0x41) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_ATTACK1);
	}
	// B 키
	if (GetAsyncKeyState(0x53) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_ATTACK2);
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

bool EventSelect::Update()
{
	int iKey;

	// 키입력 처리
	// 메세지가 없을때까지 반복
	while (m_KeyQueue->Dequeue(&iKey)) {
		switch (iKey) {
		case df_ACTION_MOVE_LL:
			m_1P->m_X = 40;
			m_Select->m_iSpriteIndex = df_SPRITE_SELECT_ALUCARD_1;
			m_Select->m_iAnimeStart = df_SPRITE_SELECT_ALUCARD_1;
			m_Select->m_iAnimeEnd = df_SPRITE_SELECT_ALUCARD_5;
			m_Select->m_iAnimeCount = 0;
			break;
		case df_ACTION_MOVE_RR:
			m_1P->m_X = 155;
			m_Select->m_iSpriteIndex = df_SPRITE_SELECT_MARION_1;
			m_Select->m_iAnimeStart = df_SPRITE_SELECT_MARION_1;
			m_Select->m_iAnimeEnd = df_SPRITE_SELECT_MARION_4;
			m_Select->m_iAnimeCount = 0;
			break;
		case df_ACTION_ATTACK1:
		case df_ACTION_ATTACK2:
		case df_NUM_1:
			if (m_1P->m_X == 40) {
				m_ScenePtr->m_RequestEventQueue->push(GAME_START);
			}
			break;
		case df_NUM_5:
			++g_iCoin;
			break;
		}
	}

	return false;
}

void EventSelect::Draw()
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
