#pragma once
#include "Object.h"

class ObjectPicture : public Object
{
public:
	ObjectPicture(int iPosX, int iPosY, int iSpriteIndex, bool bOption = false);
	virtual ~ObjectPicture();

	virtual bool Action();
	virtual void Draw();

private:
	int m_iSpriteIndex;
	bool m_bOption;

public:
	bool m_bDrawFlag;				// true 그림, false 안그림
};

