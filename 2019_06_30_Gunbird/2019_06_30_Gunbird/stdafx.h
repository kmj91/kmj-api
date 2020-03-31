// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 특정 포함 파일이 들어 있는
// 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// 여기서 프로그램에 필요한 추가 헤더를 참조합니다.
#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "SceneManager.h"
#include "Object.h"
#include "ObjectPlayer.h"
#include "CObjectPoolTLS.h"
#include "ObjectBullet.h"
#include "CQueue.h"

#include <list>


using namespace std;

// 디파인
#define df_WIN_CLASS_NAME		L"슈팅 게임"
#define df_WIN_LEFT				0
#define df_WIN_TOP				0
#define df_WIN_RIGHT			700
#define df_WIN_BOTTOM			1000

#define df_MOVE_BOTTOM			900			// 플레이어 하단 이동 범위

#define df_GAME_LEFT			-50
#define df_GAME_TOP				-50
#define df_GAME_RIGHT			750
#define df_GAME_BOTTOM			1050

#define df_IN_GAME_LEFT			50			// 적 화면 안에 들어오는 판정
#define df_IN_GAME_TOP			50
#define df_IN_GAME_RIGHT		650
#define df_IN_GAME_BOTTOM		950

#define PI						3.141592654
#define FPS						80
#define FRAME_DELEY				1000/ FPS

// 타입
#define df_TYPE_EVENT_TITLE				0
#define df_TYPE_EVENT_SELECT			1
#define df_TYPE_EVENT_MAIN_GAME			2
#define df_TYPE_EVENT_GAME_OVER			3

#define df_TYPE_OBJECT_BACKGROUND					100
#define df_TYPE_OBJECT_PICTURE						101
#define df_TYPE_OBJECT_PLAYER						200
#define df_TYPE_OBJECT_ENEMY_RED					300
#define df_TYPE_OBJECT_ENEMY_BLUE					301
#define df_TYPE_OBJECT_ITEM_POWER_UP				400
#define df_TYPE_OBJECT_ALUCARD_BULLET_1				500
#define df_TYPE_OBJECT_ALUCARD_BULLET_2				501
#define df_TYPE_OBJECT_ALUCARD_BULLET_3				502
#define df_TYPE_OBJECT_ALUCARD_GUIDED_BULLET		503
#define df_TYPE_OBJECT_ENEMY_BULLET_1				600
#define df_TYPE_OBJECT_EFFECT_KABOOM_1				900

#define df_COMPREHENSIVE_TYPE_NEUTRAL				0
#define df_COMPREHENSIVE_TYPE_PLAYER				1
#define df_COMPREHENSIVE_TYPE_PLAYER_BULLET			2
#define df_COMPREHENSIVE_TYPE_ENEMY					3
#define df_COMPREHENSIVE_TYPE_ENEMY_BULLET			4
#define df_COMPREHENSIVE_TYPE_ITEM					5

//-------------------------------------------------------------------------
// Key 값
//-------------------------------------------------------------------------
// 공용
#define df_NUM_5				20
#define df_NUM_1				21


// MainGame
#define df_ACTION_MOVE_LL		0
#define df_ACTION_MOVE_LU		1
#define df_ACTION_MOVE_UU		2
#define df_ACTION_MOVE_RU		3
#define df_ACTION_MOVE_RR		4
#define df_ACTION_MOVE_RD		5
#define df_ACTION_MOVE_DD		6
#define df_ACTION_MOVE_LD		7
#define df_ACTION_ATTACK1		11
#define df_ACTION_ATTACK1_END	12
#define df_ACTION_ATTACK2		13
#define df_ACTION_ATTACK3		14
#define df_ACTION_EMPT				253
#define df_ACTION_DIE				254
#define df_ACTION_STAND				255


//-------------------------------------------------------------------------
// SceneTitleAndSelect 스프라이트
//-------------------------------------------------------------------------
#define df_SPRITE_TITLE_1							0
#define df_SPRITE_START								1
#define df_SPRITE_SELECT_SKY						2
#define df_SPRITE_SELECT_BAR						3
#define df_SPRITE_SELECT_1P							4

#define df_SPRITE_SELECT_ALUCARD_1					10
#define df_SPRITE_SELECT_ALUCARD_2					11
#define df_SPRITE_SELECT_ALUCARD_3					12
#define df_SPRITE_SELECT_ALUCARD_4					13
#define df_SPRITE_SELECT_ALUCARD_5					14
#define df_SPRITE_SELECT_MARION_1					20
#define df_SPRITE_SELECT_MARION_2					21
#define df_SPRITE_SELECT_MARION_3					22
#define df_SPRITE_SELECT_MARION_4					23

