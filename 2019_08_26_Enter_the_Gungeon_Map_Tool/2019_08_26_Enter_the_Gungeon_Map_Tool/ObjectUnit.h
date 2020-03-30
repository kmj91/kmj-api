#pragma once
#include "Object.h"
class ObjectUnit : public Object
{
public:
	ObjectUnit();
	virtual ~ObjectUnit();

	virtual bool Action() = 0;
	virtual void Draw() = 0;
	virtual bool Click() = 0;

public:
	int m_iUnitType;

	int m_iStartSprite;
	int m_iEndSprite;
	int m_iAnimeDelay;
	int m_iAnimeCount;

	bool m_bEraseFlag;		// true면 지우기
	bool m_bSelectFlag;		// true면 현제 유닛이 선택되었다
};

