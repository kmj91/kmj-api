//#pragma once

#ifndef __SPRITE_DIB__
#define __SPRITE_DIB__




class CSpriteDib
{
public:

	// **************************************************************** 
	// DIB 스프라이트 구조체. 
	//
	// 스프라이트 이미지와 사이즈 정보를 가짐.
	// **************************************************************** 
	typedef struct st_SPRITE
	{
		BYTE	*bypImage;				// 스프라이트 이미지 포인터.
		int		iWidth;					// Widht
		int		iHeight;				// Height
		int		iPitch;					// Pitch

		int		iCenterPointX;			// 중점 X
		int		iCenterPointY;			// 중점 Y
		int		iColorByte;				// 이미지가 한픽셀을 표현하는데 필요한 바이트 수 (비트 컬러 ex.24비트 컬러 , 32비트 컬러)
	}st_SPRITE;
	////////////////////////////////////////////////////////////////////
	// 생성자, 파괴자.
	//
	// Parameters: (int)최대 스프라이트 개수. (DWORD)투명칼라.
	////////////////////////////////////////////////////////////////////
	CSpriteDib();
	CSpriteDib(int iMaxSprite, DWORD dwColorKey);
	virtual ~CSpriteDib();

	///////////////////////////////////////////////////////
	// LoadDibSprite. 
	// BMP파일을 읽어서 하나의 프레임으로 저장한다.
	//
	///////////////////////////////////////////////////////
	BOOL LoadDibSprite(int iSpriteIndex, const wchar_t * szFileName, int iCenterPointX, int iCenterPointY);

	///////////////////////////////////////////////////////
	// ReleaseSprite. 
	// 해당 스프라이트 해제.
	//
	///////////////////////////////////////////////////////
	void ReleaseSprite(int iSpriteIndex);


	///////////////////////////////////////////////////////
	// DrawCompatibilitySprite. 
	// 특정 메모리 위치에 스프라이트를 출력한다. (칼라키, 클리핑 처리)
	// sprite 비트컬러 24, 32 호환
	// dest 비트컬러 24, 32 호환
	///////////////////////////////////////////////////////
	void CompatibleDrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDestColorByte);

	// 24, 32 비트 컬러 호환 합성 출력
	void CompatibleDrawSpriteCompose(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iDestColorByte, double dCompose);


	///////////////////////////////////////////////////////
	// DrawSprite. 
	// 특정 메모리 위치에 스프라이트를 출력한다. (칼라키, 클리핑 처리)
	//
	///////////////////////////////////////////////////////
	void DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, bool bCameraOption = true);
	///////////////////////////////////////////////////////
	// DrawImage. 
	// 특정 메모리 위치에 이미지를 출력한다. (클리핑 처리)
	//
	///////////////////////////////////////////////////////
	void DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, bool bCameraOption = true);

	// 스프라이트 특정영역 잘래내고 출력
	void DrawCutOutImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, RECT SpriteCutOutRect, bool bCameraOption = true);

	// 빨간색 톤으로 출력
	void DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch);

	// 합성 출력
	void DrawSpriteCompose(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, double dCompose = 1.0, bool bCameraOption = true);

	// 체력바 용
	void DrawPercentage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
		int iDestHeight, int iDestPitch, int iPercentage = 100);

	// dest를 특정 색으로 채운다 (배경)
	void DrawBackground(COLORREF RGBColor, BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDestColorByte);
	void DrawBackground(DWORD dwRGBColor, int iDrawX, int iDrawY, int iWidth, int iHeight, 
		BYTE *bypDest, int iDestWidth, int iDestHeight, int iDestPitch,
		double dCompose = 1.0, bool bCameraOption = false, bool bComposeOption = false);

	//카메라 위치
	void SetCameraPosition(int iPosX, int iPosY);

	void Reset(int iMaxSprite, DWORD dwColorKey);

protected:

	//------------------------------------------------------------------
	// Sprite 배열 정보.
	//------------------------------------------------------------------
	int		m_iMaxSprite;

public:
	st_SPRITE	*m_stpSprite;

protected:
	//------------------------------------------------------------------
	// 투명 색상으로 사용할 컬러.
	//------------------------------------------------------------------
	DWORD		m_dwColorKey;

	//플레이어 카메라 왼쪽 위 꼭지점 0,0 부분 좌표
	int	m_iCameraPosX;
	int m_iCameraPosY;
};

#endif
