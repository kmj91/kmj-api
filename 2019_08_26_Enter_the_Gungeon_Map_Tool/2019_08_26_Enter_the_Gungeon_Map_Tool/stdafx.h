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
#include <ft2build.h>						// freetype
#include FT_FREETYPE_H						// freetype
#pragma comment(lib,"freetype.lib")			// freetype

#include <vector>

using namespace std;

#include "CSpriteDib.h"
#include "CList.h"
#include "Object.h"
#include "ObjectUI.h"
#include "UIScrollBar.h"
#include "ObjectMousePointer.h"
#include "ObjectLightDib.h"
#include "ObjectLight.h"
#include "ObjectUnit.h"
#include "UnitLocation.h"



// 디파인
#define df_WIN_PARENT_CLASS_NAME				L"Enter the Cungeon Map Tool Class"
#define df_WIN_PARENT_TITLE_NAME				L"Enter the Cungeon Map Tool"
#define df_DEFAULT_TILE_X						15
#define df_DEFAULT_TILE_Y						15
#define df_DEFAULT_TILE_SIZE					64
#define df_RENDER_PRIORITY_LAST					1000000
#define df_SPEED_CAMERA							10					// 카메라 속도
#define FPS										60
#define FRAME_DELEY								1000/FPS

// 방향
#define df_DIRECTION_UU							1
#define df_DIRECTION_UR							2
#define df_DIRECTION_RR							4
#define df_DIRECTION_DR							8
#define df_DIRECTION_DD							16
#define df_DIRECTION_DL							32
#define df_DIRECTION_LL							64
#define df_DIRECTION_UL							128
#define df_DIRECTION_EMPT						0					// 모든 방향이 해당안됨
#define df_DIRECTION_ALL						255					// 모든 방향


// 정렬할떄 필요함
// 가장 화면 맨 위에 올라온 오브젝트 들의 정렬임
// 타입 번호
#define df_TYPE_STAGE_MAP						1
#define df_TYPE_OBJECT							2
#define df_TYPE_LIGHT							98
#define df_TYPE_LOCATION						99
#define df_TYPE_UI								100

// 마우스 클릭 오브젝트
#define df_MOUSE_CLICK_EMPT						0
#define df_MOUSE_CLICK_MENU_TITLE_BAR			1
#define df_MOUSE_CLICK_MENU_SCROLL_BAR			2
#define df_MOUSE_CLICK_UNIT_LOCATION			3
#define df_MOUSE_CLICK_UNIT_LOCATION_LEFT		4
#define df_MOUSE_CLICK_UNIT_LOCATION_TOP		5
#define df_MOUSE_CLICK_UNIT_LOCATION_RIGHT		6
#define df_MOUSE_CLICK_UNIT_LOCATION_BOTTOM		7

// 마우스 상태
#define df_MOUSE_STATE_EMPT						0
#define df_MOUSE_STATE_TILE_MAKE				1
#define df_MOUSE_STATE_OBJECT_MAKE				2
#define df_MOUSE_STATE_LOCATION_MAKE			3
#define df_MOUSE_STATE_LOCATION_DRAG			4					// 생성 후 크기 변경
#define df_MOUSE_STATE_SELECT_UNIT				5					// 유닛 선택 중
#define df_MOUSE_STATE_SELECT_LOCATION			6					// 로케이션 선택 중


// 메뉴
#define df_FOCUS_MENU_EMPT						0
#define df_FOCUS_MENU_TILE						1
#define df_FOCUS_MENU_OBJECT					2
#define df_FOCUS_MENU_ENEMY						3
#define df_FOCUS_MENU_LOCATION					4


// 트리거 메뉴
#define df_FOCUS_TRIGGER_MENU_EMPT				0
#define df_FOCUS_TRIGGER_MENU_TRIGGER_LIST		1
#define df_FOCUS_TRIGGER_MENU_CREATE_TRIGGER	2
#define df_FOCUS_TRIGGER_MENU_EDIT_TRIGGER		3
#define df_FOCUS_TRIGGER_MENU_DELETE_TRIGGER	4

// 타일 종류
#define df_TILE_TYPE_EMPT						0
#define df_TILE_TYPE_LIBRARY					1			// 서고
#define df_TILE_TYPE_CASTLE						2			// 성채

// 타일 상태
#define df_TILE_STATE_EMPT						0
#define df_TILE_STATE_FLOOR						1			// 바닥
#define df_TILE_STATE_BORDER					2			// 테두리
#define df_TILE_STATE_WALL_TOP					3			// 상단 벽
#define df_TILE_STATE_WALL_BOTTOM				4			// 하단 벽

