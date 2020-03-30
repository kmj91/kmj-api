#include "stdafx.h"
#include "ObjectStageMap.h"
#include "ObjectUnit.h"


ObjectStageMap::ObjectStageMap(int iMapWidth, int iMapHeight)
{
	// 타입
	m_iType = df_TYPE_STAGE_MAP;
	// 위치
	m_dX = 0;
	m_dY = 0;
	// 스프라이트 번호
	m_iSpriteIndex = df_SPRITE_BACKGROUND_2;
	// 오브젝트 범위
	m_dLeft = 0;
	m_dTop = 0;
	m_dRight = m_dLeft + g_iDestWidth;
	m_dBottom = m_dTop + g_iDestHeight;
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
	g_cSprite->DrawSprite(m_iSpriteIndex, m_dX, m_dY, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, false);
}

bool ObjectStageMap::Click()
{
	CList<Object*>::iterator iter;
	CList<Object*>::iterator iter_end;
	Object *newObject;
	Object *ObjectTemp;
	int iMouseX;
	int iMouseY;
	int iTilePosX;
	int iTilePosY;
	int iPropertiesX;
	int iPropertiesY;

	//--------------------------------------
	// 화면 기준으로 움직이는 UI
	// 카메라 좌표를 빼준다
	//--------------------------------------
	iMouseX = g_iMouseX - g_CameraRect.left;
	iMouseY = g_iMouseY - g_CameraRect.top;

	if (m_dLeft < iMouseX && iMouseX < m_dRight &&
		m_dTop < iMouseY && iMouseY < m_dBottom) {

		switch (g_MouseState)
		{
		case MOUSE_STATE_EMPT:
			break;
		case MOUSE_STATE_TILE_MAKE_4:
			iTilePosX = g_iMouseX / df_TILE_WIDTH;
			iTilePosY = g_iMouseY / df_TILE_HEIGHT;

			//--------------------------------------
			// 홀수짝수 구분
			// 홀수 번째 위치가 다름
			//--------------------------------------
			if (iTilePosY % 2) {
				if (iTilePosX % 2) {
					iTilePosY = iTilePosY + 1;
				}
				else {

				}
			}
			else {
				if (iTilePosX % 2) {

				}
				else {
					iTilePosY = iTilePosY + 1;
				}
			}

			iTilePosX = iTilePosX * df_TILE_WIDTH;
			iTilePosY = iTilePosY * df_TILE_HEIGHT;

			iPropertiesX = iTilePosX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = iTilePosY / df_TILE_PROPERTIES_SIZE;

			SetProperties(iPropertiesX, iPropertiesY);

			newObject = new ObjectUnit(df_TYPE_TILE, g_iMakeTileIndex, iTilePosX, iTilePosY);
			g_ObjectList->push_back(newObject);

			break;
		case MOUSE_STATE_TILE_MAKE_1:
			iTilePosX = g_iMouseX / df_TILE_SMALL_WIDTH;
			iTilePosY = g_iMouseY / df_TILE_SMALL_HEIGHT;

			//--------------------------------------
			// 홀수짝수 구분
			// 홀수 번째 위치가 다름
			//--------------------------------------
			if (iTilePosY % 2) {
				if (iTilePosX % 2) {
					iTilePosY = iTilePosY + 1;
				}
				else {

				}
			}
			else {
				if (iTilePosX % 2) {

				}
				else {
					iTilePosY = iTilePosY + 1;
				}
			}

			iTilePosX = iTilePosX * df_TILE_SMALL_WIDTH;
			iTilePosY = iTilePosY * df_TILE_SMALL_HEIGHT;

			iPropertiesX = iTilePosX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = iTilePosY / df_TILE_PROPERTIES_SIZE;

			SetProperties(iPropertiesX, iPropertiesY);

			newObject = new ObjectUnit(df_TYPE_TILE, g_iMakeTileIndex, iTilePosX, iTilePosY);
			g_ObjectList->push_back(newObject);

			break;
		case MOUSE_STATE_OBJECT_MAKE:
			// 스프라이트 중점좌표가 기준임 x, y
			iPropertiesX = g_iMouseX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = g_iMouseY / df_TILE_PROPERTIES_SIZE;

			if (CheckProperties(iPropertiesX, iPropertiesY)) {
				iTilePosX = iPropertiesX * df_TILE_PROPERTIES_SIZE;
				iTilePosY = iPropertiesY * df_TILE_PROPERTIES_SIZE;

				SetProperties(iPropertiesX, iPropertiesY);

				newObject = new ObjectUnit(df_TYPE_OBJECT, g_iMakeTileIndex, iTilePosX, iTilePosY);
				g_ObjectList->push_back(newObject);
			}

			break;
		case MOUSE_STATE_STAGE_MAKE:
			// 스프라이트 중점좌표가 기준임 x, y
			iPropertiesX = g_iMouseX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = g_iMouseY / df_TILE_PROPERTIES_SIZE;

			if (CheckProperties(iPropertiesX, iPropertiesY)) {
				iTilePosX = iPropertiesX * df_TILE_PROPERTIES_SIZE;
				iTilePosY = iPropertiesY * df_TILE_PROPERTIES_SIZE;

				SetProperties(iPropertiesX, iPropertiesY);

				newObject = new ObjectUnit(df_TYPE_STAGE, g_iMakeTileIndex, iTilePosX, iTilePosY);
				g_ObjectList->push_back(newObject);
			}

			break;
		case MOUSE_STATE_BROKEN_MAKE:
			// 스프라이트 중점좌표가 기준임 x, y
			iPropertiesX = g_iMouseX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = g_iMouseY / df_TILE_PROPERTIES_SIZE;

			if (CheckProperties(iPropertiesX, iPropertiesY)) {
				iTilePosX = iPropertiesX * df_TILE_PROPERTIES_SIZE;
				iTilePosY = iPropertiesY * df_TILE_PROPERTIES_SIZE;

				SetProperties(iPropertiesX, iPropertiesY);

				newObject = new ObjectUnit(df_TYPE_BROKEN, g_iMakeTileIndex, iTilePosX, iTilePosY);
				g_ObjectList->push_back(newObject);
			}

			break;
		case MOUSE_STATE_ENEMY_MAKE:
			// 스프라이트 중점좌표가 기준임 x, y
			iPropertiesX = g_iMouseX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = g_iMouseY / df_TILE_PROPERTIES_SIZE;

			if (CheckProperties(iPropertiesX, iPropertiesY)) {
				iTilePosX = iPropertiesX * df_TILE_PROPERTIES_SIZE;
				iTilePosY = iPropertiesY * df_TILE_PROPERTIES_SIZE;

				SetProperties(iPropertiesX, iPropertiesY);

				newObject = new ObjectUnit(df_TYPE_ENEMY, g_iMakeTileIndex, iTilePosX, iTilePosY);
				g_ObjectList->push_back(newObject);
			}

			break;
		case MOUSE_STATE_PLAYER_MAKE:
			// 스프라이트 중점좌표가 기준임 x, y
			iPropertiesX = g_iMouseX / df_TILE_PROPERTIES_SIZE;
			iPropertiesY = g_iMouseY / df_TILE_PROPERTIES_SIZE;

			if (CheckProperties(iPropertiesX, iPropertiesY)) {
				iTilePosX = iPropertiesX * df_TILE_PROPERTIES_SIZE;
				iTilePosY = iPropertiesY * df_TILE_PROPERTIES_SIZE;

				SetProperties(iPropertiesX, iPropertiesY);

				if (g_Player != NULL) {
					g_ObjectList->delete_Node(g_Player);
				}

				g_Player = new ObjectUnit(df_TYPE_PLAYER , g_iMakeTileIndex, iTilePosX, iTilePosY);
				g_ObjectList->push_back(g_Player);
			}
			
			break;
		case MOUSE_STATE_ERASE:
			// 지울 영역 탐색
			iter = g_ObjectList->rbegin();
			iter_end = g_ObjectList->head();
			while (iter != iter_end) {
				ObjectTemp = *iter;
				
				switch (ObjectTemp->m_iType) {
				case df_TYPE_STAGE:
				case df_TYPE_TILE:
				case df_TYPE_PLAYER:
				case df_TYPE_OBJECT:
				case df_TYPE_BROKEN:
				case df_TYPE_ENEMY:
					if (((ObjectUnit *)ObjectTemp)->Erase(g_iMouseX, g_iMouseY)) {
						// 속성도 지우기
						//--------------------------------------
						// 솔직히 오브젝트가 자신의 속성좌표 지우는거
						// 애바참치임
						// 그냥 다시 모든 오브젝트 돌면서
						// 속성 업데이트하는게 나음
						//--------------------------------------
						g_bUpdateProperties = true;
					}
					break;
				}

				--iter;
			}


			break;
		default:
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

void ObjectStageMap::CreateMap(int iMapWidth, int iMapHeight)
{
	// 가로
	m_iPropertiesWidth = iMapWidth / df_TILE_PROPERTIES_SIZE;
	if (iMapWidth % df_TILE_PROPERTIES_SIZE) {
		++m_iPropertiesWidth;
	}
	// 세로
	m_iPropertiesHeight = iMapHeight / df_TILE_PROPERTIES_SIZE;
	if (iMapHeight % df_TILE_PROPERTIES_SIZE) {
		++m_iPropertiesHeight;
	}
	// 크기
	m_iPropertiesSize = m_iPropertiesWidth * m_iPropertiesHeight;

	m_bypMapProperties = (BYTE*)malloc(m_iPropertiesSize);

	memset(m_bypMapProperties, 1, m_iPropertiesSize);
}

void ObjectStageMap::ReleaseMap()
{
	free(m_bypMapProperties);
}

void ObjectStageMap::SetProperties(int iPropertiesX, int iPropertiesY)
{
	int iCntX;
	int iCntY;
	int iArrayCntX;
	int iArrayCntY;
	int iPropertiesSizeX;			// 가로 길이
	int iPropertiesSizeY;			// 세로 길이
	int iArrayPropertiesIndex;
	int iMapPropertiesIndex;
	BYTE byCheckMakeProperties;

	//--------------------------------------
	// 타일 범위
	// 중점 좌표를 뺀 값이 시작 위치 좌표
	//--------------------------------------
	iPropertiesX = iPropertiesX - g_iPropertiesCenterPointX;
	iPropertiesY = iPropertiesY - g_iPropertiesCenterPointY;


	iPropertiesSizeX = g_iMakePropertiesSizeX;
	iPropertiesSizeY = g_iMakePropertiesSizeY;
	byCheckMakeProperties = g_byCheckMakeProperties;


	iArrayCntX = 0;
	iArrayCntY = 0;

	// 화면밖으로 빠져나가는지?
	if (iPropertiesX < 0) {
		iArrayCntX = -iPropertiesX;
		iPropertiesSizeX = iPropertiesSizeX + iPropertiesX;
		iPropertiesX = 0;
	}
	if (iPropertiesX + iPropertiesSizeX > m_iPropertiesWidth) {
		iPropertiesSizeX = iPropertiesSizeX - ((iPropertiesX + iPropertiesSizeX) - m_iPropertiesWidth);
	}
	if (iPropertiesY < 0) {
		iArrayCntY = -iPropertiesY;
		iPropertiesSizeY = iPropertiesSizeY + iPropertiesY;
		iPropertiesY = 0;
	}
	if (iPropertiesY + iPropertiesSizeY > m_iPropertiesHeight) {
		iPropertiesSizeY = iPropertiesSizeY - ((iPropertiesY + iPropertiesSizeY) - m_iPropertiesHeight);
	}


	iCntY = 0;
	while (iCntY < iPropertiesSizeY) {

		iCntX = 0;
		while (iCntX < iPropertiesSizeX) {

			iArrayPropertiesIndex = (iArrayCntX + iCntX) + ((iArrayCntY + iCntY) * g_iMakePropertiesSizeX);

			//if (g_bypArrayMAkeTileProperties[iArrayPropertiesIndex] != byCheckMakeProperties) {
			if (g_bypArrayMAkeTileProperties[iArrayPropertiesIndex] == 1) {
				++iCntX;
				continue;
			}

			iMapPropertiesIndex = (iPropertiesX + iCntX) + ((iPropertiesY + iCntY) * m_iPropertiesWidth);

			m_bypMapProperties[iMapPropertiesIndex] = g_bypArrayMAkeTileProperties[iArrayPropertiesIndex];
			++iCntX;
		}
		++iCntY;
	}
}

bool ObjectStageMap::CheckProperties(int iPropertiesX, int iPropertiesY)
{
	int iCntX;
	int iCntY;
	int iPropertiesSizeX;
	int iPropertiesSizeY;
	int iArrayPropertiesIndex;
	int iMapPropertiesIndex;
	int iWidth;
	int iHeight;
	BYTE byCheckMakeProperties;
	BYTE byCheckMapProperties;

	//--------------------------------------
	// 타일 범위
	// 중점 좌표를 뺀 값이 시작 위치 좌표
	//--------------------------------------
	iPropertiesX = iPropertiesX - g_iPropertiesCenterPointX;
	iPropertiesY = iPropertiesY - g_iPropertiesCenterPointY;
	iWidth = iPropertiesX + df_TILE_PROPERTIES_X;
	iHeight = iPropertiesY + df_TILE_PROPERTIES_Y;
	

	iPropertiesSizeX = g_iMakePropertiesSizeX;
	iPropertiesSizeY = g_iMakePropertiesSizeY;
	byCheckMakeProperties = g_byCheckMakeProperties;
	byCheckMapProperties = g_byCheckMapProperties;

	iCntY = 0;
	while (iCntY < iPropertiesSizeY) {

		if (iPropertiesY + iCntY < 0) {
			++iCntY;
			continue;
		}
		if (iPropertiesY + iCntY >= m_iPropertiesHeight) {
			break;
		}

		iCntX = 0;
		while (iCntX < iPropertiesSizeX) {

			iArrayPropertiesIndex = iCntX + (iCntY * iPropertiesSizeX);

			if (g_bypArrayMAkeTileProperties[iArrayPropertiesIndex] != byCheckMakeProperties) {
				++iCntX;
				continue;
			}
			if (iPropertiesX + iCntX < 0) {
				++iCntX;
				continue;
			}
			if (iPropertiesX + iCntX >= m_iPropertiesWidth) {
				break;
			}

			iMapPropertiesIndex = (iPropertiesX + iCntX) + ((iPropertiesY + iCntY) * m_iPropertiesWidth);

			if (m_bypMapProperties[iMapPropertiesIndex] != byCheckMapProperties) {
				return false;
			}

			++iCntX;
		}
		++iCntY;
	}

	return true;
}
