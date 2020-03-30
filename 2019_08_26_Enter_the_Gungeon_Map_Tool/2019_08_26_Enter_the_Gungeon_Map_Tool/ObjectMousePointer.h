#pragma once
#include "Object.h"

class ObjectMousePointer : public Object
{
public:
	ObjectMousePointer(int iPosX, int iPosY);
	virtual ~ObjectMousePointer();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

public:
	int m_iSpriteWidth;
	int m_iSpriteHeight;

	int m_iIncreaseVlaue;	// 스프라이트 증가 값
	int m_iStartSprite;		// 스프라이트 시작
	int m_iEndSprite;		// 스프라이트 끝
	int m_iAnimeDelay;		// 딜레이
	int m_iAnimeCount;		// 현제 딜레이 수

	bool m_bOnUIMouse;		// 마우스가 UI위에 있나
	bool m_bRedOption;		// 레드 옵션
	bool m_bBlueOption;		// 블루
};

