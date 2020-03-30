#include "stdafx.h"
#include "ObjectStageMap.h"
#include "ObjectUnit.h"
#include "UnitLantern.h"
#include "UnitLocation.h"
#include "UnitEnemy.h"


ObjectStageMap::ObjectStageMap(int iMapWidth, int iMapHeight)
{
	// 타입
	m_iType = df_TYPE_STAGE_MAP;
	// 위치
	m_iX = 0;
	m_iY = 0;
	// 그리기 우선순위
	m_iRenderPriority = 0;

	CreateMap(iMapWidth, iMapHeight);
}


ObjectStageMap::~ObjectStageMap()
{
	ReleaseMap();
}

bool ObjectStageMap::Action()
{


	return false;
}

void ObjectStageMap::Draw()
{
	int iCnt = 0;
	int iByteX = 0;
	int iByteY = 0;
	int iX;				// 좌표
	int iY;
	int iWidth;			// 길이
	int iHeight;


	// 기본 배경색
	//g_cSprite->DrawBackground(0x282828, m_iX, m_iY, g_iDestWidth, g_iDestHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);

	//--------------------------------------
	// 렉걸려서 필요한 부분만 칠해야겠음
	//--------------------------------------
	// 왼쪽 바깥
	if (g_CameraRect.left < 0) {
		iX = 0;
		iY = 0;
		iWidth = -g_CameraRect.left;	
		iHeight = g_iDestHeight;

		g_cSprite->DrawBackground(0x282828, iX, iY, iWidth, iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}
	// 위
	if (g_CameraRect.top < 0) {
		iY = 0;
		iHeight = -g_CameraRect.top;
		if (g_CameraRect.left < 0) {
			iX = -g_CameraRect.left;
			iWidth = g_iDestWidth + g_CameraRect.left;
		}
		else if (g_CameraRect.right > g_iMapWidth) {
			iX = 0;
			iWidth = g_iDestWidth - (g_CameraRect.right - g_iMapWidth);
		}
		else {
			iX = 0;
			iWidth = g_iDestWidth;
		}

		g_cSprite->DrawBackground(0x282828, iX, iY, iWidth, iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}
	// 오른쪽
	if (g_CameraRect.right > g_iMapWidth) {
		iX = g_iDestWidth - (g_CameraRect.right - g_iMapWidth);
		iWidth = g_iDestWidth - iX;
		iY = 0;
		iHeight = g_iDestHeight;
		g_cSprite->DrawBackground(0x282828, iX, iY, iWidth, iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}
	// 아래
	if (g_CameraRect.bottom > g_iMapHeight) {
		iY = g_iDestHeight - (g_CameraRect.bottom - g_iMapHeight);
		iHeight = g_iDestHeight - iY;
		if (g_CameraRect.left < 0) {
			iX = -g_CameraRect.left;
			iWidth = g_iDestWidth + g_CameraRect.left;
		}
		else if (g_CameraRect.right > g_iMapWidth) {
			iX = 0;
			iWidth = g_iDestWidth - (g_CameraRect.right - g_iMapWidth);
		}
		else {
			iX = 0;
			iWidth = g_iDestWidth;
		}

		g_cSprite->DrawBackground(0x282828, iX, iY, iWidth, iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
	}



	while (iCnt < m_iBufferSize) {
		if (iByteX >= m_iTileWidth) {
			iByteX = 0;
			++iByteY;
		}

		iX = iByteX * df_DEFAULT_TILE_SIZE;
		iY = iByteY * df_DEFAULT_TILE_SIZE;
		
		g_cSprite->DrawIncreaseSprite(m_stpTileMap[iCnt].iTileSprite, iX, iY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, 4, true);

		++iCnt;
		++iByteX;
	}

	
}

bool ObjectStageMap::Click()
{
	int iMouseX;
	int iMouseY;
	int iTileX;
	int iTileY;

	//--------------------------------------
	// 전체 맵의 범위
	//--------------------------------------
	iMouseX = g_iMapMouseX;
	iMouseY = g_iMapMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		iTileX = iMouseX / df_DEFAULT_TILE_SIZE;
		iTileY = iMouseY / df_DEFAULT_TILE_SIZE;
		

		switch (g_iMouseState) {
		case df_MOUSE_STATE_TILE_MAKE :
			//------------------------------------------------
			// 마우스 상태가 타일 타입이면
			// 타일 까는 로직 들어감
			// 해당 위치의 타일과 주변 타일도 변경되어야 함
			//------------------------------------------------
			ModifyTileService(iTileX, iTileY);
			break;
		case df_MOUSE_STATE_OBJECT_MAKE:
			//------------------------------------------------
			// 물체는 unit 객체로 생성됨
			// 물체는 종류에따라 지정 위치에만 생성될 수 있음
			//------------------------------------------------
			CreateUnit(iTileX, iTileY);
			break;
		case df_MOUSE_STATE_LOCATION_MAKE:
			//------------------------------------------------
			// 로케이션은 unit으로 생성됨
			// 단, 직사각형으로 타일 크기에 맞춰서 생성됨
			// 그 외에 제한사항 없음
			//------------------------------------------------
			CreateLocation(iTileX, iTileY);
			break;
		case df_MOUSE_STATE_SELECT_UNIT:
			//------------------------------------------------
			// 유닛 선택 상태면
			// 선택 해제됨
			//------------------------------------------------
			// 선택된 유닛 해제
			g_SelectedObjectUnit->m_bSelectFlag = false;
			g_SelectedObjectUnit = NULL;
			// 마우스 상태 변경
			g_iMouseState = df_MOUSE_STATE_EMPT;
			break;
		default :

			break;
		}


		return true;
	}

	return false;

}

void ObjectStageMap::ResizeMap(int iMapWidth, int iMapHeight)
{
	ReleaseMap();
	CreateMap(iMapWidth, iMapHeight);
}

// 오브젝트 만들 위치 검사
bool ObjectStageMap::CheckObjectMake(int iTileX, int iTileY)
{
	int iSelectedState = g_iSelectedState;						// 선택된 상태
	int iSelectedType = g_iSelectedType;						// 선택된 타입
	int iCnt;
	st_TileData *stpTileMap;
	CList<Object *>::iterator iter;
	CList<Object *>::iterator iter_end;
	Object* ObjectTemp;
	int iWidth;
	int iHeight;
	int iCenterPointX;
	int iCenterPointY;
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;

	switch (iSelectedState)
	{
	case df_UNIT_STATE_FLOOR:
		//--------------------------------------
		// 바닥에 붙어있는 오브젝트
		//--------------------------------------

		iWidth = g_cSprite->m_stpSprite[iSelectedType].iWidth * 4;
		iHeight = g_cSprite->m_stpSprite[iSelectedType].iHeight * 4;
		iCenterPointX = g_cSprite->m_stpSprite[iSelectedType].iCenterPointX * 4;
		iCenterPointY = g_cSprite->m_stpSprite[iSelectedType].iCenterPointY * 4;

		// 충돌 범위
		iLeft = g_iMapMouseX - iCenterPointX;
		iTop = g_iMapMouseY - iCenterPointY;
		iRight = iLeft + iWidth;
		iBottom = iTop + iHeight;

		// 검사
		iter = g_ObjectList->begin();
		iter_end = g_ObjectList->end();
		while (iter != iter_end) {
			ObjectTemp = *iter;

			if (ObjectTemp->m_iType != df_TYPE_OBJECT) {
				++iter;
				continue;
			}

			if (ObjectTemp->m_iLeft < iRight && ObjectTemp->m_iRight > iLeft &&
				ObjectTemp->m_iTop < iBottom && ObjectTemp->m_iBottom > iTop) {

				return false;
			}
			++iter;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iLeft / df_DEFAULT_TILE_SIZE;
		iTileY = iTop / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return false;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iRight / df_DEFAULT_TILE_SIZE;
		iTileY = iTop / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return false;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iLeft / df_DEFAULT_TILE_SIZE;
		iTileY = iBottom / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return false;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iRight / df_DEFAULT_TILE_SIZE;
		iTileY = iBottom / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return false;
		}

		return true;;

	case df_UNIT_STATE_WALL:
		//--------------------------------------
		// 벽에 붙어있는 오브젝트
		//--------------------------------------
		switch (iSelectedType) {
		case df_UNIT_TYPE_LANTERN:
			//--------------------------------------
			// 랜턴
			// 상단벽에 붙음
			// 왼쪽 오른쪽 벽에도 붙음
			//--------------------------------------
			
			// 현제 타일 정보
			iCnt = iTileX + (iTileY * m_iTileWidth);
			stpTileMap = &m_stpTileMap[iCnt];

			// 상단 벽인가?
			if (stpTileMap->iTileState == df_TILE_STATE_WALL_TOP) {
				g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_CENTER_01;
				g_MousePointer->m_iStartSprite = df_SPRITE_OBJECT_LANTERN_CENTER_01;
				g_MousePointer->m_iEndSprite = df_SPRITE_OBJECT_LANTERN_CENTER_03;
				g_MousePointer->m_iAnimeDelay = 5;
				// 이미 무언가 설치되었나?
				if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
					return false;
				}
				else {
					return true;
				}
			}
			// 바닥인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
				//--------------------------------------
				// 왼쪽과 오른쪽을 검사 해야 함
				//--------------------------------------

				// 왼쪽에 가까움
				if (g_iMapMouseX % df_DEFAULT_TILE_SIZE < df_DEFAULT_TILE_SIZE / 2) {
					// 왼쪽 검사
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						g_MousePointer->m_iStartSprite = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						g_MousePointer->m_iEndSprite = df_SPRITE_OBJECT_LANTERN_LEFT_03;
						g_MousePointer->m_iAnimeDelay = 5;
						// 이미 무언가 설치되었나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							return false;
						}
						else {
							return true;
						}
					}

					// 오른쪽 검사
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						g_MousePointer->m_iStartSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						g_MousePointer->m_iEndSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_03;
						g_MousePointer->m_iAnimeDelay = 5;
						// 이미 무언가 설치되었나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							return false;
						}
						else {
							return true;
						}
					}
				}
				// 오른쪽에 가까움
				else {
					// 오른쪽 검사
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						g_MousePointer->m_iStartSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						g_MousePointer->m_iEndSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_03;
						g_MousePointer->m_iAnimeDelay = 5;
						// 이미 무언가 설치되었나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							return false;
						}
						else {
							return true;
						}
					}

					// 왼쪽 검사
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						g_MousePointer->m_iStartSprite = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						g_MousePointer->m_iEndSprite = df_SPRITE_OBJECT_LANTERN_LEFT_03;
						g_MousePointer->m_iAnimeDelay = 5;
						// 이미 무언가 설치되었나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							return false;
						}
						else {
							return true;
						}
					}
				}
			}

			// 디폴트 모양
			g_MousePointer->m_iSpriteIndex = df_SPRITE_OBJECT_LANTERN_CENTER_01;

			break;
		default:
			break;
		}

		break;

	default:
		break;
	}

	return false;
}

// 유닛 삭제
// 외부에서 함수호출로 사용합니다
void ObjectStageMap::DeleteUnit(ObjectUnit * DeleteUnitPtr)
{
	int iTileX;
	int iTileY;

	switch (DeleteUnitPtr->m_iUnitType) {
	case df_UNIT_TYPE_ENEMY_BULLET_KIN:
	case df_UNIT_TYPE_ENEMY_BANDANA_KIN:
	case df_UNIT_TYPE_ENEMY_RED_SHOTGUN_KIN:
	case df_UNIT_TYPE_ENEMY_BLUE_SHOTGUN_KIN:
	case df_UNIT_TYPE_ENEMY_MUMMY_KIN:
	case df_UNIT_TYPE_ENEMY_SHOTGRUB_KIN:
	case df_UNIT_TYPE_ENEMY_GHOST_KIN:
	case df_UNIT_TYPE_ENEMY_FLY_KIN:
	case df_UNIT_TYPE_ENEMY_BIRD:
	case df_UNIT_TYPE_ENEMY_CUBE:
	case df_UNIT_TYPE_ENEMY_GREEN_BOOK:
	case df_UNIT_TYPE_ENEMY_BLUE_BOOK:
	case df_UNIT_TYPE_ENEMY_YELLOW_MAGIC:
	case df_UNIT_TYPE_ENEMY_BLUE_MAGIC:
	case df_UNIT_TYPE_ENEMY_RED_MAGIC:
	case df_UNIT_TYPE_ENEMY_WISP:
	case df_UNIT_TYPE_ENEMY_MUSHROOM:
		DeleteUnitPtr->m_bEraseFlag = true;
		return;
	default:
		break;;
	}


	iTileX = DeleteUnitPtr->m_iX / df_DEFAULT_TILE_SIZE;
	iTileY = DeleteUnitPtr->m_iY / df_DEFAULT_TILE_SIZE;

	DeleteUnitService(iTileX, iTileY);
}

int ObjectStageMap::GetBufferSize()
{
	return m_iBufferSize * sizeof(st_TileData);
}

