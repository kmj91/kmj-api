#pragma once

class ObjectLight;

class ObjectLightDib : public Object
{
public:
	ObjectLightDib(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey);
	virtual ~ObjectLightDib();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

	// 맵 크기 변경시 기존 메모리를 삭제하고 다시 메모리 사이즈를 할당
	void ResizeMap(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey);
	void CreateLight(int iSpriteIndex, int iDrawX, int iDrawY, int iIncreaseValue);
	void DeleteLight(ObjectLight * DeleteObjectLightPtr);
	void ClearLightBuffer();		// 조명 버퍼 비우기

private:
	void CreateDibBuffer(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey);
	void ReleaseDibBuffer(bool bResizeFlag = false);

private:
	BYTE		*m_bypBuffer;		// 비트맵 포인터

	DWORD		m_dwColorKey;		// 투명 컬러

	int			m_iWidth;			// 가로 길이
	int			m_iHeight;			// 세로 길이
	int			m_iPitch;			// 피치값 (다음줄로 넘어가는데 필요한 길이 값)
	int			m_iColorBit;		// 비트 컬러( 24, 32 bit)
	int			m_iBufferSize;		// 버퍼 사이즈 ( 피치 * 세로)
};

