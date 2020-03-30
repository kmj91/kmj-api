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
	int m_iX;					// 위치 좌표 X
	int m_iY;					// 위치 좌표 Y
	int m_iSpriteIndex;			// 스프라이트 인덱스
	int m_iLeft;				// 오브젝트 영역 RECT
	int m_iRight;
	int m_iTop;
	int m_iBottom;
	int m_iWidth;				// 가로 길이
	int m_iHeight;				// 세로 길이
	int m_iRenderPriority;		// 랜더 우선순위
	int m_iSectorPosX;			// 섹터 좌표 X
	int m_iSectorPosY;			// 섹터 좌표 Y
};

