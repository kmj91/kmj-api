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


#include "CList.h"
#include "CScreenDib.h"
#include "CSpriteDib.h"
#include "Object.h"
#include "ObjectMenuElementListScrollBar.h"
#include "ObjectMousePointer.h"
#include "ObjectUnit.h"




// 디파인
#define df_WIN_PARENT_CLASS_NAME				L"Bastion Map Tool Class"
#define df_WIN_PARENT_TITLE_NAME				L"Bastion Map Tool Title"

#define df_WIN_MAIN_SIZE_WIDTH					1000
#define df_WIN_MAIN_SIZE_HEIGHT					800
#define df_MAP_LEFT								0
#define df_MAP_TOP								0
#define df_MAP_RIGHT							2000
#define df_MAP_BOTTOM							2000
#define df_TILE_WIDTH							100
#define df_TILE_HEIGHT							50
#define df_TILE_SMALL_WIDTH						50
#define df_TILE_SMALL_HEIGHT					25
#define df_TILE_PROPERTIES_SIZE					25
#define df_TILE_PROPERTIES_X					8
#define df_TILE_PROPERTIES_Y					8
#define df_MENU_SCROLL_BAR_WIDTH				25

#define df_RENDER_PRIORITY_LAST					1000000

#define df_SPEED_CAMERA							5


// 정렬할떄 필요함
// 가장 화면 맨 위에 올라온 오브젝트 들의 정렬임
// 타입 번호
#define df_TYPE_STAGE_MAP						1
#define df_TYPE_STAGE							2
#define df_TYPE_TILE							3
#define df_TYPE_PLAYER							10
#define df_TYPE_OBJECT							11
#define df_TYPE_BROKEN							12
#define df_TYPE_ENEMY							13
#define df_TYPE_UI								100

// 스프라이트 번호
// 타일
#define df_SPRITE_TILE_0						0
#define df_SPRITE_TILE_1						1
#define df_SPRITE_TILE_2						2
#define df_SPRITE_TILE_3						3
#define df_SPRITE_TILE_4						4
#define df_SPRITE_TILE_5						5
#define df_SPRITE_TILE_6						6
#define df_SPRITE_TILE_7						7
#define df_SPRITE_TILE_8						8
#define df_SPRITE_TILE_9						9
#define df_SPRITE_TILE_10						10
#define df_SPRITE_TILE_11						11
#define df_SPRITE_TILE_12						12
#define df_SPRITE_TILE_13						13
#define df_SPRITE_TILE_14						14
#define df_SPRITE_TILE_15						15
#define df_SPRITE_TILE_16						16
#define df_SPRITE_TILE_17						17
#define df_SPRITE_TILE_18						18
#define df_SPRITE_TILE_19						19
#define df_SPRITE_TILE_20						20
#define df_SPRITE_TILE_21						21
#define df_SPRITE_TILE_22						22
#define df_SPRITE_TILE_23						23
#define df_SPRITE_TILE_24						24
#define df_SPRITE_TILE_25						25
#define df_SPRITE_TILE_26						26
#define df_SPRITE_TILE_27						27
#define df_SPRITE_TILE_28						28
#define df_SPRITE_TILE_29						29
#define df_SPRITE_TILE_30						30
#define df_SPRITE_TILE_31						31
#define df_SPRITE_TILE_32						32
#define df_SPRITE_TILE_33						33
#define df_SPRITE_TILE_34						34
#define df_SPRITE_TILE_35						35
#define df_SPRITE_TILE_36						36
#define df_SPRITE_TILE_37						37
#define df_SPRITE_TILE_38						38
#define df_SPRITE_TILE_39						39
#define df_SPRITE_TILE_40						40
#define df_SPRITE_TILE_41						41
#define df_SPRITE_TILE_42						42
#define df_SPRITE_TILE_43						43
#define df_SPRITE_TILE_44						44
#define df_SPRITE_TILE_45						45
#define df_SPRITE_TILE_46						46
#define df_SPRITE_TILE_47						47
#define df_SPRITE_TILE_48						48
#define df_SPRITE_TILE_49						49
#define df_SPRITE_TILE_50						50
#define df_SPRITE_TILE_51						51
#define df_SPRITE_TILE_52						52
#define df_SPRITE_TILE_53						53
#define df_SPRITE_TILE_54						54
#define df_SPRITE_TILE_55						55
#define df_SPRITE_TILE_56						56
#define df_SPRITE_TILE_57						57
#define df_SPRITE_TILE_58						58
#define df_SPRITE_TILE_59						59
#define df_SPRITE_TILE_60						60
#define df_SPRITE_TILE_61						61
#define df_SPRITE_TILE_62						62
#define df_SPRITE_TILE_63						63
#define df_SPRITE_TILE_64						64
#define df_SPRITE_TILE_65						65
#define df_SPRITE_TILE_66						66
#define df_SPRITE_TILE_67						67
#define df_SPRITE_TILE_68						68
#define df_SPRITE_TILE_69						69
#define df_SPRITE_TILE_70						70
#define df_SPRITE_TILE_71						71
#define df_SPRITE_TILE_72						72
#define df_SPRITE_TILE_73						73
#define df_SPRITE_TILE_74						74
#define df_SPRITE_TILE_75						75
#define df_SPRITE_TILE_76						76
#define df_SPRITE_TILE_77						77
#define df_SPRITE_TILE_78						78
#define df_SPRITE_TILE_79						79
// 스테이지
#define df_SPRITE_STAGE_1						80
#define df_SPRITE_STAGE_2						81
#define df_SPRITE_STAGE_3						82
#define df_SPRITE_STAGE_4						83
#define df_SPRITE_STAGE_5						84
#define df_SPRITE_STAGE_6						85