// 유닛 만들기
// 위치 좌표, 선택된 상태, 선택된 타입, 선택된 스프라이트 증가 값
void ObjectStageMap::CreateUnit(int iPosX, int iPosY, int iUnitType, int iIncreaseVlaue)
{
	int iCnt;
	ObjectUnit *newUnit;
	st_SpriteInfo stSpriteInfo;
	int iTileX = iPosX / df_DEFAULT_TILE_SIZE;
	int iTileY = iPosY / df_DEFAULT_TILE_SIZE;


	switch (iUnitType) {
	case df_UNIT_TYPE_LANTERN:
		stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_CENTER_01;
		stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_CENTER_03;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitLantern(stSpriteInfo, iUnitType, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);

		// 현제 타일에 오브젝트 설치 값 저장
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iUnitType = iUnitType;
		m_stpTileMap[iCnt].UnitPtr = newUnit;
		break;
	case df_UNIT_TYPE_LANTERN_L:
		stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_LEFT_01;
		stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_LEFT_03;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitLantern(stSpriteInfo, iUnitType, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);

		// 현제 타일에 오브젝트 설치 값 저장
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iUnitType = iUnitType;
		m_stpTileMap[iCnt].UnitPtr = newUnit;
		break;
	case df_UNIT_TYPE_LANTERN_R:
		stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
		stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_03;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitLantern(stSpriteInfo, iUnitType, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);

		// 현제 타일에 오브젝트 설치 값 저장
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iUnitType = iUnitType;
		m_stpTileMap[iCnt].UnitPtr = newUnit;
		break;
	case df_UNIT_TYPE_ENEMY_BULLET_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BULLET_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BULLET_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_BANDANA_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BANDANA_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BANDANA_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_RED_SHOTGUN_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_BLUE_SHOTGUN_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_MUMMY_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_MUMMY_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_MUMMY_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_SHOTGRUB_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_GHOST_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_GHOST_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_GHOST_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_FLY_KIN:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_FLY_KIN;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_FLY_KIN;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_BIRD:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BIRD;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BIRD;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_CUBE:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_CUBE;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_CUBE;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_GREEN_BOOK:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_GREEN_BOOK;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_GREEN_BOOK;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_BLUE_BOOK:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_BOOK;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_BOOK;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_YELLOW_MAGIC:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_BLUE_MAGIC:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_RED_MAGIC:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_RED_MAGIC;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_RED_MAGIC;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_WISP:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_WISP;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_WISP;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	case df_UNIT_TYPE_ENEMY_MUSHROOM:
		stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_MUSHROOM;
		stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_MUSHROOM;
		stSpriteInfo.iDelay = 5;
		newUnit = new UnitEnemy(stSpriteInfo, iUnitType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);
		break;
	default:
		break;
	}
}


void ObjectStageMap::CreateMap(int iMapWidth, int iMapHeight)
{
	int iCnt;

	// 오브젝트 범위
	m_iLeft = 0;
	m_iTop = 0;
	m_iRight = iMapWidth;
	m_iBottom = iMapHeight;

	// 가로
	m_iTileWidth = iMapWidth / df_DEFAULT_TILE_SIZE;
	// 세로
	m_iTileHeight = iMapHeight / df_DEFAULT_TILE_SIZE;

	// 생성자로 맵 길이를 줄때 크기를 딱 맞춰서 줌
	// 이 로직은 필요 없음
	//if (iMapWidth % df_DEFAULT_TILE_SIZE) {
	//	++m_iTileWidth;
	//}
	//if (iMapHeight % df_DEFAULT_TILE_SIZE) {
	//	++m_iTileHeight;
	//}

	// 크기
	m_iBufferSize = m_iTileWidth * m_iTileHeight;

	// 타일맵 구조체 버퍼
	m_stpTileMap = (st_TileData*)malloc(m_iBufferSize * sizeof(st_TileData));

	// 초기화
	iCnt = 0;
	while (iCnt < m_iBufferSize) {
		m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
		m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
		m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_EMPT;
		m_stpTileMap[iCnt].UnitPtr = NULL;
		++iCnt;
	}
}

void ObjectStageMap::ReleaseMap()
{
	free(m_stpTileMap);
}


//--------------------------------------
// ModifyTileService
// 타일 교체 일괄 처리 서비스
//--------------------------------------
void ObjectStageMap::ModifyTileService(int iTileX, int iTileY)
{
	int iSelectedTileState = g_iSelectedState;		// 선택된 타일 상태
	int iSelectedTileType = g_iSelectedType;		// 선택된 타일 타입
	int iCnt;

	switch (iSelectedTileState)
	{
	case df_TILE_STATE_FLOOR:

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			//--------------------------------------
			// 서고 바닥
			//--------------------------------------
			iCnt = iTileX + (iTileY * m_iTileWidth);
			// 바닥이 아닌가?
			if (m_stpTileMap[iCnt].iTileState != df_TILE_STATE_FLOOR) {
				//----------------------------------------------------------
				// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
				// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
				//----------------------------------------------------------
				DeleteUnitService(iTileX, iTileY);
			}
			// 천장인가?
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
				//-------------------------------------------
				// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
				//-------------------------------------------
				// 왼쪽
				if (iTileX - 1 >= 0) {
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 현제 타일 왼쪽이 바닥인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX - 1, iTileY);
					}
				}
				// 오른쪽
				if (iTileX + 1 < m_iTileWidth) {
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 현제 타일 오른쪽이 바닥인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX + 1, iTileY);
					}
					
				}
			}

			iCnt = iTileX + (iTileY * m_iTileWidth);
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
			m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;

			// 아래 1칸
			ModifyAroundTile(iTileX, iTileY + 1);
			// 위 1칸
			ModifyAroundTile(iTileX, iTileY - 1);

			// 아래 2칸
			ModifyAroundTile(iTileX, iTileY + 2);
			// 위 2칸
			ModifyAroundTile(iTileX, iTileY - 2);
			

			// 위 3칸
			ModifyAroundTile(iTileX, iTileY - 3);

			// 위 3칸 옆
			ModifyAroundTile(iTileX - 1, iTileY - 3);
			ModifyAroundTile(iTileX + 1, iTileY - 3);

			// 위 2칸 옆
			ModifyAroundTile(iTileX - 1, iTileY - 2);
			ModifyAroundTile(iTileX + 1, iTileY - 2);

			// 아래 2칸 옆
			ModifyAroundTile(iTileX - 1, iTileY + 2);
			ModifyAroundTile(iTileX + 1, iTileY + 2);

			// 위 옆
			ModifyAroundTile(iTileX - 1, iTileY - 1);
			ModifyAroundTile(iTileX + 1, iTileY - 1);

			// 옆
			ModifyAroundTile(iTileX - 1, iTileY);
			ModifyAroundTile(iTileX + 1, iTileY);

			// 아래 옆
			ModifyAroundTile(iTileX - 1, iTileY + 1);
			ModifyAroundTile(iTileX + 1, iTileY + 1);


			break;
		case df_TILE_TYPE_CASTLE:
			//--------------------------------------
			// 성채 바닥
			//--------------------------------------
			iCnt = iTileX + (iTileY * m_iTileWidth);
			// 바닥이 아닌가?
			if (m_stpTileMap[iCnt].iTileState != df_TILE_STATE_FLOOR) {
				//----------------------------------------------------------
				// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
				// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
				//----------------------------------------------------------
				DeleteUnitService(iTileX, iTileY);
			}
			// 천장인가?
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
				//-------------------------------------------
				// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
				//-------------------------------------------
				// 왼쪽
				if (iTileX - 1 >= 0) {
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 현제 타일 왼쪽이 바닥인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX - 1, iTileY);
					}
				}
				// 오른쪽
				if (iTileX + 1 < m_iTileWidth) {
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 현제 타일 오른쪽이 바닥인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX + 1, iTileY);
					}

				}
			}

			//--------------------------------------
			// 바닥 타일이 두종류임
			//--------------------------------------
			iCnt = iTileX + (iTileY * m_iTileWidth);
			m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			if (rand() % 2) {
				m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
			}
			else {
				m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
			}
			

			// 아래 1칸
			ModifyAroundTile(iTileX, iTileY + 1);
			// 위 1칸
			ModifyAroundTile(iTileX, iTileY - 1);

			// 아래 2칸
			ModifyAroundTile(iTileX, iTileY + 2);
			// 위 2칸
			ModifyAroundTile(iTileX, iTileY - 2);


			// 위 3칸
			ModifyAroundTile(iTileX, iTileY - 3);

			// 위 3칸 옆
			ModifyAroundTile(iTileX - 1, iTileY - 3);
			ModifyAroundTile(iTileX + 1, iTileY - 3);

			// 위 2칸 옆
			ModifyAroundTile(iTileX - 1, iTileY - 2);
			ModifyAroundTile(iTileX + 1, iTileY - 2);

			// 아래 2칸 옆
			ModifyAroundTile(iTileX - 1, iTileY + 2);
			ModifyAroundTile(iTileX + 1, iTileY + 2);

			// 위 옆
			ModifyAroundTile(iTileX - 1, iTileY - 1);
			ModifyAroundTile(iTileX + 1, iTileY - 1);

			// 옆
			ModifyAroundTile(iTileX - 1, iTileY);
			ModifyAroundTile(iTileX + 1, iTileY);

			// 아래 옆
			ModifyAroundTile(iTileX - 1, iTileY + 1);
			ModifyAroundTile(iTileX + 1, iTileY + 1);

			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_TILE_STATE_BORDER:

		ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType, true);


		//----------------------------------------------------------
		// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
		// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
		//----------------------------------------------------------
		DeleteUnitService(iTileX, iTileY);

		// 위아래 1칸
		ModifyAroundTile(iTileX, iTileY - 1);
		ModifyAroundTile(iTileX, iTileY + 1);

		// 위아래 2칸
		ModifyAroundTile(iTileX, iTileY - 2);
		ModifyAroundTile(iTileX, iTileY + 2);

		// 위 2칸 옆
		ModifyAroundTile(iTileX - 1, iTileY - 2);
		ModifyAroundTile(iTileX + 1, iTileY - 2);

		// 아래 2칸 옆
		ModifyAroundTile(iTileX - 1, iTileY + 2);
		ModifyAroundTile(iTileX + 1, iTileY + 2);

		// 위 옆
		ModifyAroundTile(iTileX - 1, iTileY - 1);
		ModifyAroundTile(iTileX + 1, iTileY - 1);

		// 옆
		ModifyAroundTile(iTileX - 1, iTileY);
		ModifyAroundTile(iTileX + 1, iTileY);

		// 아래 옆
		ModifyAroundTile(iTileX - 1, iTileY + 1);
		ModifyAroundTile(iTileX + 1, iTileY + 1);


		break;

	default:
		break;
	}

}


