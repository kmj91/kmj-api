#pragma once
#include "Object.h"
class ObjectEffect : public Object
{
public:
	ObjectEffect(int iType, int iPosX, int iPosY);
	virtual ~ObjectEffect();

	virtual bool Action();
	virtual void Draw();
	
private:
	bool Animation(int iAnimeDelay);		// 애니메이션 처리

private:
	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;
};