// 타일 아이콘
#define df_SPRITE_MENU_TILE_0					100
#define df_SPRITE_MENU_TILE_1					101
#define df_SPRITE_MENU_TILE_2					102
#define df_SPRITE_MENU_TILE_3					103
#define df_SPRITE_MENU_TILE_4					104
#define df_SPRITE_MENU_TILE_5					105
#define df_SPRITE_MENU_TILE_6					106
#define df_SPRITE_MENU_TILE_7					107
#define df_SPRITE_MENU_TILE_8					108
#define df_SPRITE_MENU_TILE_9					109
#define df_SPRITE_MENU_TILE_10					110
#define df_SPRITE_MENU_TILE_11					111
#define df_SPRITE_MENU_TILE_12					112
#define df_SPRITE_MENU_TILE_13					113
#define df_SPRITE_MENU_TILE_14					114
#define df_SPRITE_MENU_TILE_15					115
#define df_SPRITE_MENU_TILE_16					116
#define df_SPRITE_MENU_TILE_17					117
#define df_SPRITE_MENU_TILE_18					118
#define df_SPRITE_MENU_TILE_19					119
#define df_SPRITE_MENU_TILE_20					120
#define df_SPRITE_MENU_TILE_21					121
#define df_SPRITE_MENU_TILE_22					122
#define df_SPRITE_MENU_TILE_23					123
#define df_SPRITE_MENU_TILE_24					124
#define df_SPRITE_MENU_TILE_25					125
#define df_SPRITE_MENU_TILE_26					126
#define df_SPRITE_MENU_TILE_27					127
#define df_SPRITE_MENU_TILE_28					128
#define df_SPRITE_MENU_TILE_29					129
#define df_SPRITE_MENU_TILE_30					130
#define df_SPRITE_MENU_TILE_31					131
#define df_SPRITE_MENU_TILE_32					132
#define df_SPRITE_MENU_TILE_33					133
#define df_SPRITE_MENU_TILE_34					134
#define df_SPRITE_MENU_TILE_35					135
#define df_SPRITE_MENU_TILE_36					136
#define df_SPRITE_MENU_TILE_37					137
#define df_SPRITE_MENU_TILE_38					138
#define df_SPRITE_MENU_TILE_39					139
#define df_SPRITE_MENU_TILE_40					140
#define df_SPRITE_MENU_TILE_41					141
#define df_SPRITE_MENU_TILE_42					142
#define df_SPRITE_MENU_TILE_43					143
#define df_SPRITE_MENU_TILE_44					144
#define df_SPRITE_MENU_TILE_45					145
#define df_SPRITE_MENU_TILE_46					146
#define df_SPRITE_MENU_TILE_47					147
#define df_SPRITE_MENU_TILE_48					148
#define df_SPRITE_MENU_TILE_49					149