#define df_SPRITE_SELECT_ALUCARD_STAND_1			50
#define df_SPRITE_SELECT_ALUCARD_STAND_2			51
#define df_SPRITE_SELECT_ALUCARD_STAND_3			52
#define df_SPRITE_SELECT_ALUCARD_STAND_4			53
#define df_SPRITE_SELECT_ALUCARD_STAND_5			54
#define df_SPRITE_SELECT_ALUCARD_STAND_6			55
#define df_SPRITE_SELECT_MARION_STAND				60


//-------------------------------------------------------------------------



//-------------------------------------------------------------------------
// SceneMainGame 스프라이트
//-------------------------------------------------------------------------
#define df_SPRITE_ALUCARD_LIFE				0
#define df_SPRITE_ALUCARD_STAND_1			1
#define df_SPRITE_ALUCARD_STAND_2			2
#define df_SPRITE_ALUCARD_STAND_3			3
#define df_SPRITE_ALUCARD_STAND_4			4
#define df_SPRITE_ALUCARD_STAND_5			5
#define df_SPRITE_ALUCARD_STAND_6			6
#define df_SPRITE_ALUCARD_LEFT_1_1			7
#define df_SPRITE_ALUCARD_LEFT_1_2			8
#define df_SPRITE_ALUCARD_LEFT_1_3			9
#define df_SPRITE_ALUCARD_LEFT_1_4			10
#define df_SPRITE_ALUCARD_LEFT_1_5			11
#define df_SPRITE_ALUCARD_LEFT_1_6			12
#define df_SPRITE_ALUCARD_LEFT_2_1			13
#define df_SPRITE_ALUCARD_LEFT_2_2			14
#define df_SPRITE_ALUCARD_LEFT_2_3			15
#define df_SPRITE_ALUCARD_LEFT_2_4			16
#define df_SPRITE_ALUCARD_LEFT_2_5			17
#define df_SPRITE_ALUCARD_LEFT_2_6			18
#define df_SPRITE_ALUCARD_RIGHT_1_1			19
#define df_SPRITE_ALUCARD_RIGHT_1_2			20
#define df_SPRITE_ALUCARD_RIGHT_1_3			21
#define df_SPRITE_ALUCARD_RIGHT_1_4			22
#define df_SPRITE_ALUCARD_RIGHT_1_5			23
#define df_SPRITE_ALUCARD_RIGHT_1_6			24
#define df_SPRITE_ALUCARD_RIGHT_2_1			25
#define df_SPRITE_ALUCARD_RIGHT_2_2			26
#define df_SPRITE_ALUCARD_RIGHT_2_3			27
#define df_SPRITE_ALUCARD_RIGHT_2_4			28
#define df_SPRITE_ALUCARD_RIGHT_2_5			29
#define df_SPRITE_ALUCARD_RIGHT_2_6			30
#define df_SPRITE_ALUCARD_CHARGE_1			31
#define df_SPRITE_ALUCARD_CHARGE_2			32
#define df_SPRITE_ALUCARD_CHARGE_3			33
#define df_SPRITE_ALUCARD_CHARGE_4			34
#define df_SPRITE_ALUCARD_CHARGE_5			35
#define df_SPRITE_ALUCARD_CHARGE_6			36
#define df_SPRITE_ALUCARD_CHARGE_7			37
#define df_SPRITE_ALUCARD_CHARGE_8			38
#define df_SPRITE_ALUCARD_BOOM_01			39
#define df_SPRITE_ALUCARD_BOOM_02			40
#define df_SPRITE_ALUCARD_BOOM_03			41
#define df_SPRITE_ALUCARD_BOOM_04			42
#define df_SPRITE_ALUCARD_BOOM_05			43
#define df_SPRITE_ALUCARD_BOOM_06			44
#define df_SPRITE_ALUCARD_BOOM_07			45
#define df_SPRITE_ALUCARD_BOOM_08			46
#define df_SPRITE_ALUCARD_BOOM_09			47
#define df_SPRITE_ALUCARD_BOOM_10			48
#define df_SPRITE_ALUCARD_BOOM_11			49
#define df_SPRITE_ALUCARD_BOOM_12			50
#define df_SPRITE_ALUCARD_BOOM_13			51
#define df_SPRITE_ALUCARD_BOOM_14			52
#define df_SPRITE_ALUCARD_BOOM_15			53
#define df_SPRITE_ALUCARD_BOOM_16			54
#define df_SPRITE_ALUCARD_BOOM_17			55
#define df_SPRITE_ALUCARD_BOOM_18			56
#define df_SPRITE_ALUCARD_BOOM_19			57
#define df_SPRITE_ALUCARD_BOOM_EFFECT_1		58
#define df_SPRITE_ALUCARD_BOOM_EFFECT_2		59