// 유닛 종류
#define df_UNIT_TYPE_EMPT						0
#define df_UNIT_TYPE_LANTERN					1			// 랜턴
#define df_UNIT_TYPE_LANTERN_L					2			// 랜턴 Left
#define df_UNIT_TYPE_LANTERN_R					3			// 랜턴 Rright
#define df_UNIT_TYPE_LOCATION_RECTANGLE			4			// 직사각형 로케이션

#define df_UNIT_TYPE_ENEMY_BULLET_KIN			100
#define df_UNIT_TYPE_ENEMY_BANDANA_KIN			101
#define df_UNIT_TYPE_ENEMY_RED_SHOTGUN_KIN		102
#define df_UNIT_TYPE_ENEMY_BLUE_SHOTGUN_KIN		103
#define df_UNIT_TYPE_ENEMY_MUMMY_KIN			104
#define df_UNIT_TYPE_ENEMY_SHOTGRUB_KIN			105
#define df_UNIT_TYPE_ENEMY_GHOST_KIN			106
#define df_UNIT_TYPE_ENEMY_FLY_KIN				107
#define df_UNIT_TYPE_ENEMY_BIRD					108
#define df_UNIT_TYPE_ENEMY_CUBE					109
#define df_UNIT_TYPE_ENEMY_GREEN_BOOK			110
#define df_UNIT_TYPE_ENEMY_BLUE_BOOK			111
#define df_UNIT_TYPE_ENEMY_YELLOW_MAGIC			112
#define df_UNIT_TYPE_ENEMY_BLUE_MAGIC			113
#define df_UNIT_TYPE_ENEMY_RED_MAGIC			114
#define df_UNIT_TYPE_ENEMY_WISP					115
#define df_UNIT_TYPE_ENEMY_MUSHROOM				116

// 유닛 상태
#define df_UNIT_STATE_EMPT						0
#define df_UNIT_STATE_FLOOR						1			// 바닥에 붙어있는
#define df_UNIT_STATE_WALL						2			// 벽에 붙어있는
#define df_UNIT_STATE_ALL						3			// 바닥, 벽 전부


// 트리거 타입
// 조건
#define df_TRIGGER_TYPE_CONDITIONS_ALWAYS		0			// 무조건

// 액션
#define df_TRIGGER_TYPE_ACTIONS_CLOSE_DOOR		0			// 문닫기


// 스프라이트 번호
// UI
#define df_SPRITE_UI_MENU_TAB_TILE				0
#define df_SPRITE_UI_MENU_TAB_OBJECT			1
#define df_SPRITE_UI_MENU_TAB_ENEMY				2
#define df_SPRITE_UI_MENU_TAB_LOCATION			3

#define df_SPRITE_UI_MENU_LIBRARY_BORDER		10
#define df_SPRITE_UI_MENU_LIBRARY_FLOOR			11
#define df_SPRITE_UI_MENU_CASTLE_BORDER			12
#define df_SPRITE_UI_MENU_CASTLE_FLOOR			13

#define df_SPRITE_UI_MENU_OBJECT_LANTERN		20

#define df_SPRITE_UI_MENU_ENEMY_BULLET_KIN				50
#define df_SPRITE_UI_MENU_ENEMY_BANDANA_KIN				51
#define df_SPRITE_UI_MENU_ENEMY_RED_SHOTGUN_KIN			52
#define df_SPRITE_UI_MENU_ENEMY_BLUE_SHOTGUN_KIN		53
#define df_SPRITE_UI_MENU_ENEMY_MUMMY_KIN				54
#define df_SPRITE_UI_MENU_ENEMY_SHOTGRUB_KIN			55
#define df_SPRITE_UI_MENU_ENEMY_GHOST_KIN				56
#define df_SPRITE_UI_MENU_ENEMY_FLY_KIN					57
#define df_SPRITE_UI_MENU_ENEMY_BIRD					58
#define df_SPRITE_UI_MENU_ENEMY_CUBE					59
#define df_SPRITE_UI_MENU_ENEMY_GREEN_BOOK				60
#define df_SPRITE_UI_MENU_ENEMY_BLUE_BOOK				61
#define df_SPRITE_UI_MENU_ENEMY_YELLOW_MAGIC			62
#define df_SPRITE_UI_MENU_ENEMY_BLUE_MAGIC				63
#define df_SPRITE_UI_MENU_ENEMY_RED_MAGIC				64
#define df_SPRITE_UI_MENU_ENEMY_WISP					65
#define df_SPRITE_UI_MENU_ENEMY_MUSHROOM				66