#define df_SPRITE_MENU_TILE_55					155
#define df_SPRITE_MENU_TILE_56					156
#define df_SPRITE_MENU_TILE_57					157
#define df_SPRITE_MENU_TILE_58					158
#define df_SPRITE_MENU_TILE_59					159
// 스테이지 아이콘
#define df_SPRITE_MENU_STAGE_1					180
#define df_SPRITE_MENU_STAGE_2					181
#define df_SPRITE_MENU_STAGE_3					182
#define df_SPRITE_MENU_STAGE_4					183
#define df_SPRITE_MENU_STAGE_5					184
#define df_SPRITE_MENU_STAGE_6					185


// 플레이어
#define df_SPRITE_KID_WAKE_UP					190
// 플레이어 아이콘
#define df_SPRITE_MENU_KID_WAKE_UP				191


// 물체
#define df_SPRITE_OBJECT_0						200
#define df_SPRITE_OBJECT_1						201
#define df_SPRITE_OBJECT_2						202
#define df_SPRITE_OBJECT_3						203
#define df_SPRITE_OBJECT_4						204
#define df_SPRITE_OBJECT_5						205
#define df_SPRITE_OBJECT_6						206
#define df_SPRITE_OBJECT_7						207
#define df_SPRITE_OBJECT_8						208
#define df_SPRITE_OBJECT_9						209
#define df_SPRITE_OBJECT_10						210
#define df_SPRITE_OBJECT_11						211
#define df_SPRITE_OBJECT_12						212
#define df_SPRITE_OBJECT_13						213
#define df_SPRITE_OBJECT_14						214
#define df_SPRITE_OBJECT_15						215
#define df_SPRITE_OBJECT_16						216
#define df_SPRITE_OBJECT_17						217
#define df_SPRITE_OBJECT_18						218
#define df_SPRITE_OBJECT_19						219
#define df_SPRITE_OBJECT_20						220
#define df_SPRITE_OBJECT_21						221
#define df_SPRITE_OBJECT_22						222
#define df_SPRITE_OBJECT_23						223
#define df_SPRITE_OBJECT_24						224
#define df_SPRITE_OBJECT_25						225
#define df_SPRITE_OBJECT_26						226
#define df_SPRITE_OBJECT_27						227
#define df_SPRITE_OBJECT_28						228
// 부서지는 물체
#define df_SPRITE_BROKEN_27						229
#define df_SPRITE_BROKEN_28						230
#define df_SPRITE_BROKEN_29						231
#define df_SPRITE_BROKEN_30						232
#define df_SPRITE_BROKEN_31						233
#define df_SPRITE_BROKEN_32						234
#define df_SPRITE_BROKEN_33						235
#define df_SPRITE_BROKEN_34						236
#define df_SPRITE_BROKEN_35						237
#define df_SPRITE_BROKEN_36						238
#define df_SPRITE_BROKEN_37						239
#define df_SPRITE_BROKEN_38						240
#define df_SPRITE_BROKEN_39						241
#define df_SPRITE_BROKEN_40						242
#define df_SPRITE_BROKEN_41						243

// 적
#define df_SPRITE_ENEMY_0						250
#define df_SPRITE_ENEMY_1						251
#define df_SPRITE_ENEMY_2						252
#define df_SPRITE_ENEMY_3						253
#define df_SPRITE_ENEMY_4						254
#define df_SPRITE_ENEMY_5						255
#define df_SPRITE_ENEMY_6						256
#define df_SPRITE_ENEMY_7						257
#define df_SPRITE_ENEMY_8						258

