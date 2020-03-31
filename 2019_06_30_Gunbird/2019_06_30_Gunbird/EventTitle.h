#pragma once
#include "Event.h"

#include <list>

using namespace std;

class Object;
class SceneTitleAndSelect;
class ObjectPicture;

class EventTitle :public Event
{
public:
	EventTitle(SceneTitleAndSelect * ScenePtr);
	virtual ~EventTitle();

	virtual void KeyProcess();
	virtual bool Update();
	virtual void Draw();

private:
	int m_iCount;		// 글자 깜빡임용

	bool m_bNum_5;
	bool m_bNum_1;

	bool m_bStartFlag;

	ObjectPicture * m_GameStartPicture;
	SceneTitleAndSelect * m_ScenePtr;
	list<Object *> * m_ObjectList;		// 오브젝트 리스트
	CQueue *m_KeyQueue;
};