#define df_SPRITE_ALUCARD_DIE						70
#define df_SPRITE_ALUCARD_BULLET_1_1				71
#define df_SPRITE_ALUCARD_BULLET_1_2				72
#define df_SPRITE_ALUCARD_BULLET_1_3				73
#define df_SPRITE_ALUCARD_BULLET_2_1				74
#define df_SPRITE_ALUCARD_BULLET_2_2				75
#define df_SPRITE_ALUCARD_BULLET_3_1				76
#define df_SPRITE_ALUCARD_BULLET_3_2				77
#define df_SPRITE_ALUCARD_BULLET_3_3				78
#define df_SPRITE_ALUCARD_BULLET_3_4				79
#define df_SPRITE_ALUCARD_CHARGE_BULLET_0			80
#define df_SPRITE_ALUCARD_CHARGE_BULLET_15			81
#define df_SPRITE_ALUCARD_CHARGE_BULLET_45			82
#define df_SPRITE_ALUCARD_CHARGE_BULLET_60			83
#define df_SPRITE_ALUCARD_CHARGE_BULLET_75			84
#define df_SPRITE_ALUCARD_CHARGE_BULLET_90			85
#define df_SPRITE_ALUCARD_CHARGE_BULLET_105			86
#define df_SPRITE_ALUCARD_CHARGE_BULLET_120			87
#define df_SPRITE_ALUCARD_CHARGE_BULLET_135			88
#define df_SPRITE_ALUCARD_CHARGE_BULLET_165			89
#define df_SPRITE_ALUCARD_CHARGE_BULLET_180			90
#define df_SPRITE_ALUCARD_CHARGE_BULLET_195			91
#define df_SPRITE_ALUCARD_CHARGE_BULLET_225			92
#define df_SPRITE_ALUCARD_CHARGE_BULLET_255			93
#define df_SPRITE_ALUCARD_CHARGE_BULLET_270			94
#define df_SPRITE_ALUCARD_CHARGE_BULLET_285			95
#define df_SPRITE_ALUCARD_CHARGE_BULLET_300			96
#define df_SPRITE_ALUCARD_CHARGE_BULLET_315			97
#define df_SPRITE_ALUCARD_CHARGE_BULLET_345			98


#define df_SPRITE_ENEMY_BLUE_CENTER_1				200
#define df_SPRITE_ENEMY_BLUE_CENTER_2				201
#define df_SPRITE_ENEMY_BLUE_LEFT_1_1				202
#define df_SPRITE_ENEMY_BLUE_LEFT_1_2				203
#define df_SPRITE_ENEMY_BLUE_LEFT_2_1				204
#define df_SPRITE_ENEMY_BLUE_LEFT_2_2				205
#define df_SPRITE_ENEMY_BLUE_LEFT_3_1				206
#define df_SPRITE_ENEMY_BLUE_LEFT_3_2				207
#define df_SPRITE_ENEMY_BLUE_RIGHT_1_1				208
#define df_SPRITE_ENEMY_BLUE_RIGHT_1_2				209
#define df_SPRITE_ENEMY_BLUE_RIGHT_2_1				210
#define df_SPRITE_ENEMY_BLUE_RIGHT_2_2				211
#define df_SPRITE_ENEMY_BLUE_RIGHT_3_1				212
#define df_SPRITE_ENEMY_BLUE_RIGHT_3_2				213

#define df_SPRITE_ENEMY_RED_CENTER_1				214
#define df_SPRITE_ENEMY_RED_CENTER_2				215
#define df_SPRITE_ENEMY_RED_LEFT_1_1				216
#define df_SPRITE_ENEMY_RED_LEFT_1_2				217
#define df_SPRITE_ENEMY_RED_LEFT_2_1				218
#define df_SPRITE_ENEMY_RED_LEFT_2_2				219
#define df_SPRITE_ENEMY_RED_RIGHT_1_1				220
#define df_SPRITE_ENEMY_RED_RIGHT_1_2				221
#define df_SPRITE_ENEMY_RED_RIGHT_2_1				222
#define df_SPRITE_ENEMY_RED_RIGHT_2_2				223

#define df_SPRITE_ENEMY_BULLET_1_1					250
#define df_SPRITE_ENEMY_BULLET_1_2					251
#define df_SPRITE_ENEMY_BULLET_1_3					252
#define df_SPRITE_ENEMY_BULLET_RELEASE_1_1			253
#define df_SPRITE_ENEMY_BULLET_RELEASE_1_2			254
#define df_SPRITE_ENEMY_BULLET_RELEASE_1_3			255
#define df_SPRITE_ENEMY_BULLET_RELEASE_1_4			256
#define df_SPRITE_ENEMY_BULLET_RELEASE_1_5			257