// 물체 아이콘
#define df_SPRITE_MENU_OBJECT_0					300
#define df_SPRITE_MENU_OBJECT_1					301
#define df_SPRITE_MENU_OBJECT_2					302
#define df_SPRITE_MENU_OBJECT_3					303
#define df_SPRITE_MENU_OBJECT_4					304
#define df_SPRITE_MENU_OBJECT_5					305
#define df_SPRITE_MENU_OBJECT_6					306
#define df_SPRITE_MENU_OBJECT_7					307
#define df_SPRITE_MENU_OBJECT_8					308
#define df_SPRITE_MENU_OBJECT_9					309
#define df_SPRITE_MENU_OBJECT_10				310
#define df_SPRITE_MENU_OBJECT_11				311
#define df_SPRITE_MENU_OBJECT_12				312
#define df_SPRITE_MENU_OBJECT_13				313
#define df_SPRITE_MENU_OBJECT_14				314
#define df_SPRITE_MENU_OBJECT_15				315
#define df_SPRITE_MENU_OBJECT_16				316
#define df_SPRITE_MENU_OBJECT_17				317
#define df_SPRITE_MENU_OBJECT_18				318
#define df_SPRITE_MENU_OBJECT_19				319
#define df_SPRITE_MENU_OBJECT_20				320
#define df_SPRITE_MENU_OBJECT_21				321
#define df_SPRITE_MENU_OBJECT_22				322
#define df_SPRITE_MENU_OBJECT_23				323
#define df_SPRITE_MENU_OBJECT_24				324
#define df_SPRITE_MENU_OBJECT_25				325
#define df_SPRITE_MENU_OBJECT_26				326
#define df_SPRITE_MENU_OBJECT_27				327
#define df_SPRITE_MENU_OBJECT_28				328
// 부서지는 물체 아이콘
#define df_SPRITE_MENU_BROKEN_27				329
#define df_SPRITE_MENU_BROKEN_28				330
#define df_SPRITE_MENU_BROKEN_29				331
#define df_SPRITE_MENU_BROKEN_30				332
#define df_SPRITE_MENU_BROKEN_31				333
#define df_SPRITE_MENU_BROKEN_32				334
#define df_SPRITE_MENU_BROKEN_33				335
#define df_SPRITE_MENU_BROKEN_34				336
#define df_SPRITE_MENU_BROKEN_35				337
#define df_SPRITE_MENU_BROKEN_36				338
#define df_SPRITE_MENU_BROKEN_37				339
#define df_SPRITE_MENU_BROKEN_38				340
#define df_SPRITE_MENU_BROKEN_39				341
#define df_SPRITE_MENU_BROKEN_40				342
#define df_SPRITE_MENU_BROKEN_41				343

// 적
#define df_SPRITE_MENU_ENEMY_0					350
#define df_SPRITE_MENU_ENEMY_1					351
#define df_SPRITE_MENU_ENEMY_2					352
#define df_SPRITE_MENU_ENEMY_3					353
#define df_SPRITE_MENU_ENEMY_4					354
#define df_SPRITE_MENU_ENEMY_5					355
#define df_SPRITE_MENU_ENEMY_6					356
#define df_SPRITE_MENU_ENEMY_7					357
#define df_SPRITE_MENU_ENEMY_8					358

// 배경
#define df_SPRITE_BACKGROUND_2					370
// UI
#define df_SPRITE_EMPT_SPRITE					380
#define df_SPRITE_MAKE_TILE_SIZE_1				381
#define df_SPRITE_MAKE_TILE_SIZE_4				382
#define df_SPRITE_MENU_TITLE_BAR				383
#define df_SPRITE_MENU_TILE						384
#define df_SPRITE_MENU_STAGE					385
#define df_SPRITE_MENU_PLAYER					386
#define df_SPRITE_MENU_OBJECT					387
#define df_SPRITE_MENU_BROKEN					388
#define df_SPRITE_MENU_ENEMY					389
// 지우개 아이콘
#define df_SPRITE_ERASE							399


// 마우스 클릭했을 때 선택된 오브젝트 (드래그에서 사용)
enum MouseClick {
	MOUSE_CLICK_EMPT = 0,
	MOUSE_CLICK_MENU_TITLE_BAR,
	MOUSE_CLICK_MENU_SCROLL_BAR
};

