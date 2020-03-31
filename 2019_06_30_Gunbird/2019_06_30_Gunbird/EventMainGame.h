#pragma once
#include "Event.h"

#include <list>

using namespace std;

class Object;
class ObjectPlayer;
class SceneMainGame;

class EventMainGame : public Event
{
private:
	//적 타입
	typedef enum enemyType {
		ENEMY_RED = 1,
		ENEMY_BLUE,
	}EnemyType;

	//적 배치 정보 구조체
	typedef struct enemyManager
	{
		int iAppearance;		// 배치 타이밍, 스크롤 Y 좌표 기준의 값이다
		int iOutTime;			// 배치되고 퇴장하기까지의 시간
		int iPosX;				// 배치되기전 화면 바깥의 좌표 X
		int iPosY;				// 배치되기전 화면 바깥의 좌표 Y
		int iDestPosX;			// 바깥에서 화면 안으로 배치될 좌표 X
		int iDestPosY;			// 바깥에서 화면 안으로 배치될 좌표 Y
		int iOutPosX;			// 화면 밖으로 퇴장할 좌표 X
		int iOutPosY;			// 화면 밖으로 퇴장할 좌표 Y
		float fDestAngle;		// 배치 될 때 선회 각도
		float fOutAngle;		// 퇴장 할 때 선회 각도
		EnemyType enType;		//적 타입
	}EnemyManager;

public:
	EventMainGame(SceneMainGame * ScenePtr);
	virtual ~EventMainGame();

	virtual void KeyProcess();
	virtual bool Update();
	virtual void Draw();

	void Init();
	void Reset();

private:
	int m_iStage;
	bool m_bNum_5;
	bool m_bNum_1;

	SceneMainGame * m_ScenePtr;
	list<Object *> * m_ObjectList;		// 오브젝트 리스트
	CQueue *m_KeyQueue;
};

