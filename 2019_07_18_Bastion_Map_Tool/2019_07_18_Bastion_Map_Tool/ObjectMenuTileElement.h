#pragma once
#include "ObjectMenuElement.h"

class ObjectMenuTileElement : public ObjectMenuElement
{
public:
	ObjectMenuTileElement(int iSpriteIndex, int iPosX, int iPosY, int * iScrollTopPtr, int * iScrollBottomPtr);
	virtual ~ObjectMenuTileElement();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	virtual void Move(int iMoveValueX, int iMoveValueY);

	virtual void SetPosition(int iPosX, int iPosY);

public:
	int m_iPropertiesCenterPointX;						// 속성 중점 좌표 X
	int m_iPropertiesCenterPointY;						// 속성 중점 좌표 Y

	int * m_iScrollTopPtr;								// 스크롤 위
	int * m_iScrollBottomPtr;							// 스크롤 아래
};

