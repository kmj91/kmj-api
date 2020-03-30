#pragma once
#include "Object.h"
class ObjectEnemyRed :public Object
{
public:
	enum State {
		STATE_STAND,		// 대기
		STATE_L,			// 왼쪽 이동
		STATE_R,			// 오른쪽 이동
		STATE_LL,			// 왼쪽 이동
		STATE_RR,			// 오른쪽 이동
	};

public:
	ObjectEnemyRed(int iAppearance, int iOutTime, int iPosX, int iPosY, int iDestPosX, int iDestPosY, float fDestAngle,
		int iOutPosX, int iOutPosY, float fOutAngle);
	virtual ~ObjectEnemyRed();

	virtual bool Action();
	virtual void Draw();

private:
	bool Animation(int iAnimeDelay);				// 애니메이션 처리

	bool Move(float fDestPosX, float fDestPosY, float fAngle, float fSpeed, bool bOption = false);		// 해당 좌표로 이동

private:
	int m_iAppearance;		// 등장할 스크롤 좌표 Y
	int m_iOutTime;			// 화면상에 보여지는 시간
	float m_fDestPosX;		// 배치 좌표 X, 배치 후 패턴 이동 좌표
	float m_fDestPosY;		// 배치 좌표 Y, 배치 후 패턴 이동 좌표
	float m_fDestAngle;		// 배치 선회 각도
	float m_fOutPosX;		// 퇴장 좌표 X
	float m_fOutPosY;		// 퇴장 좌표 Y
	float m_fOutAngle;		// 퇴장 선회 각도

	int m_iSpriteIndex;
	int m_iAnimeStart;
	int m_iAnimeEnd;
	int m_iAnimeCount;

	int m_iOnTime;
	int m_iActionTime;
	int m_iOnShotCnt;

	State m_Action;

	bool m_bOnFlag;		// 배치가 완료되었나?
	bool m_bOutFlag;	// 퇴장할 시간인가?
};