//--------------------------------------
// ChangeTileProcess
// 천장 테두리 타일 교체 부분 처리 (서비스에 종속)
// 4방향 타일을 검사하고 교체함
//--------------------------------------
void ObjectStageMap::ModifyBorderTileProcess(int iTileX, int iTileY, int iSelectedTileType, bool bCertainlyBorderFlag)
{
	int iCnt;
	BYTE byTileDirection = 0;		// 타일 방향

	// ★ : 위치
	// ▣ : 맵의 끝
	// ■ : 천장
	// ▥ : 벽

	//--------------------------------------
	// 위쪽 검사
	//--------------------------------------
	// 위쪽이 맵의 끝인가?
	if (iTileY - 1 < 0) {
		// ▣
		// ★
		byTileDirection = byTileDirection | df_DIRECTION_UU;
		byTileDirection = byTileDirection | df_DIRECTION_UL;
		byTileDirection = byTileDirection | df_DIRECTION_UR;
	}
	// 위쪽이 맵의 끝이 아닌 상태에서
	else {
		// 위쪽이 천장인가?
		iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
			// ■
			// ★
			byTileDirection = byTileDirection | df_DIRECTION_UU;

			//--------------------------------------
			// ↖ 검사
			//--------------------------------------
			// 위 왼쪽이 맵의 끝인가?
			if (iTileX - 1 < 0) {
				// ▣■
				// 　★
				byTileDirection = byTileDirection | df_DIRECTION_UL;
			}
			// 왼쪽이 맵의 끝이 아닌 상태에서
			else {
				// 위 왼쪽이 천장인가?
				iCnt = (iTileX - 1) + ((iTileY - 1) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ■■
					// 　★
					byTileDirection = byTileDirection | df_DIRECTION_UL;
				}
			}

			//--------------------------------------
			// ↗ 검사
			//--------------------------------------
			// 위 오른쪽이 맵의 끝인가?
			if (iTileX + 1 >= m_iTileWidth) {
				// ■▣
				// ★
				byTileDirection = byTileDirection | df_DIRECTION_UR;
			}
			// 오른쪽이 맵의 끝이 아닌 상태에서
			else {
				// 위 오른쪽이 천장인가?
				iCnt = (iTileX + 1) + ((iTileY - 1) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ■■
					// ★
					byTileDirection = byTileDirection | df_DIRECTION_UR;
				}
			}
		}
		// 위쪽이 상단 벽인가?
		else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_TOP) {
			// ▥
			// ★

			// 2칸 위가 맵의 끝인가?
			if (iTileY - 2 < 0) {
				// ▣
				// ▥
				// ★
				// 이런 경우는 없다
				//byTileDirection = byTileDirection | df_DIRECTION_UU;
				throw;
			}
			 // 2칸 위쪽이 맵의 끝이 아닌 상태에서
			else {
				// 2칸 위쪽이 천장인가?	
				iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ■
					// ▥
					// ★
					// 상단 벽은 천장으로 교체될 것임
					byTileDirection = byTileDirection | df_DIRECTION_UU;

					//--------------------------------------
					// ↖ 검사
					//--------------------------------------
					// 위 왼쪽이 맵의 끝인가?
					if (iTileX - 1 < 0) {
						// ▣■
						// 　★
						byTileDirection = byTileDirection | df_DIRECTION_UL;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 위 왼쪽이 천장인가?
						iCnt = (iTileX - 1) + ((iTileY - 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ■■
							// 　★
							byTileDirection = byTileDirection | df_DIRECTION_UL;
						}
					}

					//--------------------------------------
					// ↗ 검사
					//--------------------------------------
					// 위 오른쪽이 맵의 끝인가?
					if (iTileX + 1 >= m_iTileWidth) {
						// ■▣
						// ★
						byTileDirection = byTileDirection | df_DIRECTION_UR;
					}
					// 오른쪽이 맵의 끝이 아닌 상태에서
					else {
						// 위 오른쪽이 천장인가?
						iCnt = (iTileX + 1) + ((iTileY - 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ■■
							// ★
							byTileDirection = byTileDirection | df_DIRECTION_UR;
						}
					}
				}
			}
		}
	}


	//--------------------------------------
	// 아래쪽 검사 및 대각선 검사
	// 2칸 아래가 바닥일 경우 벽으로 바뀔 수 있음
	//--------------------------------------
	// 아래쪽이 맵의 끝인가?
	if (iTileY + 1 >= m_iTileHeight) {
		// ★
		// ▣
		byTileDirection = byTileDirection | df_DIRECTION_DD;
		byTileDirection = byTileDirection | df_DIRECTION_DL;
		byTileDirection = byTileDirection | df_DIRECTION_DR;
	}
	// 아래쪽이 맵의 끝이 아닌 상태에서
	else {
		// 아래쪽이 천장인가?
		iCnt = iTileX + ((iTileY + 1) * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
			// ★
			// ■
			byTileDirection = byTileDirection | df_DIRECTION_DD;

			//--------------------------------------
			// ↙ 검사
			//--------------------------------------
			// 아래 왼쪽이 맵의 끝인가?
			if (iTileX - 1 < 0) {
				// 　★
				// ▣■
				byTileDirection = byTileDirection | df_DIRECTION_DL;
			}
			// 왼쪽이 맵의 끝이 아닌 상태에서
			else {
				// 아래 왼쪽이 천장인가?
				iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// 　★
					// ■■
					byTileDirection = byTileDirection | df_DIRECTION_DL;
				}
			}

			//--------------------------------------
			// ↘ 검사
			//--------------------------------------
			// 아래 오른쪽이 맵의 끝인가?
			if (iTileX + 1 >= m_iTileWidth) {
				// ★
				// ■▣
				byTileDirection = byTileDirection | df_DIRECTION_DR;
			}
			// 왼쪽이 맵의 끝이 아닌 상태에서
			else {
				// 아래 오른쪽이 천장인가?
				iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ★
					// ■■
					byTileDirection = byTileDirection | df_DIRECTION_DR;
				}
			}
		}
		// 아래쪽이 하단 벽인가?
		else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_BOTTOM) {
			// ★
			// ▥

			// 2칸 아래가 맵의 끝인가?
			if (iTileY + 2 >= m_iTileHeight) {
				// ★
				// ▥
				// ▣
				// 벽은 천장으로 교체될 것임
				byTileDirection = byTileDirection | df_DIRECTION_DD;

				//--------------------------------------
				// ↙ 검사
				//--------------------------------------
				// 아래 왼쪽이 맵의 끝인가?
				if (iTileX - 1 < 0) {
					// 　★
					// ▣■
					byTileDirection = byTileDirection | df_DIRECTION_DL;
				}
				// 왼쪽이 맵의 끝이 아닌 상태에서
				else {
					// 아래 왼쪽이 천장인가?
					iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						// 　★
						// ■■
						byTileDirection = byTileDirection | df_DIRECTION_DL;
					}
				}

				//--------------------------------------
				// ↘ 검사
				//--------------------------------------
				// 아래 오른쪽이 맵의 끝인가?
				if (iTileX + 1 >= m_iTileWidth) {
					// ★
					// ■▣
					byTileDirection = byTileDirection | df_DIRECTION_DR;
				}
				// 왼쪽이 맵의 끝이 아닌 상태에서
				else {
					// 아래 오른쪽이 천장인가?
					iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						// ★
						// ■■
						byTileDirection = byTileDirection | df_DIRECTION_DR;
					}
				}
			}
			// 2칸 아래쪽이 맵의 끝이 아닌 상태에서
			else {
				// 2칸 아래쪽이 천장인가?	
				iCnt = iTileX + ((iTileY + 2) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ★
					// ▥
					// ■
					// 벽은 천장으로 교체될 것임
					byTileDirection = byTileDirection | df_DIRECTION_DD;

					//--------------------------------------
					// ↙ 검사
					//--------------------------------------
					// 아래 왼쪽이 맵의 끝인가?
					if (iTileX - 1 < 0) {
						// 　★
						// ▣■
						byTileDirection = byTileDirection | df_DIRECTION_DL;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 왼쪽이 천장인가?
						iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// 　★
							// ■■
							byTileDirection = byTileDirection | df_DIRECTION_DL;
						}
					}

					//--------------------------------------
					// ↘ 검사
					//--------------------------------------
					// 아래 오른쪽이 맵의 끝인가?
					if (iTileX + 1 >= m_iTileWidth) {
						// ★
						// ■▣
						byTileDirection = byTileDirection | df_DIRECTION_DR;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 오른쪽이 천장인가?
						iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ★
							// ■■
							byTileDirection = byTileDirection | df_DIRECTION_DR;
						}
					}
				}
				// 2칸 아래쪽이 바닥인가?	
				else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
					// ★
					// ▥
					// □

					// 천장타일이 다른 타일로 바뀌는 상황
					if (!bCertainlyBorderFlag) {
						//--------------------------------------
						// 타일 변경
						// 천장 -> 상단 벽으로 변경
						//--------------------------------------
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iTileState = df_TILE_STATE_WALL_TOP;
						switch (iSelectedTileType)
						{
						case df_TILE_TYPE_LIBRARY:
							m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_TOP;
							m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
							break;
						case df_TILE_TYPE_CASTLE:
							m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_WALL_TOP;
							m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
							break;
						default:
							// 에러
							throw;
						}

						//-------------------------------------------
						// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
						//-------------------------------------------
						// 현제 타일 왼쪽에 유닛이 있나?
						if (iTileX - 1 >= 0) {
							iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
							if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
								//----------------------------------------------------------
								// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
								// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
								//----------------------------------------------------------
								DeleteUnitProcess(iTileX - 1, iTileY);
							}
						}

						if (iTileX + 1 < m_iTileWidth) {
							iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
							if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
								//----------------------------------------------------------
								// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
								// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
								//----------------------------------------------------------
								DeleteUnitProcess(iTileX + 1, iTileY);
							}
						}

						// 꼭 빠져나와야 함
						return;
					}
				}
			}
		}
		// 아래쪽이 바닥 인가?
		else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
			// ★
			// □

			// 2칸 아래가 맵의 끝인가?
			if (iTileY + 2 >= m_iTileHeight) {
				// ★
				// □
				// ▣

				// 꼭 천장으로 만들어야하는 옵션이면
				if (bCertainlyBorderFlag) {
					// 벽은 천장으로 교체될 것임
					byTileDirection = byTileDirection | df_DIRECTION_DD;

					//--------------------------------------
					// ↙ 검사
					//--------------------------------------
					// 아래 왼쪽이 맵의 끝인가?
					if (iTileX - 1 < 0) {
						// 　★
						// ▣□
						//   ▣
						byTileDirection = byTileDirection | df_DIRECTION_DL;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 왼쪽이 천장인가?
						iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// 　★
							// ■□
							//   ▣
							byTileDirection = byTileDirection | df_DIRECTION_DL;
						}
					}

					//--------------------------------------
					// ↘ 검사
					//--------------------------------------
					// 아래 오른쪽이 맵의 끝인가?
					if (iTileX + 1 >= m_iTileWidth) {
						// ★
						// □▣
						// ▣
						byTileDirection = byTileDirection | df_DIRECTION_DR;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 오른쪽이 천장인가?
						iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ★
							// □■
							// ▣
							byTileDirection = byTileDirection | df_DIRECTION_DR;
						}
					}
				}
				// 천장으로 만들어야하는 상황이 아님
				else {
					// ★
					// □
					// ▣

					// 위쪽이 맵의 끝인가?
					if (iTileY - 1 < 0) {
						// ▣
						// ★
						// □
						// ▣

						//--------------------------------------
						// 타일 변경
						// 천장 -> 바닥으로 변경
						//--------------------------------------
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
						switch (iSelectedTileType)
						{
						case df_TILE_TYPE_LIBRARY:
							m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
							m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
							break;
						case df_TILE_TYPE_CASTLE:
							//--------------------------------------
							// 바닥 타일이 두종류임
							//--------------------------------------
							if (rand() % 2) {
								m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
							}
							else {
								m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
							}
							m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
							break;
						default:
							// 에러
							throw;
						}

						//-------------------------------------------
						// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
						//-------------------------------------------
						// 현제 타일 왼쪽에 유닛이 있나?
						if (iTileX - 1 >= 0) {
							iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
							if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
								//----------------------------------------------------------
								// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
								// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
								//----------------------------------------------------------
								DeleteUnitProcess(iTileX - 1, iTileY);
							}
						}

						if (iTileX + 1 < m_iTileWidth) {
							iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
							if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
								//----------------------------------------------------------
								// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
								// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
								//----------------------------------------------------------
								DeleteUnitProcess(iTileX + 1, iTileY);
							}
						}

						// 꼭 빠져나와야 함
						return;
					}
					// 위쪽이 맵의 끝이 아닌 상태에서
					else {
						// 위쪽이 천장인가?	
						iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ■
							// ★
							// □
							// ▣

							// 2칸 위쪽이 맵의 끝인가?
							if (iTileY - 2 < 0) {
								// ▣
								// ■
								// ★
								// □
								// ▣

								//--------------------------------------
								// 타일 변경
								// 천장 -> 바닥으로 변경
								//--------------------------------------
								iCnt = iTileX + (iTileY * m_iTileWidth);
								m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
								switch (iSelectedTileType)
								{
								case df_TILE_TYPE_LIBRARY:
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
									break;
								case df_TILE_TYPE_CASTLE:
									//--------------------------------------
									// 바닥 타일이 두종류임
									//--------------------------------------
									if (rand() % 2) {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
									}
									else {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
									}
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
									break;
								default:
									// 에러
									throw;
								}

								//-------------------------------------------
								// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
								//-------------------------------------------
								// 현제 타일 왼쪽에 유닛이 있나?
								if (iTileX - 1 >= 0) {
									iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX - 1, iTileY);
									}
								}

								if (iTileX + 1 < m_iTileWidth) {
									iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX + 1, iTileY);
									}
								}

								// 꼭 빠져나와야 함
								return;
							}
							// 2칸 위쪽이 맵의 끝이 아닌 상태에서
							else {
								// 2칸 위쪽이 천장인가?	
								iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
								if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
									// ■
									// ■
									// ★
									// □
									// ▣

									//--------------------------------------
									// 타일 변경
									// 천장 -> 하단 벽으로 변경
									//--------------------------------------
									iCnt = iTileX + (iTileY * m_iTileWidth);
									m_stpTileMap[iCnt].iTileState = df_TILE_STATE_WALL_BOTTOM;
									switch (iSelectedTileType)
									{
									case df_TILE_TYPE_LIBRARY:
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
										break;
									case df_TILE_TYPE_CASTLE:
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
										break;
									default:
										// 에러
										throw;
									}

									//-------------------------------------------
									// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
									//-------------------------------------------
									// 현제 타일 왼쪽에 유닛이 있나?
									if (iTileX - 1 >= 0) {
										iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX - 1, iTileY);
										}
									}

									if (iTileX + 1 < m_iTileWidth) {
										iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX + 1, iTileY);
										}
									}

									// 꼭 빠져나와야 함
									return;
								}
								// 2칸 위쪽이 천장이 아니면
								else {
									// ?
									// ■
									// ★
									// □
									// ▣

									//--------------------------------------
									// 타일 변경
									// 천장 -> 바닥으로 변경
									//--------------------------------------
									iCnt = iTileX + (iTileY * m_iTileWidth);
									m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
									switch (iSelectedTileType)
									{
									case df_TILE_TYPE_LIBRARY:
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
										break;
									case df_TILE_TYPE_CASTLE:
										//--------------------------------------
										// 바닥 타일이 두종류임
										//--------------------------------------
										if (rand() % 2) {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
										}
										else {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
										}
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
										break;
									default:
										// 에러
										throw;
									}

									//-------------------------------------------
									// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
									//-------------------------------------------
									// 현제 타일 왼쪽에 유닛이 있나?
									if (iTileX - 1 >= 0) {
										iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX - 1, iTileY);
										}
									}

									if (iTileX + 1 < m_iTileWidth) {
										iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX + 1, iTileY);
										}
									}

									// 꼭 빠져나와야 함
									return;
								}
							}
						}
						// 위쪽이 바닥인가?
						else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
							// □
							// ★
							// □
							// ▣

							//--------------------------------------
							// 타일 변경
							// 천장 -> 바닥으로 변경
							//--------------------------------------
							iCnt = iTileX + (iTileY * m_iTileWidth);
							m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
							switch (iSelectedTileType)
							{
							case df_TILE_TYPE_LIBRARY:
								m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
								break;
							case df_TILE_TYPE_CASTLE:
								//--------------------------------------
								// 바닥 타일이 두종류임
								//--------------------------------------
								if (rand() % 2) {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
								}
								else {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
								}
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
								break;
							default:
								// 에러
								throw;
							}

							//-------------------------------------------
							// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
							//-------------------------------------------
							// 현제 타일 왼쪽에 유닛이 있나?
							if (iTileX - 1 >= 0) {
								iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX - 1, iTileY);
								}
							}

							if (iTileX + 1 < m_iTileWidth) {
								iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX + 1, iTileY);
								}
							}

							// 꼭 빠져나와야 함
							return;
						}
					}
				}
			}
			// 2칸 아래쪽이 맵의 끝이 아닌 상태에서
			else {
				// 2칸 아래쪽이 천장인가?	
				iCnt = iTileX + ((iTileY + 2) * m_iTileWidth);
				if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
					// ★
					// □
					// ■

					// 꼭 천장으로 만들어야하는 옵션이면
					if (bCertainlyBorderFlag) {
						// 바닥은 천장으로 교체될 것임
						byTileDirection = byTileDirection | df_DIRECTION_DD;

						//--------------------------------------
						// ↙ 검사
						//--------------------------------------
						// 아래 왼쪽이 맵의 끝인가?
						if (iTileX - 1 < 0) {
							// 　★
							// ▣■
							byTileDirection = byTileDirection | df_DIRECTION_DL;
						}
						// 왼쪽이 맵의 끝이 아닌 상태에서
						else {
							// 아래 왼쪽이 천장인가?
							iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// 　★
								// ■■
								byTileDirection = byTileDirection | df_DIRECTION_DL;
							}
						}

						//--------------------------------------
						// ↘ 검사
						//--------------------------------------
						// 아래 오른쪽이 맵의 끝인가?
						if (iTileX + 1 >= m_iTileWidth) {
							// ★
							// ■▣
							byTileDirection = byTileDirection | df_DIRECTION_DR;
						}
						// 왼쪽이 맵의 끝이 아닌 상태에서
						else {
							// 아래 오른쪽이 천장인가?
							iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// ★
								// ■■
								byTileDirection = byTileDirection | df_DIRECTION_DR;
							}
						}
					}
					// 천장으로 만들어야하는 상황이 아님
					else {
						// ★
						// □
						// ■

						// 위쪽이 맵의 끝인가?
						if (iTileY - 1 < 0) {
							// ▣
							// ★
							// □
							// ■

							//--------------------------------------
							// 타일 변경
							// 천장 -> 바닥으로 변경
							//--------------------------------------
							iCnt = iTileX + (iTileY * m_iTileWidth);
							m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
							switch (iSelectedTileType)
							{
							case df_TILE_TYPE_LIBRARY:
								m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
								break;
							case df_TILE_TYPE_CASTLE:
								//--------------------------------------
								// 바닥 타일이 두종류임
								//--------------------------------------
								if (rand() % 2) {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
								}
								else {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
								}
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
								break;
							default:
								// 에러
								throw;
							}

							//-------------------------------------------
							// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
							//-------------------------------------------
							// 현제 타일 왼쪽에 유닛이 있나?
							if (iTileX - 1 >= 0) {
								iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX - 1, iTileY);
								}
							}

							if (iTileX + 1 < m_iTileWidth) {
								iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX + 1, iTileY);
								}
							}

							// 꼭 빠져나와야 함
							return;
						}
						// 위쪽이 맵의 끝이 아닌 상태에서
						else {
							// 위쪽이 천장인가?	
							iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// ■
								// ★
								// □
								// ■

								// 2칸 위쪽이 맵의 끝인가?
								if (iTileY - 2 < 0) {
									// ▣
									// ■
									// ★
									// □
									// ■

									//--------------------------------------
									// 타일 변경
									// 천장 -> 바닥으로 변경
									//--------------------------------------
									iCnt = iTileX + (iTileY * m_iTileWidth);
									m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
									switch (iSelectedTileType)
									{
									case df_TILE_TYPE_LIBRARY:
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
										break;
									case df_TILE_TYPE_CASTLE:
										//--------------------------------------
										// 바닥 타일이 두종류임
										//--------------------------------------
										if (rand() % 2) {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
										}
										else {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
										}
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
										break;
									default:
										// 에러
										throw;
									}

									//-------------------------------------------
									// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
									//-------------------------------------------
									// 현제 타일 왼쪽에 유닛이 있나?
									if (iTileX - 1 >= 0) {
										iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX - 1, iTileY);
										}
									}

									if (iTileX + 1 < m_iTileWidth) {
										iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
										if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
											//----------------------------------------------------------
											// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
											// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
											//----------------------------------------------------------
											DeleteUnitProcess(iTileX + 1, iTileY);
										}
									}

									// 꼭 빠져나와야 함
									return;
								}
								// 2칸 위쪽이 맵의 끝이 아닌 상태에서
								else {
									// 2칸 위쪽이 천장인가?	
									iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
									if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
										// ■
										// ■
										// ★
										// □
										// ■

										//--------------------------------------
										// 타일 변경
										// 천장 -> 하단 벽으로 변경
										//--------------------------------------
										iCnt = iTileX + (iTileY * m_iTileWidth);
										m_stpTileMap[iCnt].iTileState = df_TILE_STATE_WALL_BOTTOM;
										switch (iSelectedTileType)
										{
										case df_TILE_TYPE_LIBRARY:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
											break;
										case df_TILE_TYPE_CASTLE:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
											break;
										default:
											// 에러
											throw;
										}

										//-------------------------------------------
										// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
										//-------------------------------------------
										// 현제 타일 왼쪽에 유닛이 있나?
										if (iTileX - 1 >= 0) {
											iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX - 1, iTileY);
											}
										}

										if (iTileX + 1 < m_iTileWidth) {
											iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX + 1, iTileY);
											}
										}

										// 꼭 빠져나와야 함
										return;
									}
									// 2칸 위쪽이 천장이 아니면
									else {
										// ?
										// ■
										// ★
										// □
										// ■

										//--------------------------------------
										// 타일 변경
										// 천장 -> 바닥으로 변경
										//--------------------------------------
										iCnt = iTileX + (iTileY * m_iTileWidth);
										m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
										switch (iSelectedTileType)
										{
										case df_TILE_TYPE_LIBRARY:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
											break;
										case df_TILE_TYPE_CASTLE:
											//--------------------------------------
											// 바닥 타일이 두종류임
											//--------------------------------------
											if (rand() % 2) {
												m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
											}
											else {
												m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
											}
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
											break;
										default:
											// 에러
											throw;
										}

										//-------------------------------------------
										// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
										//-------------------------------------------
										// 현제 타일 왼쪽에 유닛이 있나?
										if (iTileX - 1 >= 0) {
											iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX - 1, iTileY);
											}
										}

										if (iTileX + 1 < m_iTileWidth) {
											iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX + 1, iTileY);
											}
										}

										// 꼭 빠져나와야 함
										return;
									}
								}
							}
							// 위쪽이 천장이 아니라면
							else {
								// ?
								// ★
								// □
								// ■

								//--------------------------------------
								// 타일 변경
								// 천장 -> 바닥으로 변경
								//--------------------------------------
								iCnt = iTileX + (iTileY * m_iTileWidth);
								m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
								switch (iSelectedTileType)
								{
								case df_TILE_TYPE_LIBRARY:
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
									break;
								case df_TILE_TYPE_CASTLE:
									//--------------------------------------
									// 바닥 타일이 두종류임
									//--------------------------------------
									if (rand() % 2) {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
									}
									else {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
									}
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
									break;
								default:
									// 에러
									throw;
								}

								//-------------------------------------------
								// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
								//-------------------------------------------
								// 현제 타일 왼쪽에 유닛이 있나?
								if (iTileX - 1 >= 0) {
									iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX - 1, iTileY);
									}
								}

								if (iTileX + 1 < m_iTileWidth) {
									iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX + 1, iTileY);
									}
								}

								// 꼭 빠져나와야 함
								return;
							}
						}
					}
				}
				// 2칸 아래쪽이 바닥인가?	
				else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
					// ★
					// □
					// □

					// 꼭 천장으로 만들어야하는 옵션이면
					if (bCertainlyBorderFlag) {

						//--------------------------------------
						// ↙ 검사
						//--------------------------------------
						// 아래 왼쪽이 맵의 끝인가?
						if (iTileX - 1 < 0) {
							// 　★
							// ▣□
							//   □
							byTileDirection = byTileDirection | df_DIRECTION_DL;
						}
						// 왼쪽이 맵의 끝이 아닌 상태에서
						else {
							// 아래 왼쪽이 천장인가?
							iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// 　★
								// ■□
								//   □
								byTileDirection = byTileDirection | df_DIRECTION_DL;
							}
						}

						//--------------------------------------
						// ↘ 검사
						//--------------------------------------
						// 아래 오른쪽이 맵의 끝인가?
						if (iTileX + 1 >= m_iTileWidth) {
							// ★
							// □▣
							// □
							byTileDirection = byTileDirection | df_DIRECTION_DR;
						}
						// 왼쪽이 맵의 끝이 아닌 상태에서
						else {
							// 아래 오른쪽이 천장인가?
							iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// ★
								// □■
								// □
								byTileDirection = byTileDirection | df_DIRECTION_DR;
							}
						}
					}
					// 천장으로 만들어야하는 상황이 아님
					else {
						// ★
						// □
						// □

						// 위쪽이 맵의 끝인가?
						if (iTileY - 1 < 0) {
							// ▣
							// ★
							// □
							// □

							//--------------------------------------
							// 타일 변경
							// 천장 -> 바닥으로 변경
							//--------------------------------------
							iCnt = iTileX + (iTileY * m_iTileWidth);
							m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
							switch (iSelectedTileType)
							{
							case df_TILE_TYPE_LIBRARY:
								m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
								break;
							case df_TILE_TYPE_CASTLE:
								//--------------------------------------
								// 바닥 타일이 두종류임
								//--------------------------------------
								if (rand() % 2) {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
								}
								else {
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
								}
								m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
								break;
							default:
								// 에러
								throw;
							}

							//-------------------------------------------
							// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
							//-------------------------------------------
							// 현제 타일 왼쪽에 유닛이 있나?
							if (iTileX - 1 >= 0) {
								iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX - 1, iTileY);
								}
							}

							if (iTileX + 1 < m_iTileWidth) {
								iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
								if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX + 1, iTileY);
								}
							}

							// 꼭 빠져나와야 함
							return;
						}
						// 위쪽이 맵의 끝이 아니라면
						else {
							// 위쪽이 천장인가?
							iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// ■
								// ★
								// □
								// □

								// 2칸 위쪽이 맵의 끝인가?
								if (iTileY - 2 < 0) {
									// ▣
									// ■
									// ★
									// □
									// □

									//--------------------------------------
									// 타일 변경
									// 천장 -> 바닥으로 변경
									//--------------------------------------
									iCnt = iTileX + (iTileY * m_iTileWidth);
									m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
									switch (iSelectedTileType)
									{
									case df_TILE_TYPE_LIBRARY:
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
										break;
									case df_TILE_TYPE_CASTLE:
										//--------------------------------------
										// 바닥 타일이 두종류임
										//--------------------------------------
										if (rand() % 2) {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
										}
										else {
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
										}
										m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
										break;
									default:
										// 에러
										throw;
									}

									// 꼭 빠져나와야 함
									return;
								}
								// 2칸 위쪽이 맵의 끝이 아닌 상태에서
								else {
									// 2칸 위쪽이 천장인가?
									iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
									if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
										// ■
										// ■
										// ★
										// □
										// □

										//--------------------------------------
										// 타일 변경
										// 천장 -> 하단 벽으로 변경
										//--------------------------------------
										iCnt = iTileX + (iTileY * m_iTileWidth);
										m_stpTileMap[iCnt].iTileState = df_TILE_STATE_WALL_BOTTOM;
										switch (iSelectedTileType)
										{
										case df_TILE_TYPE_LIBRARY:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
											break;
										case df_TILE_TYPE_CASTLE:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
											break;
										default:
											// 에러
											throw;
										}

										//-------------------------------------------
										// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
										//-------------------------------------------
										// 현제 타일 왼쪽에 유닛이 있나?
										if (iTileX - 1 >= 0) {
											iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX - 1, iTileY);
											}
										}

										if (iTileX + 1 < m_iTileWidth) {
											iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX + 1, iTileY);
											}
										}

										// 꼭 빠져나와야 함
										return;
									}
									// 2칸 위쪽이 천장이 아니라면
									else {
										// ?
										// ■
										// ★
										// □
										// □

										//--------------------------------------
										// 타일 변경
										// 천장 -> 바닥으로 변경
										//--------------------------------------
										iCnt = iTileX + (iTileY * m_iTileWidth);
										m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
										switch (iSelectedTileType)
										{
										case df_TILE_TYPE_LIBRARY:
											m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
											break;
										case df_TILE_TYPE_CASTLE:
											//--------------------------------------
											// 바닥 타일이 두종류임
											//--------------------------------------
											if (rand() % 2) {
												m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
											}
											else {
												m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
											}
											m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
											break;
										default:
											// 에러
											throw;
										}

										//-------------------------------------------
										// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
										//-------------------------------------------
										// 현제 타일 왼쪽에 유닛이 있나?
										if (iTileX - 1 >= 0) {
											iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX - 1, iTileY);
											}
										}

										if (iTileX + 1 < m_iTileWidth) {
											iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
											if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
												//----------------------------------------------------------
												// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
												// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
												//----------------------------------------------------------
												DeleteUnitProcess(iTileX + 1, iTileY);
											}
										}

										// 꼭 빠져나와야 함
										return;
									}
								}
							}
							// 위쪽이 천장이 아니라면
							else {
								// ?
								// ★
								// □
								// □

								//--------------------------------------
								// 타일 변경
								// 천장 -> 바닥으로 변경
								//--------------------------------------
								iCnt = iTileX + (iTileY * m_iTileWidth);
								m_stpTileMap[iCnt].iTileState = df_TILE_STATE_FLOOR;
								switch (iSelectedTileType)
								{
								case df_TILE_TYPE_LIBRARY:
									m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
									break;
								case df_TILE_TYPE_CASTLE:
									//--------------------------------------
									// 바닥 타일이 두종류임
									//--------------------------------------
									if (rand() % 2) {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
									}
									else {
										m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
									}
									m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
									break;
								default:
									// 에러
									throw;
								}

								//-------------------------------------------
								// 랜턴의 경우 천장 좌, 우쪽에 붙어있음
								//-------------------------------------------
								// 현제 타일 왼쪽에 유닛이 있나?
								if (iTileX - 1 >= 0) {
									iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX - 1, iTileY);
									}
								}

								if (iTileX + 1 < m_iTileWidth) {
									iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
									if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
										//----------------------------------------------------------
										// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
										// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
										//----------------------------------------------------------
										DeleteUnitProcess(iTileX + 1, iTileY);
									}
								}

								// 꼭 빠져나와야 함
								return;
							}
						}
					}
				}
			}
		}
		// 아래쪽이 상단 벽 인가?
		else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_TOP) {
			// ★
			// ▥

			// 2칸 아래가 맵의 끝인가?
			if (iTileY + 2 >= m_iTileHeight) {
				// ★
				// ▥
				// ▣

				// 꼭 천장으로 만들어야하는 옵션이면
				if (bCertainlyBorderFlag) {
					// 벽은 천장으로 교체될 것임
					byTileDirection = byTileDirection | df_DIRECTION_DD;

					//--------------------------------------
					// ↙ 검사
					//--------------------------------------
					// 아래 왼쪽이 맵의 끝인가?
					if (iTileX - 1 < 0) {
						// 　★
						// ▣■
						byTileDirection = byTileDirection | df_DIRECTION_DL;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 왼쪽이 천장인가?
						iCnt = (iTileX - 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// 　★
							// ■■
							byTileDirection = byTileDirection | df_DIRECTION_DL;
						}
					}

					//--------------------------------------
					// ↘ 검사
					//--------------------------------------
					// 아래 오른쪽이 맵의 끝인가?
					if (iTileX + 1 >= m_iTileWidth) {
						// ★
						// ■▣
						byTileDirection = byTileDirection | df_DIRECTION_DR;
					}
					// 왼쪽이 맵의 끝이 아닌 상태에서
					else {
						// 아래 오른쪽이 천장인가?
						iCnt = (iTileX + 1) + ((iTileY + 1) * m_iTileWidth);
						if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
							// ★
							// ■■
							byTileDirection = byTileDirection | df_DIRECTION_DR;
						}
					}
				}
				// 천장으로 만들어야하는 상황이 아님
				else {
					// ★
					// ▥
					// ▣

					// 이런 모양이 나올 수가 없음
					// 에러
					throw;
				}
			}
			// 2칸 아래가 맵의 끝이 아니면
			else {
				// ★
				// ▥	// 상단 벽

				// 할게 없음
				// 천장을 같은 위치에 클릭인 경우
			}

		}
	}


	//--------------------------------------
	// 왼쪽 검사
	//--------------------------------------
	// 왼쪽이 맵의 끝인가?
	if (iTileX - 1 < 0) {
		byTileDirection = byTileDirection | df_DIRECTION_LL;
	}
	// 왼쪽이 맵의 끝이 아닌 상태에서
	else {
		// 왼쪽이 천장인가?
		iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
			byTileDirection = byTileDirection | df_DIRECTION_LL;
		}
	}

	//--------------------------------------
	// 오른쪽 검사
	//--------------------------------------
	// 오른쪽이 맵의 끝인가?
	if (iTileX + 1 >= m_iTileWidth) {
		byTileDirection = byTileDirection | df_DIRECTION_RR;
	}
	// 오른쪽이 맵의 끝이 아닌 상태에서
	else {
		// 오른쪽이 천장인가?
		iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
			byTileDirection = byTileDirection | df_DIRECTION_RR;
		}
	}



	//--------------------------------------
	// 최종 심사
	// 최종 타일 모양을 결정한다
	//--------------------------------------
	switch (byTileDirection)
	{
	case df_DIRECTION_EMPT:
	case df_DIRECTION_DR:
	case df_DIRECTION_DL:
	case (df_DIRECTION_DR + df_DIRECTION_DL):
		// 모든 방향이 해당 안됨
		// 　　　★★★
		// 　　　★☆★
		// 　　　★★★
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_45;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_45;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_DIRECTION_ALL:
		// 모든 방향
		// ☆☆☆
		// ☆☆☆
		// ☆☆☆

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_WALL;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_WALL;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_DIRECTION_UU:
	case (df_DIRECTION_UU + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_DL + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_DR + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DL):						// 위쪽이 맵의 끝인 경우
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DR):						// 위쪽이 맵의 끝인 경우
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DL + df_DIRECTION_DR):		// 위쪽이 맵의 끝인 경우
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■
		// 　　　★☆★
		// 　　　★☆★
		// 　　　★★★
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■
		
		// 위쪽 뚫려있음
		// ■□■
		// ■□■
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_42;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_42;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_DIRECTION_LL:
	case (df_DIRECTION_LL + df_DIRECTION_UL):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_DL):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DL):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■■■
		// ■□■★★★
		// ■□■☆☆★
		// ■□■★★★
		// ■□■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 왼쪽 뚫려있음
		// ■■■
		// □□■
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_40;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_40;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_DIRECTION_RR:
	case (df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DL):
	case (df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_UL):
	case (df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_RR + df_DIRECTION_DR + df_DIRECTION_UL):
	case (df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR):
	case (df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_UL):
	case (df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DL):
	case (df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■□■
		// 　　　★★★■□■
		// 　　　★☆☆■□■
		// 　　　★★★■□■
		// ■■■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 오른쪽 뚫려있음
		// ■■■
		// ■□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_39;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_39;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case df_DIRECTION_DD:
	case (df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_UL):		// 아래가 맵의 끝인 경우
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_UR):		// 아래가 맵의 끝인 경우
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_UL + df_DIRECTION_UR):		// 아래가 맵의 끝인 경우
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■
		// 　　　★★★
		// 　　　★☆★
		// 　　　★☆★
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래쪽 뚫려있음
		// ■■■
		// ■□■
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_41;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_41;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UL +df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■
		// 　　　★☆★
		// 　　　★☆★
		// 　　　★☆★
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 위쪽 뚫려있음
		// ■□■
		// ■□■
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_44;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_44;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_RR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_RR):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■□■★★★■□■
		// ■□■☆☆☆■□■
		// ■□■★★★■□■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 왼쪽 오른쪽 뚫려있음
		// ■■■
		// □□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_43;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_43;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_DR):		// 위 왼쪽이 맵의 끝인 경우
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_DL + df_DIRECTION_DR):		// 위 왼쪽이 맵의 끝인 경우
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■■■
		// ■□■☆☆★
		// ■□■☆☆★
		// ■□■★★★
		// ■□■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 왼쪽 대각선 뚫려있음
		// □□■
		// □□■
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_11;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_11;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_DL):		// 위 오른쪽이 맵의 끝인 경우
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_DR + df_DIRECTION_DL):		// 위 오른쪽이 맵의 끝인 경우
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■□■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// 　　　★★★■□■
		// ■■■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 오른쪽 대각선 뚫려있음
		// ■□□
		// ■□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_12;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_12;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UR + df_DIRECTION_DL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■□■
		// 　　　★★★■□■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// ■■■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■
		
		// 아래 오른쪽 대각선 뚫려있음
		// ■■■
		// ■□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_09;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_09;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_DL):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■■■
		// ■□■★★★
		// ■□■☆☆★
		// ■□■☆☆★
		// ■□■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 왼쪽 대각선 뚫려있음
		// ■■■
		// □□■
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_10;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_10;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_LL):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_DR):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■
		// ■■■★☆★
		// ■□■☆☆★
		// ■□■★★★
		// ■□■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 왼쪽 뚫려있음
		// ■□■
		// □□■
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_31;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_31;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■■■■　　　
		// ■□□□□■　　　
		// ■■■■■■　　　
		// 　　　★☆★■■■
		// 　　　★☆☆■□■
		// 　　　★★★■□■
		// ■■■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 오른쪽 뚫려있음
		// ■□■
		// ■□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_32;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_32;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_RR):
	case (df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■■■　　　■□■
		// 　　　★★★■□■
		// 　　　★☆☆■□■
		// 　　　★☆★■■■
		// ■■■■■■　　　
		// ■□□□□■　　　
		// ■■■■■■　　　

		// 아래 오른쪽 뚫려있음
		// ■■■
		// ■□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_33;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_33;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_LL):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_UL):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■■■
		// ■□■★★★
		// ■□■☆☆★
		// ■■■★☆★
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 아래 왼쪽 뚫려있음
		// ■■■
		// □□■
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_34;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_34;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_DR):
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		// ■■■★☆★■■■
		// ■□■☆☆☆■□■
		// ■□■★★★■□■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 왼쪽 오른쪽 뚫려있음
		// ■□■
		// □□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_38;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_38;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■□■★★★■□■
		// ■□■☆☆☆■□■
		// ■■■★☆★■■■
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		

		// 아래 왼쪽 오른쪽 뚫려있음
		// ■■■
		// □□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_36;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_36;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_DR):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■
		// ■■■★☆★
		// ■□■☆☆★
		// ■■■★☆★
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 위 왼쪽 아래 뚫려있음
		// ■□■
		// □□■
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_37;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_37;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DL):
		// ■■■■■■　　　
		// ■□□□□■　　　
		// ■■■■■■　　　
		// 　　　★☆★■■■
		// 　　　★☆☆■□■
		// 　　　★☆★■■■
		// ■■■■■■
		// ■□□□□■
		// ■■■■■■

		// 위 오른쪽 아래 뚫려있음
		// ■□■
		// ■□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_35;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_35;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■■■■
		// ■□□□□■
		// ■□■■■■
		// ■□■☆☆★■■■
		// ■□■☆☆☆■□■
		// ■□■★★★■□■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 왼쪽 대각선 오른쪽 뚫려있음
		// □□■
		// □□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_24;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_24;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_LL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_DL + df_DIRECTION_DR):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■□■
		// ■■■★☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■★★★■□■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 오른쪽 대각선 왼쪽 뚫려있음
		// ■□□
		// □□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_23;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_23;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_LL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■□■★★★■□■
		// ■□■☆☆☆■□■
		// ■■■★☆☆■□■
		// 　　　■■■■□■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 아래 오른쪽 대각선 왼쪽 뚫려있음
		// ■■■
		// □□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_26;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_26;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■□■★★★■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆★■■■
		// ■□■■■■　　　
		// ■□□□□■　　　
		// ■■■■■■　　　

		// 아래 왼쪽 대각선 오른쪽 뚫려있음
		// ■■■
		// □□□
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_25;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_25;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_UR + df_DIRECTION_DR):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■■■
		// ■□■☆☆★
		// ■□■☆☆★
		// ■■■★☆★
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 위 왼쪽 대각선 아래 뚫려있음
		// □□■
		// □□■
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_28;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_28;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UU):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UU + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UU + df_DIRECTION_DR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■■■
		// ■■■★☆★
		// ■□■☆☆★
		// ■□■☆☆★
		// ■□■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 왼쪽 대각선 위 뚫려있음
		// ■□■
		// □□■
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_27;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_27;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DD):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_UL + df_DIRECTION_DL):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■□■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// 　　　★☆★■■■
		// ■■■■■■
		// ■□□□□■
		// ■■■■■■

		// 위 오른쪽 대각선 아래 뚫려있음
		// ■□□
		// ■□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_30;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_30;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UU):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UU + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UU + df_DIRECTION_DL):
		// ■■■■■■
		// ■□□□□■
		// ■■■■■■
		// 　　　★☆★■■■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// ■■■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 오른쪽 대각선 아래 뚫려있음
		// ■□■
		// ■□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_29;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_29;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■★★★■□■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■■■　　　■■■

		// 위 왼쪽 대각선 오른쪽 대각선 뚫려있음
		// □□□
		// □□□
		// ■■■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_06;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_06;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_LL + df_DIRECTION_RR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UR):
	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_UR):
		// ■■■　　　■■■
		// ■□■　　　■□■
		// ■□■　　　■□■
		// ■□■★★★■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 왼쪽 대각선 오른쪽 대각선 뚫려있음
		// ■■■
		// □□□
		// □□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_05;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_05;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DD + df_DIRECTION_LL):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_DR):
	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_UR + df_DIRECTION_DR):	// 위쪽이 맵의 끝인 경우
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■■■
		// ■□■☆☆★　　　
		// ■□■☆☆★　　　
		// ■□■☆☆★　　　
		// ■□■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 위 왼쪽 대각선 아래 왼쪽 대각선 뚫려있음
		// □□■
		// □□■
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_08;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_08;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DD + df_DIRECTION_RR):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_UL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_DL):
	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_UL + df_DIRECTION_DL):	// 위쪽이 맵의 끝인 경우
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■■■■■■■□■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// 　　　★☆☆■□■
		// ■■■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 위 오른쪽 대각선 아래 오른쪽 대각선 뚫려있음
		// ■□□
		// ■□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_07;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_07;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_LL):
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		// ■■■★☆★■■■
		// ■□■☆☆☆■□■
		// ■■■★☆★■■■
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■

		// 위 오른쪽 아래 왼쪽 대각선 뚫려있음
		// ■□■
		// □□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_46;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_46;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_RR):
		// ■■■■■■
		// ■□□□□■
		// ■□■■■■
		// ■□■☆☆★■■■
		// ■□■☆☆☆■□■
		// ■■■★☆★■■■
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■

		// 위 왼쪽 대각선 아래 오른쪽 뚫려있음
		// □□■
		// □□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_21;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_21;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_LL):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■□■
		// ■■■★☆☆■□■
		// ■□■☆☆☆■□■
		// ■■■★☆★■■■
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■

		// 위 오른쪽 대각선 아래 왼쪽 뚫려있음
		// ■□□
		// □□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_22;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_22;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DR + df_DIRECTION_RR + df_DIRECTION_UU + df_DIRECTION_LL):
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		// ■■■★☆★■■■
		// ■□■☆☆☆■□■
		// ■■■★☆☆■□■
		// 　　　■■■■□■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 아래 오른쪽 대각선 위 왼쪽 뚫려있음
		// ■□■
		// □□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_19;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_19;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_LL + df_DIRECTION_UU + df_DIRECTION_RR):
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		// ■■■★☆★■■■
		// ■□■☆☆☆■□■
		// ■□■☆☆★■■■
		// ■□■■■■
		// ■□□□□■
		// ■■■■■■

		// 아래 왼쪽 대각선 위 오른쪽 뚫려있음
		// ■□■
		// □□□
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_20;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_20;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■■■★☆★■■■
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■

		// 위 왼쪽 대각선 오른쪽 대각선 아래 뚫려있음
		// □□□
		// □□□
		// ■□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_14;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_14;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_UU):
		// 　　　■■■
		// 　　　■□■
		// 　　　■■■
		// ■■■★☆★■■■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 아래 왼쪽 대각선 오른쪽 대각선 위 뚫려있음
		// ■□■
		// □□□
		// □□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_13;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_13;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_DL + df_DIRECTION_DD + df_DIRECTION_LL + df_DIRECTION_RR):
		// ■■■■■■
		// ■□□□□■
		// ■□■■■■
		// ■□■☆☆★■■■　　　
		// ■□■☆☆☆□□■
		// ■□■☆☆★■■■
		// ■□■■■■
		// ■□□□□■
		// ■■■■■■

		// 위 왼쪽 대각선 아래 왼쪽 대각선 오른쪽 뚫려있음
		// □□■
		// □□□
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_16;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_16;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_DR + df_DIRECTION_DD + df_DIRECTION_RR + df_DIRECTION_LL):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■□■
		// ■■■★☆☆■□■　　　
		// ■□■☆☆☆■□■
		// ■■■★☆☆■□■
		// 　　　■■■■□■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 위 오른쪽 대각선 아래 오른쪽 대각선 왼쪽 뚫려있음
		// ■□□
		// □□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_15;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_15;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DR):
		// ■■■■■■
		// ■□□□□■
		// ■□■■■■
		// ■□■☆☆★■■■
		// ■□■☆☆☆■□■
		// ■■■★☆☆■□■
		// 　　　■■■■□■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 위 왼쪽 대각선 아래 오른쪽 대각선 뚫려있음
		// □□■
		// □□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_18;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_18;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_RR + df_DIRECTION_LL + df_DIRECTION_DD + df_DIRECTION_DL):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■□■
		// ■■■★☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆★■■■
		// ■□■■■■
		// ■□□□□■
		// ■■■■■■

		// 위 오른쪽 대각선 아래 왼쪽 대각선 뚫려있음
		// ■□□
		// □□□
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_17;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_17;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DL):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆★■■■
		// ■□■■■■
		// ■□□□□■
		// ■■■■■■

		// 아래 오른쪽 대각선 빼고 다 뚫려있음
		// □□□
		// □□□
		// □□■

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_03;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_03;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DR):
		// ■■■■■■■■■
		// ■□□□□□□□■
		// ■□■■■■■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■■■★☆☆■□■
		// 　　　■■■■□■
		// 　　　■□□□□■
		// 　　　■■■■■■

		// 아래 왼쪽 대각선 빼고 다 뚫려있음
		// □□□
		// □□□
		// ■□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_04;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_04;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UR + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
		// 　　　■■■■■■
		// 　　　■□□□□■
		// 　　　■■■■□■
		// ■■■★☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 위 왼쪽 대각선 빼고 다 뚫려있음
		// ■□□
		// □□□
		// □□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_01;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_01;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	case (df_DIRECTION_UU + df_DIRECTION_UL + df_DIRECTION_LL + df_DIRECTION_RR + df_DIRECTION_DD + df_DIRECTION_DL + df_DIRECTION_DR):
		// ■■■■■■
		// ■□□□□■
		// ■□■■■■
		// ■□■☆☆★■■■
		// ■□■☆☆☆■□■
		// ■□■☆☆☆■□■
		// ■□■■■■■□■
		// ■□□□□□□□■
		// ■■■■■■■■■

		// 위 오른쪽 대각선 빼고 다 뚫려있음
		// □□■
		// □□□
		// □□□

		//--------------------------------------
		// 타일 변경
		//--------------------------------------
		iCnt = iTileX + (iTileY * m_iTileWidth);
		m_stpTileMap[iCnt].iTileState = df_TILE_STATE_BORDER;
		switch (iSelectedTileType)
		{
		case df_TILE_TYPE_LIBRARY:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_BORDER_02;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_LIBRARY;
			break;
		case df_TILE_TYPE_CASTLE:
			m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_BORDER_02;
			m_stpTileMap[iCnt].iTileType = df_TILE_TYPE_CASTLE;
			break;
		default:
			// 에러
			throw;
		}

		break;

	default:
		// 에러
		throw;
	}
	

}