#define df_SPRITE_UI_MENU_LOCATION_RECTANGLE	89			// 사각형 로케이션 메뉴 버튼

#define df_SPRITE_UI_MENU_ELEMENT_SELECT		90
#define df_SPRITE_UI_TILE_POINTER				91


// 타일
#define df_SPRITE_EMPT_SPRITE					98			// 마우스 포인터 등등, 비어있는 스프라이트를 가리킨다
#define df_SPRITE_BASIC_TILE					99
#define df_SPRITE_TILE_WALL						100
#define df_SPRITE_TILE_LIBRARY_BORDER_01		101
#define df_SPRITE_TILE_LIBRARY_BORDER_02		102
#define df_SPRITE_TILE_LIBRARY_BORDER_03		103
#define df_SPRITE_TILE_LIBRARY_BORDER_04		104
#define df_SPRITE_TILE_LIBRARY_BORDER_05		105
#define df_SPRITE_TILE_LIBRARY_BORDER_06		106
#define df_SPRITE_TILE_LIBRARY_BORDER_07		107
#define df_SPRITE_TILE_LIBRARY_BORDER_08		108
#define df_SPRITE_TILE_LIBRARY_BORDER_09		109
#define df_SPRITE_TILE_LIBRARY_BORDER_10		110
#define df_SPRITE_TILE_LIBRARY_BORDER_11		111
#define df_SPRITE_TILE_LIBRARY_BORDER_12		112
#define df_SPRITE_TILE_LIBRARY_BORDER_13		113
#define df_SPRITE_TILE_LIBRARY_BORDER_14		114
#define df_SPRITE_TILE_LIBRARY_BORDER_15		115
#define df_SPRITE_TILE_LIBRARY_BORDER_16		116
#define df_SPRITE_TILE_LIBRARY_BORDER_17		117
#define df_SPRITE_TILE_LIBRARY_BORDER_18		118
#define df_SPRITE_TILE_LIBRARY_BORDER_19		119
#define df_SPRITE_TILE_LIBRARY_BORDER_20		120
#define df_SPRITE_TILE_LIBRARY_BORDER_21		121
#define df_SPRITE_TILE_LIBRARY_BORDER_22		122
#define df_SPRITE_TILE_LIBRARY_BORDER_23		123
#define df_SPRITE_TILE_LIBRARY_BORDER_24		124
#define df_SPRITE_TILE_LIBRARY_BORDER_25		125
#define df_SPRITE_TILE_LIBRARY_BORDER_26		126
#define df_SPRITE_TILE_LIBRARY_BORDER_27		127
#define df_SPRITE_TILE_LIBRARY_BORDER_28		128
#define df_SPRITE_TILE_LIBRARY_BORDER_29		129
#define df_SPRITE_TILE_LIBRARY_BORDER_30		130
#define df_SPRITE_TILE_LIBRARY_BORDER_31		131
#define df_SPRITE_TILE_LIBRARY_BORDER_32		132
#define df_SPRITE_TILE_LIBRARY_BORDER_33		133
#define df_SPRITE_TILE_LIBRARY_BORDER_34		134
#define df_SPRITE_TILE_LIBRARY_BORDER_35		135
#define df_SPRITE_TILE_LIBRARY_BORDER_36		136
#define df_SPRITE_TILE_LIBRARY_BORDER_37		137
#define df_SPRITE_TILE_LIBRARY_BORDER_38		138
#define df_SPRITE_TILE_LIBRARY_BORDER_39		139
#define df_SPRITE_TILE_LIBRARY_BORDER_40		140
#define df_SPRITE_TILE_LIBRARY_BORDER_41		141
#define df_SPRITE_TILE_LIBRARY_BORDER_42		142
#define df_SPRITE_TILE_LIBRARY_BORDER_43		143
#define df_SPRITE_TILE_LIBRARY_BORDER_44		144
#define df_SPRITE_TILE_LIBRARY_BORDER_45		145
#define df_SPRITE_TILE_LIBRARY_BORDER_46		146
#define df_SPRITE_TILE_LIBRARY_FLOOR			147
#define df_SPRITE_TILE_LIBRARY_WALL_TOP			148
#define df_SPRITE_TILE_LIBRARY_WALL_BOTTOM		149

