#pragma once
#include "ObjectUnit.h"

class UnitLocation : public ObjectUnit
{
public:
	UnitLocation(int iPosX, int iPosY);
	virtual ~UnitLocation();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	void Drag();					// 마우스 클릭 드래그
	void Move(int iMoveValueX, int iMoveValueY);
	void ReSizeLeft(int iMoveValue);
	void ReSizeTop(int iMoveValue);
	void ReSizeRight(int iMoveValue);
	void ReSizeBottom(int iMoveValue);

public:
	int m_iLocationNumber;			// 로케이션 고유 번호

private:
	int m_iNameSize;				// 버튼 이름 길이
	wchar_t * m_wchpName;			// 버튼 이름 배열
	FT_Face * m_face;				// 폰트 페이스
};

