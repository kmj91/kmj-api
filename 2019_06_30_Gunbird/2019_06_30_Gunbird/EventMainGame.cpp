#include "stdafx.h"
#include "EventMainGame.h"
#include "SceneMainGame.h"
#include "CQueue.h"
#include "Object.h"
#include "ObjectPlayer.h"
#include "ObjectAlucard.h"
#include "ObjectBackground.h"
#include "ObjectEnemyRed.h"
#include "ObjectEnemyBlue.h"

EventMainGame::EventMainGame(SceneMainGame * ScenePtr)
{
	// 멤버변수 초기화
	m_type = df_TYPE_EVENT_MAIN_GAME;
	// 스테이지 1
	m_iStage = 1;
	m_bNum_5 = true;
	m_bNum_1 = true;
	m_ScenePtr = ScenePtr;
	m_ObjectList = new list<Object *>;
	m_KeyQueue = new CQueue;
	//----------------------------------
	// 오브젝트 리스트 전역변수로
	//  * 맘에 안듬...
	//----------------------------------
	g_ObjectList = m_ObjectList;


	Init();
}

EventMainGame::~EventMainGame()
{
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;
	Object * ObjectTemp;

	iter = m_ObjectList->begin();
	iter_end = m_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = m_ObjectList->erase(iter);
		if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY_BULLET) {
			g_ObjectBulletPool.Free((ObjectBullet *)ObjectTemp);
		}
		else {
			delete ObjectTemp;
		}
	}

	delete m_ObjectList;
	delete m_KeyQueue;
}


// 키 처리
// 큐에 입력된 키를 넣는다
void EventMainGame::KeyProcess()
{
	//윈도우 활성화 확인
	if (!g_bActiveApp) {
		return;
	}

	// 캐릭터 이동 관련 키 입력
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8000)) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_LU);
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_UP) & 0x8000)) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_RU);
	}
	else if ((GetAsyncKeyState(VK_LEFT) & 0x8000) && (GetAsyncKeyState(VK_DOWN) & 0x8000)) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_LD);
	}
	else if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) && (GetAsyncKeyState(VK_DOWN) & 0x8000)) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_RD);
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_UU);
	}
	else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_DD);
	}
	else if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_LL);
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_MOVE_RR);
	}
	else {
		m_KeyQueue->Enqueue(df_ACTION_STAND);
	}

	// A 키
	if (GetAsyncKeyState(0x41) & 0x8000) {
		m_KeyQueue->Enqueue(df_ACTION_ATTACK1);
	}
	//if (GetKeyState(0x41) & 0x0001) {
	else {
		m_KeyQueue->Enqueue(df_ACTION_ATTACK1_END);
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

// 게임 로직
bool EventMainGame::Update()
{
	int iKey;			// 키값
	Object *newObject;

	// 키입력 처리
	// 메세지가 없을때까지 반복
	while (m_KeyQueue->Dequeue(&iKey)) {
		switch (iKey) {
		case df_ACTION_MOVE_LU:
		case df_ACTION_MOVE_RU:
		case df_ACTION_MOVE_LD:
		case df_ACTION_MOVE_RD:
		case df_ACTION_MOVE_UU:
		case df_ACTION_MOVE_DD:
		case df_ACTION_MOVE_LL:
		case df_ACTION_MOVE_RR:
		case df_ACTION_STAND:
		case df_ACTION_ATTACK1:
		case df_ACTION_ATTACK1_END:
		case df_ACTION_ATTACK2:
			if (g_Player != NULL) {
				g_Player->KeyAction(iKey);
			}
			break;
		case df_NUM_5:
			++g_iCoin;
			break;
		case df_NUM_1:
			// 코인이 있고
			if (g_iCoin > 0) {
				// 플레이어 죽어있으면 부활
				if (g_Player == NULL) {
					--g_iCoin;
					// 플레이어 생성
					newObject = new ObjectAlucard();
					m_ObjectList->push_back(newObject);
					g_Player = (ObjectPlayer *)newObject;		// 전역변수로

					g_iContinueCnt = 9;
					g_bGameOver = false;
				}
			}
			break;
		}
	}

	// 게임 오버 상태
	if (g_Player == NULL) {
		g_bGameOver = true;
	}

	if (g_iContinueCnt == 0) {
		m_ScenePtr->m_RequestEventQueue->push(GAME_OVER);
	}

	return false;
}

// 드로우
void EventMainGame::Draw()
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
			if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY_BULLET) {
				g_ObjectBulletPool.Free((ObjectBullet *)ObjectTemp);
			}
			else {
				delete ObjectTemp;
			}
			continue;
		}
		ObjectTemp->Draw();
		++iter;
	}
}