// 마우스 상태
enum MouseState {
	MOUSE_STATE_EMPT = 0,
	MOUSE_STATE_TILE_MAKE_4,
	MOUSE_STATE_TILE_MAKE_1,
	MOUSE_STATE_STAGE_MAKE,
	MOUSE_STATE_PLAYER_MAKE,
	MOUSE_STATE_OBJECT_MAKE,
	MOUSE_STATE_BROKEN_MAKE,
	MOUSE_STATE_ENEMY_MAKE,
	MOUSE_STATE_ERASE
};

// 메뉴 항목
enum FocusMenu {
	FOCUS_MENU_EMPT = 0,
	FOCUS_MENU_TILE,
	FOCUS_MENU_STAGE,
	FOCUS_MENU_PLAYER,
	FOCUS_MENU_OBJECT,
	FOCUS_MENU_BROKEN,
	FOCUS_MENU_ENEMY
};


// extern
extern CSpriteDib *g_cSprite;					// 스프라이트
extern CList<Object *> *g_ObjectList;			// WinMain 오브젝트 리스트
extern ObjectUnit *g_Player;					// 플레이어
extern BYTE *g_bypDest;							// 스크린 버퍼
extern int g_iDestWidth;						// 스크린 가로 길이
extern int g_iDestHeight;						// 스크린 세로 길이
extern int g_iDestPitch;						// 스크린 피치 값

extern int g_iMouseX;							// 마우스 좌표 X
extern int g_iMouseY;							// 마우스 좌표 Y
extern RECT g_CameraRect;						// 카메라 좌표

extern bool g_bOnUIMouse;								// 마우스가 UI에 올라갔을 경우
extern bool g_bUpdateProperties;						// 타일 속성좌표 업데이트 플래그
extern int g_iMakeTileIndex;							// 마우스 상태가 MAKE TILE 일 경우 만들 타일 인덱스 번호
extern MouseClick g_MouseClickObject;					// 마우스 클릭한 오브젝트
extern MouseState g_MouseState;							// 마우스 상태
extern ObjectMousePointer * g_MousePointer;				// 마우스 지시자 (마우스 상태에따라 변함)
extern ObjectMenuElementListScrollBar * g_MenuElementListScrollBar;		// 메뉴 요소 스크롤 바

