#pragma once
class Object
{
public:
	Object();
	virtual ~Object();

	virtual bool Action() = 0;
	virtual void Draw() = 0;
	virtual bool Click() = 0;

public:
	int m_iType;				// 오브젝트 타입

	double m_dX;
	double m_dY;

	int m_iSpriteIndex;

	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;

	int m_iRenderPriority;		// 랜더 우선순위
};

