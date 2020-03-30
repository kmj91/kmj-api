#pragma once
#include "Object.h"
class ObjectUI : public Object
{
public:
	ObjectUI();
	virtual ~ObjectUI();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY) = 0;	// 메뉴와 오브젝트 요소 전부 이동
};