#define df_SPRITE_TILE_CASTLE_BORDER_01			150
#define df_SPRITE_TILE_CASTLE_BORDER_02			151
#define df_SPRITE_TILE_CASTLE_BORDER_03			152
#define df_SPRITE_TILE_CASTLE_BORDER_04			153
#define df_SPRITE_TILE_CASTLE_BORDER_05			154
#define df_SPRITE_TILE_CASTLE_BORDER_06			155
#define df_SPRITE_TILE_CASTLE_BORDER_07			156
#define df_SPRITE_TILE_CASTLE_BORDER_08			157
#define df_SPRITE_TILE_CASTLE_BORDER_09			158
#define df_SPRITE_TILE_CASTLE_BORDER_10			159
#define df_SPRITE_TILE_CASTLE_BORDER_11			160
#define df_SPRITE_TILE_CASTLE_BORDER_12			161
#define df_SPRITE_TILE_CASTLE_BORDER_13			162
#define df_SPRITE_TILE_CASTLE_BORDER_14			163
#define df_SPRITE_TILE_CASTLE_BORDER_15			164
#define df_SPRITE_TILE_CASTLE_BORDER_16			165
#define df_SPRITE_TILE_CASTLE_BORDER_17			166
#define df_SPRITE_TILE_CASTLE_BORDER_18			167
#define df_SPRITE_TILE_CASTLE_BORDER_19			168
#define df_SPRITE_TILE_CASTLE_BORDER_20			169
#define df_SPRITE_TILE_CASTLE_BORDER_21			170
#define df_SPRITE_TILE_CASTLE_BORDER_22			171
#define df_SPRITE_TILE_CASTLE_BORDER_23			172
#define df_SPRITE_TILE_CASTLE_BORDER_24			173
#define df_SPRITE_TILE_CASTLE_BORDER_25			174
#define df_SPRITE_TILE_CASTLE_BORDER_26			175
#define df_SPRITE_TILE_CASTLE_BORDER_27			176
#define df_SPRITE_TILE_CASTLE_BORDER_28			177
#define df_SPRITE_TILE_CASTLE_BORDER_29			178
#define df_SPRITE_TILE_CASTLE_BORDER_30			179
#define df_SPRITE_TILE_CASTLE_BORDER_31			180
#define df_SPRITE_TILE_CASTLE_BORDER_32			181
#define df_SPRITE_TILE_CASTLE_BORDER_33			182
#define df_SPRITE_TILE_CASTLE_BORDER_34			183
#define df_SPRITE_TILE_CASTLE_BORDER_35			184
#define df_SPRITE_TILE_CASTLE_BORDER_36			185
#define df_SPRITE_TILE_CASTLE_BORDER_37			186
#define df_SPRITE_TILE_CASTLE_BORDER_38			187
#define df_SPRITE_TILE_CASTLE_BORDER_39			188
#define df_SPRITE_TILE_CASTLE_BORDER_40			189
#define df_SPRITE_TILE_CASTLE_BORDER_41			190
#define df_SPRITE_TILE_CASTLE_BORDER_42			191
#define df_SPRITE_TILE_CASTLE_BORDER_43			192
#define df_SPRITE_TILE_CASTLE_BORDER_44			193
#define df_SPRITE_TILE_CASTLE_BORDER_45			194
#define df_SPRITE_TILE_CASTLE_BORDER_46			195
#define df_SPRITE_TILE_CASTLE_FLOOR_01			196
#define df_SPRITE_TILE_CASTLE_FLOOR_02			197
#define df_SPRITE_TILE_CASTLE_WALL_TOP			198
#define df_SPRITE_TILE_CASTLE_WALL_BOTTOM		199

// 빛
#define df_SPRITE_LIGHT_LANTERN_CENTER			200

// 물체
#define df_SPRITE_OBJECT_LANTERN_CENTER_01		210
#define df_SPRITE_OBJECT_LANTERN_CENTER_02		211
#define df_SPRITE_OBJECT_LANTERN_CENTER_03		212
#define df_SPRITE_OBJECT_LANTERN_LEFT_01		214
#define df_SPRITE_OBJECT_LANTERN_LEFT_02		215
#define df_SPRITE_OBJECT_LANTERN_LEFT_03		216
#define df_SPRITE_OBJECT_LANTERN_RIGHT_01		218
#define df_SPRITE_OBJECT_LANTERN_RIGHT_02		219
#define df_SPRITE_OBJECT_LANTERN_RIGHT_03		220

