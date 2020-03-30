//#pragma once

#ifndef __SCREEN_DIB__
#define __SCREEN_DIB__

//DIB를 사용한 GDI용 스크린 버퍼
//윈도우의 HDC에 DIB를 사용하여 그림을 찍어준다.
class CScreenDib {
public :
	CScreenDib(int iWidth, int iHeight, int iColorBit);
	~CScreenDib();

protected :
	void CreateDibBuffer(int iWidth, int iHeight, int iColorBit);
	void ReleaseDibBuffer(void);

public :
	void DrawBuffer(HWND hWnd, int iX = 0, int iY = 0);

	BYTE *GetDibBuffer();
	int GetWidth();
	int GetHeight();
	int GetPitch();
	BITMAPINFO * GetDibInfo();

	// 2019.07.12	추가
	// 이미지 파일 24비트 32비트 호환을 위해 필요하다
	int GetColorByte();
	// 2019.07.19	추가
	// 스크린 크기 변경을 위해 필요하다
	void ResizeScreenDib(int iWidth, int iHeight, int iColorBit);

protected :
	BITMAPINFO	m_stDibInfo;		// 비트맵 이미지 정보
	BYTE		*m_bypBuffer;		// 비트맵 포인터

	int			m_iWidth;			// 가로 길이
	int			m_iHeight;			// 세로 길이
	int			m_iPitch;			// 피치값 (다음줄로 넘어가는데 필요한 길이 값)
	int			m_iColorBit;		// 비트 컬러( 24, 32 bit)
	int			m_iBufferSize;		// 버퍼 사이즈 ( 피치 * 세로)
};

#endif