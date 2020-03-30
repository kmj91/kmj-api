#pragma once
#include "Object.h"
class ObjectBackground : public Object
{
public:
	ObjectBackground(int iPosX, int iPosY);
	virtual ~ObjectBackground();

	virtual bool Action();
	virtual void Draw();

private:
	int m_iSpriteIndex;
};