void ObjectStageMap::ModifyAroundTile(int iTileX, int iTileY)
{
	int iCnt;
	int iSelectedTileState;			// 선택된 타일 상태
	int iSelectedTileType;			// 선택된 타일 종류
	int iBorderTileType;			// 천장의 타입 (검사하는 곳이 벽일때 천장의 타입으로 맞추기 위한 변수임)
	st_TileData *stpTileMap;		// 현제 타일 정보 포인터
	BYTE byTileDirection = 0;		// 타일 방향

	// 맵 바깥을 나가는가?
	if (iTileX < 0 || iTileY < 0 || iTileX >= m_iTileWidth || iTileY >= m_iTileHeight) {
		return;
	}


	iCnt = iTileX + (iTileY * m_iTileWidth);
	stpTileMap = &m_stpTileMap[iCnt];
	iSelectedTileState = stpTileMap->iTileState;
	iSelectedTileType = stpTileMap->iTileType;


	//--------------------------------------
	// 무슨 타일인가
	//--------------------------------------
	switch (iSelectedTileState)
	{
	case df_TILE_STATE_FLOOR:
		//--------------------------------------
		// 바닥
		// 변화 : 위에 천장이면 벽 타일로 바꿈
		//        아래쪽이 맵의 끝이면 천장으로 바꿈
		//--------------------------------------

		//--------------------------------------
		// 위쪽 검사
		//--------------------------------------
		// 위쪽이 맵의 끝이 아닌가?
		if (iTileY - 1 >= 0) {
			// 위쪽이 천장인가?
			iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {

				// 천장 타입 저장
				iBorderTileType = m_stpTileMap[iCnt].iTileType;

				// 아래쪽이 맽의 끝이 아닌가?
				if (iTileY + 1 < m_iTileHeight) {
					// 아래쪽이 천장이 아닌가?
					iCnt = iTileX + ((iTileY + 1) * m_iTileWidth);
					if (m_stpTileMap[iCnt].iTileState != df_TILE_STATE_BORDER) {
						// ■
						// ★
						// ?

						//--------------------------------------
						// 타일 변경
						// 바닥 타일을 -> 상단 벽
						//--------------------------------------

						//--------------------------------------
						// 천장 타입으로 맞춰줌
						//--------------------------------------
						stpTileMap->iTileType = iBorderTileType;
						stpTileMap->iTileState = df_TILE_STATE_WALL_TOP;
						switch (iBorderTileType)
						{
						case df_TILE_TYPE_LIBRARY:
							stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_TOP;
							break;
						case df_TILE_TYPE_CASTLE:
							stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_TOP;
							break;
						default:
							break;
						}

						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX, iTileY);
					}
					// 아래쪽이 천장임
					else {
						//--------------------------------------
						// 타일 변경
						// 바닥 타일을 -> 천장
						//--------------------------------------
						ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitService(iTileX, iTileY);
					}
				}
				// 아래쪽이 맵의 끝
				else {
					//--------------------------------------
					// 타일 변경
					// 바닥 타일을 -> 천장
					//--------------------------------------
					ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

					//----------------------------------------------------------
					// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
					// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
					//----------------------------------------------------------
					DeleteUnitService(iTileX, iTileY);
				}
			}
			// 위쪽이 상단 벽인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_TOP) {
				// ▥
				// ★

				// 천장 타입 저장
				iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
				iBorderTileType = m_stpTileMap[iCnt].iTileType;

				//--------------------------------------
				// 타일 변경
				// 바닥 타일을 -> 하단 벽
				//--------------------------------------

				//--------------------------------------
				// 천장 타입으로 맞춰줌
				//--------------------------------------
				stpTileMap->iTileState = df_TILE_STATE_WALL_BOTTOM;
				stpTileMap->iTileType = iBorderTileType;
				switch (iBorderTileType)
				{
				case df_TILE_TYPE_LIBRARY:
					stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
					break;
				case df_TILE_TYPE_CASTLE:
					stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
					break;
				default:
					break;
				}

				//----------------------------------------------------------
				// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
				// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
				//----------------------------------------------------------
				DeleteUnitService(iTileX, iTileY);
			}
		}

		return;

	case df_TILE_STATE_BORDER:
		//--------------------------------------
		// 천장
		// 변화 : 주변에 따라 모양 바뀌어야 함
		//--------------------------------------
		ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

		break;

	case df_TILE_STATE_WALL_TOP:
		//--------------------------------------
		// 상단 벽
		// 변화 : 하단이 천장이면 천장으로 바뀌어야 함
		//        하단이 바닥이면 위에 2칸 봐야 함
		//        위쪽이 당연히 천장이어야 함
		//--------------------------------------


		//--------------------------------------
		// 위쪽 검사
		//--------------------------------------
		// 위쪽이 맵의 끝이 아닌가?
		if (iTileY - 1 >= 0) {
			// 위쪽이 천장인가?
			iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
				// ■
				// ★

				// 천장 타입 저장
				iBorderTileType = m_stpTileMap[iCnt].iTileType;

				//--------------------------------------
				// 아래쪽 검사
				//--------------------------------------
				// 아래쪽이 맽의 끝이 아닌가?
				if (iTileY + 1 < m_iTileHeight) {
					// 아래쪽이 천장인가?
					iCnt = iTileX + ((iTileY + 1) * m_iTileWidth);
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						// ■
						// ★
						// ■

						//--------------------------------------
						// 타일 변경
						// 상단 벽 타일을 -> 천장
						//--------------------------------------
						ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

						// 현제 타일 위에 유닛이 있나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							//----------------------------------------------------------
							// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
							// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
							//----------------------------------------------------------
							DeleteUnitProcess(iTileX, iTileY);
						}
					}
					// 아래쪽이 바닥인가?
					else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
						// ■
						// ★
						// □

						// 2칸 위쪽이 맵의 끝인가?
						if (iTileY - 2 < 0) {
							// ▣
							// ■
							// ★
							// □

							//--------------------------------------
							// 타일 변경
							// 상단 벽 타일을 -> 바닥
							//--------------------------------------
							stpTileMap->iTileState = df_TILE_STATE_FLOOR;
							switch (iSelectedTileType)
							{
							case df_TILE_TYPE_LIBRARY:
								stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
								stpTileMap->iTileType = df_TILE_TYPE_LIBRARY;
								break;
							case df_TILE_TYPE_CASTLE:
								//--------------------------------------
								// 바닥 타일이 두종류임
								//--------------------------------------
								if (rand() % 2) {
									stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
								}
								else {
									stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
								}
								stpTileMap->iTileType = df_TILE_TYPE_CASTLE;
								break;
							default:
								break;
							}

							// 현제 타일 위에 유닛이 있나?
							if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
								//----------------------------------------------------------
								// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
								// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
								//----------------------------------------------------------
								DeleteUnitProcess(iTileX, iTileY);
							}
						}
						// 2칸 위쪽이 맵이 끝이 아니라면
						else {
							// 2칸 위쪽이 천장인가?	
							iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
							if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
								// ■
								// ■
								// ★
								// □

								//--------------------------------------
								// 타일 변경
								// 상단 벽 타일을 -> 하단 벽
								//--------------------------------------
								stpTileMap->iTileState = df_TILE_STATE_WALL_BOTTOM;
								switch (iSelectedTileType)
								{
								case df_TILE_TYPE_LIBRARY:
									stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
									stpTileMap->iTileType = df_TILE_TYPE_LIBRARY;
									break;
								case df_TILE_TYPE_CASTLE:
									stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
									stpTileMap->iTileType = df_TILE_TYPE_CASTLE;
									break;
								default:
									break;
								}

								// 현제 타일 위에 유닛이 있나?
								if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX, iTileY);
								}
							}
							// 2칸 위쪽이 바닥인가?	
							else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
								// □
								// ■
								// ★
								// □

								//--------------------------------------
								// 타일 변경
								// 상단 벽 타일을 -> 바닥
								//--------------------------------------
								stpTileMap->iTileState = df_TILE_STATE_FLOOR;
								switch (iSelectedTileType)
								{
								case df_TILE_TYPE_LIBRARY:
									stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
									stpTileMap->iTileType = df_TILE_TYPE_LIBRARY;
									break;
								case df_TILE_TYPE_CASTLE:
									//--------------------------------------
									// 바닥 타일이 두종류임
									//--------------------------------------
									if (rand() % 2) {
										stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
									}
									else {
										stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
									}
									stpTileMap->iTileType = df_TILE_TYPE_CASTLE;
									break;
								default:
									break;
								}

								// 현제 타일 위에 유닛이 있나?
								if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
									//----------------------------------------------------------
									// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
									// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
									//----------------------------------------------------------
									DeleteUnitProcess(iTileX, iTileY);
								}
							}
						}
					}
					// 아래쪽이 하단 벽인가?
					else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_BOTTOM) {
						// ■
						// ★
						// ▥

						//--------------------------------------
						// 천장 타입으로 맞춰줌
						//--------------------------------------
						stpTileMap->iTileType = iBorderTileType;
						//m_stpTileMap[iCnt].iTileType = iBorderTileType;
						switch (iBorderTileType)
						{
						case df_TILE_TYPE_LIBRARY:
							stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_TOP;
							//m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
							break;
						case df_TILE_TYPE_CASTLE:
							stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_TOP;
							//m_stpTileMap[iCnt].iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
							break;
						default:
							break;
						}
					}
				}
			}
			// 위쪽이 바닥인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
				// □
				// ★

				//--------------------------------------
				// 타일 변경
				// 상단 벽 타일을 -> 바닥
				//--------------------------------------
				stpTileMap->iTileState = df_TILE_STATE_FLOOR;
				switch (iSelectedTileType)
				{
				case df_TILE_TYPE_LIBRARY:
					stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
					stpTileMap->iTileType = df_TILE_TYPE_LIBRARY;
					break;
				case df_TILE_TYPE_CASTLE:
					//--------------------------------------
					// 바닥 타일이 두종류임
					//--------------------------------------
					if (rand() % 2) {
						stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
					}
					else {
						stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
					}
					stpTileMap->iTileType = df_TILE_TYPE_CASTLE;
					break;
				default:
					break;
				}

				// 현제 타일 위에 유닛이 있나?
				if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
					//----------------------------------------------------------
					// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
					// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
					//----------------------------------------------------------
					DeleteUnitProcess(iTileX, iTileY);
				}
			}
		}


		

		break;

	case df_TILE_STATE_WALL_BOTTOM:
		//--------------------------------------
		// 하단 벽
		// 변화 : 상단이 천장이면 천장이나 상단 벽으로 바뀌어야 함
		//--------------------------------------

		//--------------------------------------
		// 위쪽 검사
		//--------------------------------------
		// 위쪽이 맵의 끝이 아닌가?
		if (iTileY - 1 >= 0) {
			// 위쪽이 천장인가?
			iCnt = iTileX + ((iTileY - 1) * m_iTileWidth);
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
				// ■
				// ★

				// 천장 타입 저장
				iBorderTileType = m_stpTileMap[iCnt].iTileType;

				//--------------------------------------
				// 아래쪽 검사
				//--------------------------------------
				// 아래쪽이 맽의 끝이 아닌가?
				if (iTileY + 1 < m_iTileHeight) {
					// 아래쪽이 천장인가?
					iCnt = iTileX + ((iTileY + 1) * m_iTileWidth);
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						//--------------------------------------
						// 타일 변경
						// 하단 벽을 -> 천장
						//--------------------------------------
						ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

						// 현제 타일 위에 유닛이 있나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							//----------------------------------------------------------
							// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
							// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
							//----------------------------------------------------------
							DeleteUnitProcess(iTileX, iTileY);
						}
					}
					// 아래쪽이 천장이 아닌 상태에서
					else {
						//--------------------------------------
						// 타일 변경
						// 하단 벽을 -> 상단 벽
						//--------------------------------------
						stpTileMap->iTileState = df_TILE_STATE_WALL_TOP;
						stpTileMap->iTileType = iBorderTileType;
						switch (iBorderTileType)
						{
						case df_TILE_TYPE_LIBRARY:
							stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_TOP;
							break;
						case df_TILE_TYPE_CASTLE:
							stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_TOP;
							break;
						default:
							break;
						}

						// 현제 타일 위에 유닛이 있나?
						if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
							//----------------------------------------------------------
							// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
							// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
							//----------------------------------------------------------
							DeleteUnitProcess(iTileX, iTileY);
						}
					}
				}
				// 아래쪽이 맵의 끝임
				else {
					//--------------------------------------
					// 타일 변경
					// 하단 벽을 -> 천장
					//--------------------------------------
					ModifyBorderTileProcess(iTileX, iTileY, iSelectedTileType);

					// 현제 타일 위에 유닛이 있나?
					if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
						//----------------------------------------------------------
						// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
						// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
						//----------------------------------------------------------
						DeleteUnitProcess(iTileX, iTileY);
					}
				}
			}
			// 위쪽이 바닥인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
				// □
				// ★

				//--------------------------------------
				// 타일 변경
				// 하단 벽을 -> 바닥
				//--------------------------------------
				stpTileMap->iTileState = df_TILE_STATE_FLOOR;
				switch (iSelectedTileType)
				{
				case df_TILE_TYPE_LIBRARY:
					stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_FLOOR;
					stpTileMap->iTileType = df_TILE_TYPE_LIBRARY;
					break;
				case df_TILE_TYPE_CASTLE:
					//--------------------------------------
					// 바닥 타일이 두종류임
					//--------------------------------------
					if (rand() % 2) {
						stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_01;
					}
					else {
						stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_FLOOR_02;
					}
					stpTileMap->iTileType = df_TILE_TYPE_CASTLE;
					break;
				default:
					break;
				}

				// 현제 타일 위에 유닛이 있나?
				if (stpTileMap->iUnitType != df_UNIT_TYPE_EMPT) {
					//----------------------------------------------------------
					// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
					// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
					//----------------------------------------------------------
					DeleteUnitProcess(iTileX, iTileY);
				}
			}
			// 위쪽이 상단 벽인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_TOP) {
				// ▥
				// ★

				// 천장 타입 저장
				iCnt = iTileX + ((iTileY - 2) * m_iTileWidth);
				iBorderTileType = m_stpTileMap[iCnt].iTileType;

				//--------------------------------------
				// 천장 타입으로 맞춰줌
				//--------------------------------------
				stpTileMap->iTileState = df_TILE_STATE_WALL_BOTTOM;
				stpTileMap->iTileType = iBorderTileType;
				switch (iBorderTileType)
				{
				case df_TILE_TYPE_LIBRARY:
					stpTileMap->iTileSprite = df_SPRITE_TILE_LIBRARY_WALL_BOTTOM;
					break;
				case df_TILE_TYPE_CASTLE:
					stpTileMap->iTileSprite = df_SPRITE_TILE_CASTLE_WALL_BOTTOM;
					break;
				default:
					break;
				}
			}
		}

		break;

	default:
		break;
	}



}