// 적
#define df_SPRITE_ENEMY_BULLET_KIN				300
#define df_SPRITE_ENEMY_BANDANA_KIN				301
#define df_SPRITE_ENEMY_RED_SHOTGUN_KIN			302
#define df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN		303
#define df_SPRITE_ENEMY_MUMMY_KIN				304
#define df_SPRITE_ENEMY_SHOTGRUB_KIN			305
#define df_SPRITE_ENEMY_GHOST_KIN				306
#define df_SPRITE_ENEMY_FLY_KIN					307
#define df_SPRITE_ENEMY_BIRD					308
#define df_SPRITE_ENEMY_CUBE					309
#define df_SPRITE_ENEMY_GREEN_BOOK				310
#define df_SPRITE_ENEMY_BLUE_BOOK				311
#define df_SPRITE_ENEMY_YELLOW_MAGIC			312
#define df_SPRITE_ENEMY_BLUE_MAGIC				313
#define df_SPRITE_ENEMY_RED_MAGIC				314
#define df_SPRITE_ENEMY_WISP					315
#define df_SPRITE_ENEMY_MUSHROOM				316

// struct 구조체

// 타일 정보
struct st_TileData {
	int iTileSprite;		// 타일 스프라이트 번호
	int iTileType;			// 타일 종류
	int iTileState;			// 타일 상태
	int iUnitType;		// 오브젝트 종류
	ObjectUnit *UnitPtr;	// 이 타일 위에 설치된 유닛 포인터
};

// 스프라이트 정보 구조체
struct st_SpriteInfo
{
	int iStartSprite;
	int iEndSprite;
	int iDelay;
};


// 메뉴 UI 리스트에서 쓸 구조체
struct st_UIElementInfo
{
	int iListType;									// 리스트 식별 변수
	UIScrollBar * ScrollBarPtr;						// 스크롤 포인터
	vector<ObjectUI *> * ElementObjectVectorPtr;	// 벡터 포인터
};

// 타일 유닛 메뉴 Element 데이터
struct st_UITileUnitElementData
{
	int iMouseState;							// 마우스 상태
	st_SpriteInfo stMousePointerSprite;			// 마우스 포인터 스프라이트
	int iMouseSpriteIncreaseVlaue;				// 마우스 스프라이트 증가 값
	int iElementType;							// 종류
	int iElementState;							// 상태
	int iElementSpriteIncreaseVlaue;			// 요소 스프라이트 증가 값
};

struct st_Trigger
{
	int iTriggerType;							// 트리거의 매칭 번호
	vector<int> TriggerTokenVector;				// 트리거 토큰
};

// 트리거 메뉴 Element 데이터
struct st_UITriggerElementData
{
	vector<st_Trigger> TriggerConditionsVector;
	vector<st_Trigger> TriggerActionsVector;
};

// extern
extern CSpriteDib *g_cSprite;					// 스프라이트
extern CList<Object *> *g_ObjectList;			// WinMain 오브젝트 리스트
extern CList<ObjectLight *> *g_ObjectLightList;	// 조명 리스트 (ObjectLightDib 에서 사용)
extern ObjectLightDib *g_ObjectLightDib;		// 조명 처리 비트맵
extern BYTE *g_bypDest;							// 스크린 버퍼
extern int g_iDestWidth;						// 스크린 가로 길이
extern int g_iDestHeight;						// 스크린 세로 길이
extern int g_iDestPitch;						// 스크린 피치 값

extern int g_iMapMouseX;						// 맵 기준 마우스 좌표 X
extern int g_iMapMouseY;						// 맵 기준 마우스 좌표 Y
extern int g_iUIMouseX;							// 화면(윈도우 창) 기준 마우스 좌표 X
extern int g_iUIMouseY;							// 화면(윈도우 창) 기준 마우스 좌표 Y
extern RECT g_CameraRect;						// 카메라 좌표
extern RECT g_WorldRect;						// 화면 구역 크기
extern int g_iMapWidth;							// 맵 가로 길이
extern int g_iMapHeight;						// 맵 세로 길이

extern ObjectUI *g_SelectedObjectUI;						// 선택된 오브젝트 UI
extern ObjectUnit *g_SelectedObjectUnit;					// 선택된 유닛
extern int g_iMouseClickObjectType;							// 마우스 클릭했을 때 선택된 오브젝트 타입 (드래그에서 사용)
extern int g_iMouseState;									// 마우스 상태
extern int g_iSelectedType;									// 메뉴에서 선택한 타일이나 오브젝트 종류
extern int g_iSelectedState;								// 메뉴에서 선택한 타일 상태
extern int g_iSelectedSpriteIncreaseVlaue ;					// 메뉴에서 선택한 스프라이트 증가 값
extern ObjectMousePointer * g_MousePointer;					// 마우스 지시자 (마우스 상태에따라 변함)

extern int g_iLocationNumber;								// 로케이션 고유 번호

extern FT_Library g_library;								// 프리타입 라이브러리
extern HDC g_hdc;											// hdc;