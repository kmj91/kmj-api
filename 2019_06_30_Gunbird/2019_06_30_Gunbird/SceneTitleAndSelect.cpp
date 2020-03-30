#include "stdafx.h"
#include "SceneTitleAndSelect.h"
#include "Event.h"
#include "EventTitle.h"
#include "EventSelect.h"


SceneTitleAndSelect::SceneTitleAndSelect()
{
	// 스프라이트 셋
	g_cSprite.Reset(100, 0x00ff00ff);
	LoadSprite();

	// 리스트
	m_EventList = new list<Event*>;
	// 이벤트 메시지 큐
	m_RequestEventQueue = new queue<EventMsg>;

	// 타이틀 화면
	Event *newEvent;
	newEvent = new EventTitle(this);
	m_EventList->push_back(newEvent);
}


SceneTitleAndSelect::~SceneTitleAndSelect()
{
	list<Event*>::iterator iter;
	list<Event*>::iterator iter_end;
	Event * EventTemp;

	iter = m_EventList->begin();
	iter_end = m_EventList->end();
	while (iter != iter_end) {
		EventTemp = *iter;
		iter = m_EventList->erase(iter);
		delete EventTemp;
	}
}

void SceneTitleAndSelect::Update()
{
	Event *EventTemp;					// 이벤트 리스트
	list<Event *>::iterator iter;		// 이터레이터
	list<Event *>::iterator iter_end;	// 이터레이터 끝
	int iCnt;
	int iSize;

	// 큐가 빌때까지 반복
	iCnt = 0;
	iSize = m_RequestEventQueue->size();
	while (iCnt < iSize) {
		switch (m_RequestEventQueue->front()) {
		case TITLE_SELECT:
			// 게임 시작 캐릭터 선택 화면으로
			EventTemp = new EventSelect(this);
			m_EventList->push_back(EventTemp);
			break;
		case GAME_START:
			// 게임 시작
			g_sceneManager.m_change = SCENE_GAME;
			break;
		}
		m_RequestEventQueue->pop();
		++iCnt;
	}


	iter = m_EventList->begin();
	iter_end = m_EventList->end();
	while (iter != iter_end) {
		EventTemp = *iter;
		EventTemp->KeyProcess();
		if (EventTemp->Update()) {
			iter = m_EventList->erase(iter);
			delete EventTemp;
			continue;
		}
		EventTemp->Draw();
		++iter;
	}


	//------------------------------------------------------------
	// ScreenDib 를 화면으로 플립
	//------------------------------------------------------------
	g_cScreenDib.DrawBuffer(g_hWnd);
}

void SceneTitleAndSelect::LoadSprite()
{
	g_cSprite.LoadDibSprite(df_SPRITE_TITLE_1, L"image\\Title_1.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_START, L"image\\Start.bmp", 105, 22);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_SKY, L"image\\Select_Sky.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_BAR, L"image\\Select_Bar.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_1P, L"image\\Select_1P.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_1, L"image\\Select_Alucard_1.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_2, L"image\\Select_Alucard_2.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_3, L"image\\Select_Alucard_3.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_4, L"image\\Select_Alucard_4.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_5, L"image\\Select_Alucard_5.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_MARION_1, L"image\\Select_Marion_1.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_MARION_2, L"image\\Select_Marion_2.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_MARION_3, L"image\\Select_Marion_3.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_MARION_4, L"image\\Select_Marion_4.bmp", 0, 0);

	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_1, L"image\\Alucard_Stand_1.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_2, L"image\\Alucard_Stand_2.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_3, L"image\\Alucard_Stand_3.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_4, L"image\\Alucard_Stand_4.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_5, L"image\\Alucard_Stand_5.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_ALUCARD_STAND_6, L"image\\Alucard_Stand_6.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_SELECT_MARION_STAND, L"image\\Marion_Stand.bmp", 0, 0);
	
}