// 유닛 만들기
// 위치 좌표, 선택된 상태, 선택된 타입, 선택된 스프라이트 증가 값
void ObjectStageMap::CreateUnit(int iTileX, int iTileY)
{
	int iSelectedState = g_iSelectedState;						// 선택된 상태
	int iSelectedType = g_iSelectedType;						// 선택된 타입
	int iIncreaseVlaue = g_iSelectedSpriteIncreaseVlaue;		// 선택된 증가 값
	int iCnt;
	ObjectUnit *newUnit;
	st_SpriteInfo stSpriteInfo;
	CList<Object *>::iterator iter;
	CList<Object *>::iterator iter_end;
	Object* ObjectTemp;
	int iWidth;
	int iHeight;
	int iCenterPointX;
	int iCenterPointY;
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
	int iPosX;
	int iPosY;


	switch (iSelectedState)
	{
	case df_UNIT_STATE_FLOOR:
		//--------------------------------------
		// 바닥에 붙어있는 오브젝트
		//--------------------------------------

		switch (iSelectedType) {
		case df_UNIT_TYPE_ENEMY_BULLET_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BULLET_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BULLET_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_BANDANA_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BANDANA_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BANDANA_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_RED_SHOTGUN_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_RED_SHOTGUN_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_BLUE_SHOTGUN_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_SHOTGUN_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_MUMMY_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_MUMMY_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_MUMMY_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_SHOTGRUB_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_SHOTGRUB_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_GHOST_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_GHOST_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_GHOST_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_FLY_KIN:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_FLY_KIN;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_FLY_KIN;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_BIRD:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BIRD;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BIRD;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_CUBE:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_CUBE;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_CUBE;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_GREEN_BOOK:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_GREEN_BOOK;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_GREEN_BOOK;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_BLUE_BOOK:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_BOOK;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_BOOK;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_YELLOW_MAGIC:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_YELLOW_MAGIC;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_BLUE_MAGIC:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_BLUE_MAGIC;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_RED_MAGIC:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_RED_MAGIC;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_RED_MAGIC;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_WISP:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_WISP;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_WISP;
			stSpriteInfo.iDelay = 5;
			break;
		case df_UNIT_TYPE_ENEMY_MUSHROOM:
			stSpriteInfo.iStartSprite = df_SPRITE_ENEMY_MUSHROOM;
			stSpriteInfo.iEndSprite = df_SPRITE_ENEMY_MUSHROOM;
			stSpriteInfo.iDelay = 5;
			break;
		default:
			return;
		}


		iWidth = g_cSprite->m_stpSprite[iSelectedType].iWidth * 4;
		iHeight = g_cSprite->m_stpSprite[iSelectedType].iHeight * 4;
		iCenterPointX = g_cSprite->m_stpSprite[iSelectedType].iCenterPointX * 4;
		iCenterPointY = g_cSprite->m_stpSprite[iSelectedType].iCenterPointY * 4;

		// 충돌 범위
		iLeft = g_iMapMouseX - iCenterPointX;
		iTop = g_iMapMouseY - iCenterPointY;
		iRight = iLeft + iWidth;
		iBottom = iTop + iHeight;

		// 검사
		iter = g_ObjectList->begin();
		iter_end = g_ObjectList->end();
		while (iter != iter_end) {
			ObjectTemp = *iter;

			if (ObjectTemp->m_iType != df_TYPE_OBJECT) {
				++iter;
				continue;
			}

			if (ObjectTemp->m_iLeft < iRight && ObjectTemp->m_iRight > iLeft &&
				ObjectTemp->m_iTop < iBottom && ObjectTemp->m_iBottom > iTop) {

				return;
			}
			++iter;
		}

		// 이 유닛은 타일 기준으로 생성되지 않기때문에 맵 마우스 좌표를 가져옵니다
		iPosX = g_iMapMouseX;
		iPosY = g_iMapMouseY;

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iLeft / df_DEFAULT_TILE_SIZE;
		iTileY = iTop / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iRight / df_DEFAULT_TILE_SIZE;
		iTileY = iTop / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iLeft / df_DEFAULT_TILE_SIZE;
		iTileY = iBottom / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return;
		}

		// 타일 검사 (바닥에서만 생성할 수 있다)
		iTileX = iRight / df_DEFAULT_TILE_SIZE;
		iTileY = iBottom / df_DEFAULT_TILE_SIZE;
		// 바닥이 아닌가?
		iCnt = iTileX + (iTileY * m_iTileWidth);
		if (m_stpTileMap[iCnt].iTileState != df_UNIT_STATE_FLOOR) {
			return;
		}

		// 생성
		newUnit = new UnitEnemy(stSpriteInfo, iSelectedType, iPosX, iPosY, iIncreaseVlaue);
		g_ObjectList->push_back(newUnit);

		//-------------------------------------------------------
		// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
		// 파란색 -> 빨간색
		//-------------------------------------------------------
		g_MousePointer->m_bBlueOption = false;
		g_MousePointer->m_bRedOption = true;

		return;

	case df_UNIT_STATE_WALL:
		//--------------------------------------
		// 벽에 붙어있는 오브젝트
		//--------------------------------------
		switch (iSelectedType) {
		case df_UNIT_TYPE_LANTERN :
			//--------------------------------------
			// 랜턴
			// 상단벽에 붙음
			// 왼쪽 오른쪽 벽에도 붙음
			//--------------------------------------

			
			iCnt = iTileX + (iTileY * m_iTileWidth);
			// 이미 설치됨
			if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
				break;
			}

			// 상단 벽인가?
			if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_WALL_TOP) {
				stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_CENTER_01;
				stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_CENTER_03;
				stSpriteInfo.iDelay = 5;
				newUnit = new UnitLantern(stSpriteInfo, df_UNIT_TYPE_LANTERN, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
				g_ObjectList->push_back(newUnit);

				// 현제 타일에 오브젝트 설치 값 저장
				m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_LANTERN;
				m_stpTileMap[iCnt].UnitPtr = newUnit;

				//-------------------------------------------------------
				// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
				// 파란색 -> 빨간색
				//-------------------------------------------------------
				g_MousePointer->m_bBlueOption = false;
				g_MousePointer->m_bRedOption = true;
			}
			// 바닥인가?
			else if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_FLOOR) {
				//--------------------------------------
				// 왼쪽과 오른쪽을 검사 해야 함
				//--------------------------------------

				// 왼쪽에 가까움
				if (g_iMapMouseX % df_DEFAULT_TILE_SIZE < df_DEFAULT_TILE_SIZE / 2) {
					// 왼쪽 검사
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_LEFT_03;
						stSpriteInfo.iDelay = 5;
						newUnit = new UnitLantern(stSpriteInfo, df_UNIT_TYPE_LANTERN_L, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
						g_ObjectList->push_back(newUnit);

						// 현제 타일에 오브젝트 설치 값 저장
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_LANTERN;
						m_stpTileMap[iCnt].UnitPtr = newUnit;

						//-------------------------------------------------------
						// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
						// 파란색 -> 빨간색
						//-------------------------------------------------------
						g_MousePointer->m_bBlueOption = false;
						g_MousePointer->m_bRedOption = true;

						break;
					}

					// 오른쪽 검사
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_03;
						stSpriteInfo.iDelay = 5;
						newUnit = new UnitLantern(stSpriteInfo, df_UNIT_TYPE_LANTERN_R, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
						g_ObjectList->push_back(newUnit);

						// 현제 타일에 오브젝트 설치 값 저장
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_LANTERN;
						m_stpTileMap[iCnt].UnitPtr = newUnit;

						//-------------------------------------------------------
						// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
						// 파란색 -> 빨간색
						//-------------------------------------------------------
						g_MousePointer->m_bBlueOption = false;
						g_MousePointer->m_bRedOption = true;

						break;
					}
				}
				// 오른쪽에 가까움
				else {
					// 오른쪽 검사
					iCnt = (iTileX + 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_01;
						stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_RIGHT_03;
						stSpriteInfo.iDelay = 5;
						newUnit = new UnitLantern(stSpriteInfo, df_UNIT_TYPE_LANTERN_R, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
						g_ObjectList->push_back(newUnit);

						// 현제 타일에 오브젝트 설치 값 저장
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_LANTERN;
						m_stpTileMap[iCnt].UnitPtr = newUnit;

						//-------------------------------------------------------
						// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
						// 파란색 -> 빨간색
						//-------------------------------------------------------
						g_MousePointer->m_bBlueOption = false;
						g_MousePointer->m_bRedOption = true;

						break;
					}

					// 왼쪽 검사
					iCnt = (iTileX - 1) + (iTileY * m_iTileWidth);
					// 천장 인가?
					if (m_stpTileMap[iCnt].iTileState == df_TILE_STATE_BORDER) {
						stSpriteInfo.iStartSprite = df_SPRITE_OBJECT_LANTERN_LEFT_01;
						stSpriteInfo.iEndSprite = df_SPRITE_OBJECT_LANTERN_LEFT_03;
						stSpriteInfo.iDelay = 5;
						newUnit = new UnitLantern(stSpriteInfo, df_UNIT_TYPE_LANTERN_L, iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE, iIncreaseVlaue);
						g_ObjectList->push_back(newUnit);

						// 현제 타일에 오브젝트 설치 값 저장
						iCnt = iTileX + (iTileY * m_iTileWidth);
						m_stpTileMap[iCnt].iUnitType = df_UNIT_TYPE_LANTERN;
						m_stpTileMap[iCnt].UnitPtr = newUnit;

						//-------------------------------------------------------
						// 현제 위치에 만들었으니까 마우스 포인터 색깔 바꿔야함
						// 파란색 -> 빨간색
						//-------------------------------------------------------
						g_MousePointer->m_bBlueOption = false;
						g_MousePointer->m_bRedOption = true;

						break;
					}
				}

				
			}
			



			break;
		default:
			break;
		}

		break;

	default:
		break;
	}
}

