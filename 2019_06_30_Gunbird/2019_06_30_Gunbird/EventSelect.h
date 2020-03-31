#pragma once
#include "Event.h"

#include <list>

using namespace std;

class Object;
class SceneTitleAndSelect;
class ObjectAnimation;
class ObjectPicture;

class EventSelect : public Event
{
public:
	EventSelect(SceneTitleAndSelect * ScenePtr);
	virtual ~EventSelect();

	virtual void KeyProcess();
	virtual bool Update();
	virtual void Draw();

private:
	bool m_bNum_5;
	bool m_bNum_1;

	ObjectAnimation * m_Select;
	ObjectPicture * m_1P;

	SceneTitleAndSelect * m_ScenePtr;
	list<Object *> * m_ObjectList;		// 오브젝트 리스트
	CQueue *m_KeyQueue;
};

