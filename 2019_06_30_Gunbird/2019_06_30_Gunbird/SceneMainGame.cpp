#include "stdafx.h"
#include "SceneMainGame.h"
#include "Event.h"
#include "EventMainGame.h"


SceneMainGame::SceneMainGame()
{
	// 게임 중
	g_GameState = GAME;

	// 스프라이트 셋
	g_cSprite.Reset(300, 0x00ff00ff);
	LoadSprite();

	// 리스트
	m_EventList = new list<Event*>;
	// 이벤트 메시지 큐
	m_RequestEventQueue = new queue<EventMsg>;

	Event *newEvent;
	newEvent = new EventMainGame(this);
	m_EventList->push_back(newEvent);
	m_EventMainGame = (EventMainGame *)newEvent;
}


SceneMainGame::~SceneMainGame()
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

void SceneMainGame::Update()
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
		case GAME_START:
			// 게임 시작
			break;
		case GAME_OVER:
			g_sceneManager.m_change = SCENE_TITLE;
			break;
		case GAME_RESET:
			// 게임 다시 시작
			m_EventMainGame->Reset();
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

void SceneMainGame::LoadSprite()
{
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LIFE, L"image\\Alucard_Life.bmp", 0, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_1, L"image\\Alucard_Stand_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_2, L"image\\Alucard_Stand_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_3, L"image\\Alucard_Stand_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_4, L"image\\Alucard_Stand_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_5, L"image\\Alucard_Stand_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_STAND_6, L"image\\Alucard_Stand_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_1, L"image\\Alucard_Left_1_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_2, L"image\\Alucard_Left_1_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_3, L"image\\Alucard_Left_1_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_4, L"image\\Alucard_Left_1_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_5, L"image\\Alucard_Left_1_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_1_6, L"image\\Alucard_Left_1_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_1, L"image\\Alucard_Left_2_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_2, L"image\\Alucard_Left_2_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_3, L"image\\Alucard_Left_2_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_4, L"image\\Alucard_Left_2_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_5, L"image\\Alucard_Left_2_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_LEFT_2_6, L"image\\Alucard_Left_2_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_1, L"image\\Alucard_Right_1_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_2, L"image\\Alucard_Right_1_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_3, L"image\\Alucard_Right_1_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_4, L"image\\Alucard_Right_1_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_5, L"image\\Alucard_Right_1_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_1_6, L"image\\Alucard_Right_1_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_1, L"image\\Alucard_Right_2_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_2, L"image\\Alucard_Right_2_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_3, L"image\\Alucard_Right_2_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_4, L"image\\Alucard_Right_2_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_5, L"image\\Alucard_Right_2_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_RIGHT_2_6, L"image\\Alucard_Right_2_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_1, L"image\\Alucard_Charge_1.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_2, L"image\\Alucard_Charge_2.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_3, L"image\\Alucard_Charge_3.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_4, L"image\\Alucard_Charge_4.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_5, L"image\\Alucard_Charge_5.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_6, L"image\\Alucard_Charge_6.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_7, L"image\\Alucard_Charge_7.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_8, L"image\\Alucard_Charge_8.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_01, L"image\\Alucard_Boom_01.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_02, L"image\\Alucard_Boom_02.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_03, L"image\\Alucard_Boom_03.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_04, L"image\\Alucard_Boom_04.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_05, L"image\\Alucard_Boom_05.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_06, L"image\\Alucard_Boom_06.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_07, L"image\\Alucard_Boom_07.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_08, L"image\\Alucard_Boom_08.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_09, L"image\\Alucard_Boom_09.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_10, L"image\\Alucard_Boom_10.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_11, L"image\\Alucard_Boom_11.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_12, L"image\\Alucard_Boom_12.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_13, L"image\\Alucard_Boom_13.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_14, L"image\\Alucard_Boom_14.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_15, L"image\\Alucard_Boom_15.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_16, L"image\\Alucard_Boom_16.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_17, L"image\\Alucard_Boom_17.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_18, L"image\\Alucard_Boom_18.bmp", 208, 47);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_19, L"image\\Alucard_Boom_19.bmp", 208, 47);

	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_EFFECT_1, L"image\\Alucard_Boom_Effect_1.bmp", 350, 500);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BOOM_EFFECT_2, L"image\\Alucard_Boom_Effect_2.bmp", 350, 500);

	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_DIE, L"image\\Alucard_Die.bmp", 52, 0);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_1_1, L"image\\Alucard_Bullet_1_1.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_1_2, L"image\\Alucard_Bullet_1_2.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_1_3, L"image\\Alucard_Bullet_1_3.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_2_1, L"image\\Alucard_Bullet_2_1.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_2_2, L"image\\Alucard_Bullet_2_2.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_3_1, L"image\\Alucard_Bullet_3_1.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_3_2, L"image\\Alucard_Bullet_3_2.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_3_3, L"image\\Alucard_Bullet_3_3.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_BULLET_3_4, L"image\\Alucard_Bullet_3_4.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_0, L"image\\Alucard_Charge_Bullet_0.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_15, L"image\\Alucard_Charge_Bullet_15.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_45, L"image\\Alucard_Charge_Bullet_45.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_60, L"image\\Alucard_Charge_Bullet_60.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_75, L"image\\Alucard_Charge_Bullet_75.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_90, L"image\\Alucard_Charge_Bullet_90.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_105, L"image\\Alucard_Charge_Bullet_105.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_120, L"image\\Alucard_Charge_Bullet_120.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_135, L"image\\Alucard_Charge_Bullet_135.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_165, L"image\\Alucard_Charge_Bullet_165.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_180, L"image\\Alucard_Charge_Bullet_180.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_195, L"image\\Alucard_Charge_Bullet_195.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_225, L"image\\Alucard_Charge_Bullet_225.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_255, L"image\\Alucard_Charge_Bullet_255.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_270, L"image\\Alucard_Charge_Bullet_270.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_285, L"image\\Alucard_Charge_Bullet_285.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_300, L"image\\Alucard_Charge_Bullet_300.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_315, L"image\\Alucard_Charge_Bullet_315.bmp", 50, 50);
	g_cSprite.LoadDibSprite(df_SPRITE_ALUCARD_CHARGE_BULLET_345, L"image\\Alucard_Charge_Bullet_345.bmp", 50, 50);

	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_CENTER_1, L"image\\Enemy_Blue_Center_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_CENTER_2, L"image\\Enemy_Blue_Center_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_1_1, L"image\\Enemy_Blue_Left_1_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_1_2, L"image\\Enemy_Blue_Left_1_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_2_1, L"image\\Enemy_Blue_Left_2_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_2_2, L"image\\Enemy_Blue_Left_2_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_3_1, L"image\\Enemy_Blue_Left_3_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_LEFT_3_2, L"image\\Enemy_Blue_Left_3_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_1_1, L"image\\Enemy_Blue_Right_1_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_1_2, L"image\\Enemy_Blue_Right_1_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_2_1, L"image\\Enemy_Blue_Right_2_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_2_2, L"image\\Enemy_Blue_Right_2_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_3_1, L"image\\Enemy_Blue_Right_3_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BLUE_RIGHT_3_2, L"image\\Enemy_Blue_Right_3_2.bmp", 62, 88);

	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_CENTER_1, L"image\\Enemy_Red_Center_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_CENTER_2, L"image\\Enemy_Red_Center_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_LEFT_1_1, L"image\\Enemy_Red_Left_1_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_LEFT_1_2, L"image\\Enemy_Red_Left_1_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_LEFT_2_1, L"image\\Enemy_Red_Left_2_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_LEFT_2_2, L"image\\Enemy_Red_Left_2_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_RIGHT_1_1, L"image\\Enemy_Red_Right_1_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_RIGHT_1_2, L"image\\Enemy_Red_Right_1_2.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_RIGHT_2_1, L"image\\Enemy_Red_Right_2_1.bmp", 62, 88);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_RED_RIGHT_2_2, L"image\\Enemy_Red_Right_2_2.bmp", 62, 88);

	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_1_1, L"image\\Enemy_Bullet_1_1.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_1_2, L"image\\Enemy_Bullet_1_2.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_1_3, L"image\\Enemy_Bullet_1_3.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_RELEASE_1_1, L"image\\Enemy_Bullet_Release_1_1.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_RELEASE_1_2, L"image\\Enemy_Bullet_Release_1_2.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_RELEASE_1_3, L"image\\Enemy_Bullet_Release_1_3.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_RELEASE_1_4, L"image\\Enemy_Bullet_Release_1_4.bmp", 10, 10);
	g_cSprite.LoadDibSprite(df_SPRITE_ENEMY_BULLET_RELEASE_1_5, L"image\\Enemy_Bullet_Release_1_5.bmp", 10, 10);


	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_1, L"image\\Item_PowerUp_1.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_2, L"image\\Item_PowerUp_2.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_3, L"image\\Item_PowerUp_3.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_4, L"image\\Item_PowerUp_4.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_5, L"image\\Item_PowerUp_5.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_6, L"image\\Item_PowerUp_6.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_7, L"image\\Item_PowerUp_7.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_1, L"image\\Item_PowerUp_1.bmp", 31, 21);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_STRING_1, L"image\\Item_PowerUp_String_1.bmp", 56, 38);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_POWER_UP_STRING_2, L"image\\Item_PowerUp_String_2.bmp", 56, 38);

	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_1, L"image\\Kaboom_1_1.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_2, L"image\\Kaboom_1_2.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_3, L"image\\Kaboom_1_3.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_4, L"image\\Kaboom_1_4.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_5, L"image\\Kaboom_1_5.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_6, L"image\\Kaboom_1_6.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_7, L"image\\Kaboom_1_7.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_8, L"image\\Kaboom_1_8.bmp", 75, 83);
	g_cSprite.LoadDibSprite(df_SPRITE_ITEM_KABOOM_1_9, L"image\\Kaboom_1_9.bmp", 75, 83);

	g_cSprite.LoadDibSprite(df_SPRITE_STAGE_1, L"image\\Stage_1.bmp", 0, 8900);
}