#define df_SPRITE_ITEM_POWER_UP_1					260
#define df_SPRITE_ITEM_POWER_UP_2					261
#define df_SPRITE_ITEM_POWER_UP_3					262
#define df_SPRITE_ITEM_POWER_UP_4					263
#define df_SPRITE_ITEM_POWER_UP_5					264
#define df_SPRITE_ITEM_POWER_UP_6					265
#define df_SPRITE_ITEM_POWER_UP_7					266
#define df_SPRITE_ITEM_POWER_UP_STRING_1			267
#define df_SPRITE_ITEM_POWER_UP_STRING_2			268

#define df_SPRITE_ITEM_KABOOM_1_1					269
#define df_SPRITE_ITEM_KABOOM_1_2					270
#define df_SPRITE_ITEM_KABOOM_1_3					271
#define df_SPRITE_ITEM_KABOOM_1_4					272
#define df_SPRITE_ITEM_KABOOM_1_5					273
#define df_SPRITE_ITEM_KABOOM_1_6					274
#define df_SPRITE_ITEM_KABOOM_1_7					275
#define df_SPRITE_ITEM_KABOOM_1_8					276
#define df_SPRITE_ITEM_KABOOM_1_9					277

#define df_SPRITE_STAGE_1							290

//-------------------------------------------------------------------------







// 스프라이트 애니메이션 딜레이
#define df_DELAY_SELECT						6
#define df_DELAY_ACTION						3
#define df_DELAY_BULLET						2
#define df_DELAY_EFFECT						3
#define df_DELAY_BOOM						10


// 맵 스크롤 속도
#define df_SPEED_SCROLL						0.8

// 아이템 이동 속도
#define df_SPEED_ITEM						3.0

// 플레이어 이속
#define df_SPEED_ALUCARD_MOVE				6.0

// 적 속도
#define df_SPEED_ENEMY_RED_MOVE				6.0
#define df_SPEED_ENEMY_RED_STAND			1.5
#define df_SPEED_ENEMY_BLUE_MOVE			6.0
#define df_SPEED_ENEMY_BLUE_STAND			1.5

// 총알 속도
#define df_SPEED_ALUCARD_BULLET_1			23.0
#define df_SPEED_ALUCARD_BULLET_2			25.0
#define df_SPEED_ALUCARD_BULLET_3			8.0

// 적 총알 속도
#define df_SPEED_ENEMY_BULLET_1				7.0

// 플레이어 충전 공격
#define df_ALUCARD_ENERGY_SHOT_GAUGE		80
#define df_ALUCARD_CHARGE_DELAY				30

// 패턴 딜레이
#define df_ENEMY_ACTION_DELAY				1000


struct Bullet {
	bool flag;
	int count;
};

enum ChangeScene {
	SCENE_EMPT,
	SCENE_TITLE,
	SCENE_GAME
};

enum EventMsg {
	TITLE_SELECT,
	GAME_START,
	GAME_OVER,
	GAME_RESET
};

enum GameState {
	EMPT,
	TITLE,
	SELECT,
	GAME
};



extern HWND g_hWnd;
extern CScreenDib g_cScreenDib;			// 전역으로 스크린DIB 생성
extern CSpriteDib g_cSprite;			// 최대 스프라이트 개수와 칼라키 값 입력.
extern SceneManager g_sceneManager;		// 씬을 관리할 씬 매니저

extern int g_time;
extern int g_fps;
extern int g_oldTime;
extern int g_skipFrame;

extern int g_FramePerSecond;
extern int g_Frame;

extern BYTE *g_bypDest;
extern int g_iDestWidth;
extern int g_iDestHeight;
extern int g_iDestPitch;

extern int g_iScore;
extern int g_iMouseX;
extern int g_iMouseY;
extern bool g_bActiveApp;
extern bool g_bNoBullet;

extern list<Object *> * g_ObjectList;		// 오브젝트 리스트
extern Object * g_StageBackground;			// 현제 배경 오브젝트

extern ObjectPlayer *g_Player;
extern CObjectPoolTLS<ObjectBullet> g_ObjectBulletPool;		// 오브젝트 풀


extern GameState g_GameState;			// 현제 게임 진행 상태
extern int g_iCoin;						// 코인
extern int g_iContinueCnt;				// 컨티뉴 카운트
extern bool g_bGameOver;				// 게임 오버