extern int	g_iPropertiesCenterPointX;			// 속성 중점 좌표 X
extern int	g_iPropertiesCenterPointY;			// 속성 중점 좌표 Y
extern int g_iMakePropertiesSizeX;				// 속성 배열 X 길이
extern int g_iMakePropertiesSizeY;				// 속성 배열 Y 길이
extern BYTE g_byCheckMapProperties;				// 검사할 MAP 속성 값
extern BYTE g_byCheckMakeProperties;			// 검사할 MAKE 속성 값
extern BYTE *g_bypArrayMAkeTileProperties;		// 타일 속성
extern BYTE g_byArrayMAkeTileProperties_4[df_TILE_PROPERTIES_Y * df_TILE_PROPERTIES_X];
extern BYTE g_byArrayMAkeTileProperties_1[df_TILE_PROPERTIES_Y * df_TILE_PROPERTIES_X];
// 스테이지
extern BYTE g_byArrayMakePropertiesStage_1[40 * 62];
extern BYTE g_byArrayMakePropertiesStage_2[40 * 62];
extern BYTE g_byArrayMakePropertiesStage_3[40 * 62];
extern BYTE g_byArrayMakePropertiesStage_4[40 * 62];
extern BYTE g_byArrayMakePropertiesStage_5[98 * 144];
extern BYTE g_byArrayMakePropertiesStage_6[60 * 95];
// 플레이어
extern BYTE g_byArrayMAkePlayerWakeUpProperties[6 * 7];
// 물체들
extern BYTE g_byArrayMakePropertiesObject_0[7 * 8];
extern BYTE g_byArrayMakePropertiesObject_1[6 * 4];
extern BYTE g_byArrayMakePropertiesObject_2[6 * 4];
extern BYTE g_byArrayMakePropertiesObject_3[5 * 7];
extern BYTE g_byArrayMakePropertiesObject_4[5 * 9];
extern BYTE g_byArrayMakePropertiesObject_5[7 * 6];
extern BYTE g_byArrayMakePropertiesObject_6[17 * 9];
extern BYTE g_byArrayMakePropertiesObject_7[20 * 4];
extern BYTE g_byArrayMakePropertiesObject_8[6 * 5];
extern BYTE g_byArrayMakePropertiesObject_9[8 * 7];
extern BYTE g_byArrayMakePropertiesObject_10[3 * 4];
extern BYTE g_byArrayMakePropertiesObject_11[4 * 6];
extern BYTE g_byArrayMakePropertiesObject_12[4 * 7];
extern BYTE g_byArrayMakePropertiesObject_13[19 * 14];
extern BYTE g_byArrayMakePropertiesObject_14[3 * 3];
extern BYTE g_byArrayMakePropertiesObject_15[7 * 13];
extern BYTE g_byArrayMakePropertiesObject_16[4 * 4];
extern BYTE g_byArrayMakePropertiesObject_17[3 * 5];
extern BYTE g_byArrayMakePropertiesObject_18[17 * 4];
extern BYTE g_byArrayMakePropertiesObject_19[3 * 4];
extern BYTE g_byArrayMakePropertiesObject_20[4 * 5];
extern BYTE g_byArrayMakePropertiesObject_21[4 * 5];
extern BYTE g_byArrayMakePropertiesObject_22[18 * 7];
extern BYTE g_byArrayMakePropertiesObject_23[18 * 7];
extern BYTE g_byArrayMakePropertiesObject_24[10 * 6];
extern BYTE g_byArrayMakePropertiesObject_25[4 * 7];
extern BYTE g_byArrayMakePropertiesObject_26[20 * 20];
extern BYTE g_byArrayMakePropertiesObject_27[6 * 6];
extern BYTE g_byArrayMakePropertiesObject_28[10 * 12];
//브로큰
extern BYTE g_byArrayMakePropertiesBroken_27[4 * 2];
extern BYTE g_byArrayMakePropertiesBroken_28[7 * 5];
extern BYTE g_byArrayMakePropertiesBroken_29[6 * 7];
extern BYTE g_byArrayMakePropertiesBroken_30[6 * 8];
extern BYTE g_byArrayMakePropertiesBroken_31[6 * 7];
extern BYTE g_byArrayMakePropertiesBroken_32[6 * 7];
extern BYTE g_byArrayMakePropertiesBroken_33[5 * 6];
extern BYTE g_byArrayMakePropertiesBroken_34[5 * 7];
extern BYTE g_byArrayMakePropertiesBroken_35[3 * 3];
extern BYTE g_byArrayMakePropertiesBroken_36[4 * 5];
extern BYTE g_byArrayMakePropertiesBroken_37[4 * 5];
extern BYTE g_byArrayMakePropertiesBroken_38[5 * 6];
extern BYTE g_byArrayMakePropertiesBroken_39[4 * 3];
extern BYTE g_byArrayMakePropertiesBroken_40[6 * 7];
extern BYTE g_byArrayMakePropertiesBroken_41[2 * 3];
// 적
extern BYTE g_byArrayMakePropertiesEnemy_0[7 * 10];
//검은새
extern BYTE g_byArrayMakePropertiesEnemy_1[6 * 7];
//하얀새
extern BYTE g_byArrayMakePropertiesEnemy_2[5 * 7];
//터렛
extern BYTE g_byArrayMakePropertiesEnemy_3[5 * 4];
//우라보스
extern BYTE g_byArrayMakePropertiesEnemy_4[10 * 13];
//우라석궁
extern BYTE g_byArrayMakePropertiesEnemy_5[10 * 13];
//우라잡몹
extern BYTE g_byArrayMakePropertiesEnemy_6[10 * 13];
//우라총
extern BYTE g_byArrayMakePropertiesEnemy_7[20 * 26];
//우라칼
extern BYTE g_byArrayMakePropertiesEnemy_8[20 * 26];

extern Object *g_TestObject;
