#pragma once
#include "Scene.h"
#include <list>
#include <queue>

using namespace std;

class Event;


class SceneTitleAndSelect : public Scene
{
public:
	SceneTitleAndSelect();
	virtual ~SceneTitleAndSelect();

	virtual void Update();

private:
	void LoadSprite();

private:
	list<Event *> *m_EventList;						// 이벤트 리스트

public:
	queue<EventMsg> *m_RequestEventQueue;
};

