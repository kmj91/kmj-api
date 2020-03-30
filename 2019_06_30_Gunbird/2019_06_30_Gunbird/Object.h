#pragma once

class Object
{
public:
	Object();
	virtual ~Object();

	virtual bool Action() = 0;
	virtual void Draw() = 0;

public:
	BYTE m_ComprehensiveType;
	WORD m_Type;	//오브젝트 타입
	float m_X;		//좌표 X
	float m_Y;		//좌표 Y

	float m_fLeft;
	float m_fTop;
	float m_fRight;
	float m_fBottom;

	float m_HP;

	bool m_bInFlag;		// 화면 안에 들어왔나
};