void EventMainGame::Init()
{
	Object *newObject;
	FILE *fp;
	EnemyManager m_EnemyInfoArray[200];		// 적 정보 배열

	switch (m_iStage) {
	case 1:
		// 배경
		newObject = new ObjectBackground(df_WIN_LEFT, df_WIN_BOTTOM);
		m_ObjectList->push_back(newObject);

		// 현제 배경 오브젝트
		g_StageBackground = newObject;
		
		// 스테이지 적 배치 읽기
		_wfopen_s(&fp, L"data\\Stage01_Enemy.ini", L"rt");
		if (fp == NULL) {
			throw;
		}

		char str[256];
		char *token;	//잘라낸 문자
		char *nextToken;
		int cnt = 0;	//위치

		while (!feof(fp)) {
			// 한줄을 읽는다
			fgets(str, sizeof(str), fp);
			// 콤마를 기준으로 잘라냄
			// 생성 시간
			token = strtok_s(str, ",", &nextToken);
			m_EnemyInfoArray[cnt].iAppearance = atoi(token);
			// 나갈 시간
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iOutTime = atoi(token);
			// 생성 좌표 X
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iPosX = atoi(token);
			// 생성 좌표 Y
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iPosY = atoi(token);
			// 생성되고 배치될 목표 좌표 X
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iDestPosX = atoi(token);
			// 생성되고 배치될 목표 좌표 Y
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iDestPosY = atoi(token);
			// 배치 될 때 선회 각도
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].fDestAngle = atof(token);
			// 화면 밖으로 퇴장할 때 좌표 X
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iOutPosX = atoi(token);
			// 화면 밖으로 퇴장할 때 좌표 Y
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].iOutPosY = atoi(token);
			// 퇴장 할 때 선회 각도
			token = strtok_s(NULL, ",", &nextToken);
			m_EnemyInfoArray[cnt].fOutAngle = atof(token);
			// 적 타입
			token = strtok_s(NULL, "\n", &nextToken);
			if (!strcmp(token, "RED")) {
				m_EnemyInfoArray[cnt].enType = ENEMY_RED;
			}
			else if (!strcmp(token, "BLUE")) {
				m_EnemyInfoArray[cnt].enType = ENEMY_BLUE;
			}
			else {
				++cnt;
				continue;
				//throw;
				//return false;
			}

			if (m_EnemyInfoArray[cnt].enType == ENEMY_RED) {
				newObject = new ObjectEnemyRed(m_EnemyInfoArray[cnt].iAppearance, m_EnemyInfoArray[cnt].iOutTime,
					m_EnemyInfoArray[cnt].iPosX, m_EnemyInfoArray[cnt].iPosY, m_EnemyInfoArray[cnt].iDestPosX, m_EnemyInfoArray[cnt].iDestPosY, 
					m_EnemyInfoArray[cnt].fDestAngle, m_EnemyInfoArray[cnt].iOutPosX, m_EnemyInfoArray[cnt].iOutPosY, m_EnemyInfoArray[cnt].fOutAngle);
				m_ObjectList->push_back(newObject);
			}
			else if (m_EnemyInfoArray[cnt].enType == ENEMY_BLUE) {
				newObject = new ObjectEnemyBlue(m_EnemyInfoArray[cnt].iAppearance, m_EnemyInfoArray[cnt].iOutTime,
					m_EnemyInfoArray[cnt].iPosX, m_EnemyInfoArray[cnt].iPosY, m_EnemyInfoArray[cnt].iDestPosX, m_EnemyInfoArray[cnt].iDestPosY,
					m_EnemyInfoArray[cnt].fDestAngle, m_EnemyInfoArray[cnt].iOutPosX, m_EnemyInfoArray[cnt].iOutPosY, m_EnemyInfoArray[cnt].fOutAngle);
				m_ObjectList->push_back(newObject);
			}



			++cnt;
		}

		fclose(fp);

		break;
	}// switch (m_iStage)
	

	// 플레이어 생성
	newObject = new ObjectAlucard();
	m_ObjectList->push_back(newObject);
	g_Player = (ObjectPlayer *)newObject;		// 전역변수로
}

void EventMainGame::Reset()
{
	Object * ObjectTemp;
	list<Object*>::iterator iter;
	list<Object*>::iterator iter_end;

	iter = m_ObjectList->begin();
	iter_end = m_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;
		iter = m_ObjectList->erase(iter);
		if (ObjectTemp->m_ComprehensiveType == df_COMPREHENSIVE_TYPE_ENEMY_BULLET) {
			g_ObjectBulletPool.Free((ObjectBullet *)ObjectTemp);
		}
		else {
			delete ObjectTemp;
		}
	}

	m_KeyQueue->Release();

	// 스테이지 1
	m_iStage = 1;

	Init();
}
