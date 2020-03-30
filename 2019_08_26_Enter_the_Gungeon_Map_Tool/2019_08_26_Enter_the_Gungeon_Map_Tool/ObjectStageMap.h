#pragma once
#include "Object.h"

class ObjectUnit;

struct st_TileData;

class ObjectStageMap : public Object
{
public:
	ObjectStageMap(int iMapWidth, int iMapHeight);
	virtual ~ObjectStageMap();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	// 맵 크기 변경시 기존 메모리를 삭제하고 다시 메모리 사이즈를 할당
	void ResizeMap(int iMapWidth, int iMapHeight);
	bool CheckObjectMake(int iTileX, int iTileY);									// 오브젝트 만들 위치 검사
	void DeleteUnit(ObjectUnit * DeleteUnitPtr);									// 유닛 삭제
	int GetBufferSize();
	void CreateUnit(int iPosX, int iPosY, int iUnitType, int iIncreaseVlaue);		// 유닛 만들기

private:
	void CreateMap(int iMapWidth, int iMapHeight);
	void ReleaseMap();
	void ModifyTileService(int iTileX, int iTileY);									// 타일 교체 일괄 서비스
	// 천장 테두리 타일 교체 부분 처리 (서비스에 종속) 4방향 타일을 검사하고 교체함
	// bCertainlyBorderFlag : 검사 도중 타일을 변경할때 이 타일이 천장이 되어야하는 경우 true
	void ModifyBorderTileProcess(int iTileX, int iTileY, int iSelectedTileType, bool bCertainlyBorderFlag = false);
	void ModifyAroundTile(int iTileX, int iTileY);									// 주변 타일 교체
	void CreateUnit(int iTileX, int iTileY);										// 유닛 만들기
	void DeleteUnitService(int iTileX, int iTileY);														// 유닛 삭제 서비스
	void DeleteUnitProcess(int iTileX, int iTileY);									// 유닛 삭제 처리
	void CreateLocation(int iTileX, int iTileY);									// 로케이션 만들기

public:
	st_TileData *m_stpTileMap;		// 타일맵 정보 구조체 버퍼

private:
	int m_iTileWidth;		// m_stpTileMap 의 너비
	int m_iTileHeight;		// m_stpTileMap 의 높이
	int m_iBufferSize;		// m_stpTileMap 의 크기
};