// 유닛 삭제 서비스
void ObjectStageMap::DeleteUnitService(int iTileX, int iTileY)
{
	int iCnt;
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
	CList<Object *>::iterator iter;
	CList<Object *>::iterator iter_end;
	Object* ObjectTemp;


	//-----------------------------------------------------
	// 타일에 결속되지않는 유닛을 검사해야 한다.
	// 적같이 바닥 위에 널부러져있는 것들
	//-----------------------------------------------------
	iLeft = iTileX * df_DEFAULT_TILE_SIZE;
	iTop = iTileY * df_DEFAULT_TILE_SIZE;
	iRight = (iTileX + 1) * df_DEFAULT_TILE_SIZE;
	iBottom = (iTileY + 1) * df_DEFAULT_TILE_SIZE;
	iter = g_ObjectList->begin();
	iter_end = g_ObjectList->end();
	while (iter != iter_end) {
		ObjectTemp = *iter;

		if (ObjectTemp->m_iType != df_TYPE_OBJECT) {
			++iter;
			continue;
		}

		if (ObjectTemp->m_iLeft < iRight && ObjectTemp->m_iRight > iLeft &&
			ObjectTemp->m_iTop < iBottom && ObjectTemp->m_iBottom > iTop) {
			// 유닛 제거
			((ObjectUnit *)ObjectTemp)->m_bEraseFlag = true;
		}
		++iter;
	}


	iCnt = iTileX + (iTileY * m_iTileWidth);
	// 현제 타일 위에 유닛이 있나?
	if (m_stpTileMap[iCnt].iUnitType != df_UNIT_TYPE_EMPT) {
		//----------------------------------------------------------
		// 타일이 교체되면서 이 위에있는 타일에 결속된 유닛이 삭제됨
		// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
		//----------------------------------------------------------
		DeleteUnitProcess(iTileX, iTileY);
	}
}


