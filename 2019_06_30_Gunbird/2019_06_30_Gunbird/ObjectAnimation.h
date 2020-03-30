#pragma once
#include "Object.h"

class ObjectAnimation : public Object
{
public:
	ObjectAnimation(int iPosX, int iPosY, int iStartIndex, int iEndIndex, int iAnimeDelay);
	virtual ~ObjectAnimation();

	virtual bool Action();
	virtual void Draw();

private:
	bool Animation(int iAnimeDelay);		// 애니메이션 처리

public:
	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;

	int m_iAnimeDelay;
};

