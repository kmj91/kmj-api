#pragma once
#include "Scene.h"
#include <list>
#include <queue>

using namespace std;

class Event;
class EventMainGame;

class SceneMainGame : public Scene
{
public:
	SceneMainGame();
	virtual ~SceneMainGame();

	virtual void Update();

private:
	void LoadSprite();

private:
	EventMainGame *m_EventMainGame;
	list<Event *> *m_EventList;						// 이벤트 리스트

public:
	queue<EventMsg> *m_RequestEventQueue;
};