//----------------------------------------------------------
// DeleteUnitProcess
// 유닛 삭제
// 타일이 교체되면서 이 위에있는 유닛이 삭제됨
// 유닛 덩치가 크면 그 만큼의 타일에가서 정보를 바꿔줘야 함
//----------------------------------------------------------
void ObjectStageMap::DeleteUnitProcess(int iTileX, int iTileY)
{
	int iCnt;
	int iX;
	int iY;
	int iLeft;
	int iTop;
	int iRight;
	int iBottom;
	st_TileData * stpTileMap;		// 타일 정보 포인터
	ObjectUnit * deleteUnitPtr;		// 삭제할 유닛

	// 예외처리
	if (iTileX < 0 || iTileY < 0 || iTileX > m_iTileWidth - 1 || iTileY > m_iTileHeight - 1) {
		throw;
	}



	iCnt = iTileX + (iTileY * m_iTileWidth);
	stpTileMap = &m_stpTileMap[iCnt];

	// 삭제 플래그
	stpTileMap->UnitPtr->m_bEraseFlag = true;
	// 삭제할 유닛
	deleteUnitPtr = stpTileMap->UnitPtr;

	// 탐색 범위
	iLeft = deleteUnitPtr->m_iLeft;
	iTop = deleteUnitPtr->m_iTop;
	iRight = deleteUnitPtr->m_iRight;
	iBottom = deleteUnitPtr->m_iBottom;

	//------------------------------------
	// 메모리 밖을 나가면 안된다
	//------------------------------------
	if (iLeft < 0) {
		iLeft = 0;
	}
	if (iTop < 0) {
		iTop = 0;
	}
	if (iRight > ((m_iTileWidth - 1) * df_DEFAULT_TILE_SIZE)) {
		iRight = (m_iTileWidth - 1) * df_DEFAULT_TILE_SIZE;
	}
	if (iBottom > ((m_iTileHeight - 1) * df_DEFAULT_TILE_SIZE)) {
		iBottom = (m_iTileHeight - 1) * df_DEFAULT_TILE_SIZE;
	}


	// 탐색
	iY = iTop;
	while (iY < iBottom) {
		iX = iLeft;
		while (iX < iRight) {
			iCnt = (iX / df_DEFAULT_TILE_SIZE) + ((iY / df_DEFAULT_TILE_SIZE) * m_iTileWidth);
			stpTileMap = &m_stpTileMap[iCnt];
			// 현제 타일 빈 타일로
			stpTileMap->iUnitType = df_UNIT_TYPE_EMPT;
			stpTileMap->UnitPtr = NULL;

			iX = iX + df_DEFAULT_TILE_SIZE;
		}
		iY = iY + df_DEFAULT_TILE_SIZE;
	}
}

//----------------------------------------------------------
// CreateLocation
// 로케이션 만들기
// 로케이션은 unit으로 생성됨
// 단, 직사각형으로 타일 크기에 맞춰서 생성됨
// 그 외에 제한사항 없음
//----------------------------------------------------------
void ObjectStageMap::CreateLocation(int iTileX, int iTileY)
{
	int iSelectedType = g_iSelectedType;						// 선택된 타입
	int iCnt;
	ObjectUnit *newUnit;
	st_SpriteInfo stSpriteInfo;


	switch (iSelectedType)
	{
	case df_UNIT_TYPE_LOCATION_RECTANGLE:
		newUnit = new UnitLocation(iTileX * df_DEFAULT_TILE_SIZE, iTileY * df_DEFAULT_TILE_SIZE);
		g_ObjectList->push_back(newUnit);
		// 선택된 유닛
		g_SelectedObjectUnit = newUnit;
		// 마우스 상태
		g_iMouseState = df_MOUSE_STATE_LOCATION_DRAG;

		break;
	default:
		break;
	}
}
