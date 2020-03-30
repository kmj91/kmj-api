#include "stdafx.h"
#include <windows.h>
#include "CSpriteDib.h"
#include "stdio.h"

CSpriteDib::CSpriteDib()
{
	int iCount;

	//-----------------------------------------------------------------
	// 최대 읽어올 개수 만큼 미리 할당 받는다.
	//-----------------------------------------------------------------
	m_iMaxSprite = 100;
	m_dwColorKey = 0x00ff00ff;
	m_stpSprite = new st_SPRITE[100];

	// 초기화
	iCount = 0;
	while (iCount < 100) {
		m_stpSprite[iCount].bypImage = NULL;
		++iCount;
	}

	//기본 카메라 위치
	m_iCameraPosX = 0;
	m_iCameraPosY = 0;
}

////////////////////////////////////////////////////////////////////
// 생성자, 파괴자.
//
// Parameters: (int)최대 스프라이트 개수. (DWORD)투명칼라.
////////////////////////////////////////////////////////////////////
CSpriteDib::CSpriteDib(int iMaxSprite, DWORD dwColorKey)
{
	int iCount;

	//-----------------------------------------------------------------
	// 최대 읽어올 개수 만큼 미리 할당 받는다.
	//-----------------------------------------------------------------
	m_iMaxSprite = iMaxSprite;
	m_dwColorKey = dwColorKey;
	m_stpSprite = new st_SPRITE[iMaxSprite];

	// 초기화
	iCount = 0;
	while (iCount < iMaxSprite) {
		m_stpSprite[iCount].bypImage = NULL;
		++iCount;
	}

	//기본 카메라 위치
	m_iCameraPosX = 0;
	m_iCameraPosY = 0;
}

CSpriteDib::~CSpriteDib()
{
	int iCount;
	//-----------------------------------------------------------------
	// 전체를 돌면서 모두 지우자.
	//-----------------------------------------------------------------
	for (iCount = 0; iCount < m_iMaxSprite; iCount++)
	{
		ReleaseSprite(iCount);
	}
	delete[] m_stpSprite;
}

///////////////////////////////////////////////////////
// LoadDibSprite. 
// BMP파일을 읽어서 하나의 프레임으로 저장한다.
//
// Parameters: (int)SpriteIndex. (const wchar_t *)FileName. (int)CenterPointX. (int)CenterPointY.
// Return: (BOOL)TRUE, FALSE.
///////////////////////////////////////////////////////
BOOL CSpriteDib::LoadDibSprite(int iSpriteIndex, const wchar_t * szFileName, int iCenterPointX,
	int iCenterPointY)
{
	//HANDLE		hFile;
	//DWORD		dwRead;
	int		iPitch;
	size_t		iImageSize;
	BITMAPFILEHEADER	stFileHeader;
	BITMAPINFOHEADER	stInfoHeader;
	
	FILE *fp;
	int err;
	int iCount;
	int iColorByte;
	//-----------------------------------------------------------------
	// 비트맵 헤더를 열어 BMP 파일 확인.
	//-----------------------------------------------------------------
	err = _wfopen_s(&fp, szFileName, L"rb");
	if (err != 0) {
		return 0;
	}
	fread(&stFileHeader, sizeof(BITMAPFILEHEADER), 1, fp);
	if (stFileHeader.bfType != 0x4d42) {
		return 0;
	}
	
	//-----------------------------------------------------------------
	// 한줄, 한줄의 피치값을 구한다.
	//-----------------------------------------------------------------
	fread(&stInfoHeader, sizeof(BITMAPINFOHEADER), 1, fp);
	iColorByte = stInfoHeader.biBitCount / 8;
	iPitch = (stInfoHeader.biWidth * iColorByte + 3) & ~3;

	//-----------------------------------------------------------------
	// 스프라이트 구조체에 크기 저장.
	//-----------------------------------------------------------------
	m_stpSprite[iSpriteIndex].iWidth = stInfoHeader.biWidth;
	m_stpSprite[iSpriteIndex].iHeight = stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].iPitch = iPitch;
	m_stpSprite[iSpriteIndex].iCenterPointX = iCenterPointX;
	m_stpSprite[iSpriteIndex].iCenterPointY = iCenterPointY;
	m_stpSprite[iSpriteIndex].iColorByte = iColorByte;

	//-----------------------------------------------------------------
	// 이미지에 대한 전체 크기를 구하고, 메모리 할당.
	//-----------------------------------------------------------------
	iImageSize = iPitch * stInfoHeader.biHeight;
	m_stpSprite[iSpriteIndex].bypImage = new BYTE[iImageSize];
	
	//-----------------------------------------------------------------
	// 이미지 부분은 스프라이트 버퍼로 읽어온다.
	// DIB 는 뒤집어져 있으므로 이를 다시 뒤집자.
	// 임시 버퍼에 읽은 뒤에 뒤집으면서 복사한다.
	//-----------------------------------------------------------------
	BYTE *buffer = new BYTE[iImageSize];
	BYTE *tempBuffer;
	BYTE *tempSprite = m_stpSprite[iSpriteIndex].bypImage;
	fread(buffer, iImageSize, 1, fp);
	tempBuffer = buffer +(iPitch * (stInfoHeader.biHeight - 1));

	for (iCount = 0; iCount < stInfoHeader.biHeight; iCount++) {
		//memcpy(tempSprite, tempBuffer, stInfoHeader.biWidth * 4);
		memcpy(tempSprite, tempBuffer, stInfoHeader.biWidth * iColorByte);
		tempBuffer = tempBuffer - iPitch;
		tempSprite = tempSprite + iPitch;
	}

	delete[] buffer;
	fclose(fp);
	//CloseHandle(hFile);
	
	return FALSE;
}
///////////////////////////////////////////////////////
// ReleaseSprite. 
// 해당 스프라이트 해제.
//
// Parameters: (int)SpriteIndex.
// Return: (BOOL)TRUE, FALSE.
///////////////////////////////////////////////////////
void CSpriteDib::ReleaseSprite(int iSpriteIndex)
{
	//-----------------------------------------------------------------
	// 최대 할당된 스프라이트를 넘어서면 안되지롱
	//-----------------------------------------------------------------
	if (m_iMaxSprite <= iSpriteIndex)
		return;

	if (NULL != m_stpSprite[iSpriteIndex].bypImage)
	{
		//-----------------------------------------------------------------
		// 삭제 후 초기화.
		//-----------------------------------------------------------------
		delete[] m_stpSprite[iSpriteIndex].bypImage;
		memset(&m_stpSprite[iSpriteIndex], 0, sizeof(st_SPRITE));
	}
}

///////////////////////////////////////////////////////
// DrawCompatibilitySprite. 
// 특정 메모리 위치에 스프라이트를 출력한다. (칼라키, 클리핑 처리)
// 이미지파일 비트컬러 24, 32 호환
///////////////////////////////////////////////////////
void CSpriteDib::CompatibleDrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDestColorByte)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	int iSpriteColorByte = stpSprite->iColorByte;
	BYTE *bypSprite = stpSprite->bypImage;
	BYTE *bypCopyDest = bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	int iR;
	int iG;
	int iB;

	iR = GetRValue(m_dwColorKey);
	iG = GetGValue(m_dwColorKey);
	iB = GetBValue(m_dwColorKey);

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	//2018.02.08
	//카메라 기준으로 스프라이트 값 클리핑 처리
	int clippingY;
	int clippingX;

	if (iDrawY < m_iCameraPosY) {
		//2018.02.08
		clippingY = m_iCameraPosY - iDrawY;
		iSpriteHeight = iSpriteHeight - clippingY;
		if (iSpriteHeight > 0) {
			bypSprite = bypSprite + iSpritePitch * clippingY;
		}
		iDrawY = m_iCameraPosY;
	}

	if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
	}

	if (iDrawX < m_iCameraPosX) {
		//2018.02.08
		clippingX = m_iCameraPosX - iDrawX;
		iSpriteWidth = iSpriteWidth - clippingX;
		if (iSpriteWidth > 0) {
			bypSprite = bypSprite + (clippingX * iSpriteColorByte);
		}
		iDrawX = m_iCameraPosX;
	}

	if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
	}


	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//2018.02.08
	//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
	iDrawX = iDrawX - m_iCameraPosX;
	iDrawY = iDrawY - m_iCameraPosY;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	bypCopyDest = bypCopyDest + (iDrawX * iDestColorByte) + (iDrawY * iDestPitch);
	bypTempSprite = bypSprite;
	bypTempDest = bypCopyDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{

			if (iR != *(bypSprite + 2)
				|| iG != *(bypSprite + 1)
				|| iB != *(bypSprite + 0)) {

				*((BYTE *)bypCopyDest + 2) = *(bypSprite + 2);
				*((BYTE *)bypCopyDest + 1) = *(bypSprite + 1);
				*((BYTE *)bypCopyDest + 0) = *(bypSprite + 0);

			}

			//다음칸 이동
			bypCopyDest = bypCopyDest + iDestColorByte;
			bypSprite = bypSprite + iDestColorByte;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		bypCopyDest = bypTempDest;
		bypSprite = bypTempSprite;
	}
}

// 24, 32 비트 컬러 호환 합성 출력
void CSpriteDib::CompatibleDrawSpriteCompose(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iDestColorByte, double dCompose)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	int iSpriteColorByte = stpSprite->iColorByte;
	BYTE *bypSprite = stpSprite->bypImage;
	BYTE *bypCopyDest = bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	double dDestCompose = 1.0;
	int iR;
	int iG;
	int iB;

	iR = GetRValue(m_dwColorKey);
	iG = GetGValue(m_dwColorKey);
	iB = GetBValue(m_dwColorKey);

	dDestCompose = dDestCompose - dCompose;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	//2018.02.08
	//카메라 기준으로 스프라이트 값 클리핑 처리
	int clippingY;
	int clippingX;

	if (iDrawY < m_iCameraPosY) {
		//2018.02.08
		clippingY = m_iCameraPosY - iDrawY;
		iSpriteHeight = iSpriteHeight - clippingY;
		if (iSpriteHeight > 0) {
			bypSprite = bypSprite + iSpritePitch * clippingY;
		}
		iDrawY = m_iCameraPosY;
	}

	if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
	}

	if (iDrawX < m_iCameraPosX) {
		//2018.02.08
		clippingX = m_iCameraPosX - iDrawX;
		iSpriteWidth = iSpriteWidth - clippingX;
		if (iSpriteWidth > 0) {
			bypSprite = bypSprite + (clippingX * iSpriteColorByte);
		}
		iDrawX = m_iCameraPosX;
	}

	if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
	}


	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//2018.02.08
	//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
	iDrawX = iDrawX - m_iCameraPosX;
	iDrawY = iDrawY - m_iCameraPosY;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	bypCopyDest = bypCopyDest + (iDrawX * iDestColorByte) + (iDrawY * iDestPitch);
	bypTempSprite = bypSprite;
	bypTempDest = bypCopyDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			if (iR != *(bypSprite + 2)
				|| iG != *(bypSprite + 1)
				|| iB != *(bypSprite + 0)) {

				*((BYTE *)bypCopyDest + 2) = (*((BYTE *)bypCopyDest + 2) * dDestCompose) + (*(bypSprite + 2) * dCompose);
				*((BYTE *)bypCopyDest + 1) = (*((BYTE *)bypCopyDest + 1) * dDestCompose) + (*(bypSprite + 1) * dCompose);
				*((BYTE *)bypCopyDest + 0) = (*((BYTE *)bypCopyDest + 0) * dDestCompose) + (*(bypSprite + 0) * dCompose);
			}

			//다음칸 이동
			bypCopyDest = bypCopyDest + iDestColorByte;
			bypSprite = bypSprite + iDestColorByte;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		bypCopyDest = bypTempDest;
		bypSprite = bypTempSprite;
	}
}

///////////////////////////////////////////////////////
// DrawSprite. 
// 특정 메모리 위치에 스프라이트를 출력한다. (클리핑 처리)
// 체력바 처리
//
///////////////////////////////////////////////////////
void CSpriteDib::DrawSprite(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, bool bCameraOption)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	int iSpriteColorByte = stpSprite->iColorByte;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	if (bCameraOption) {
		//2018.02.08
		//카메라 기준으로 스프라이트 값 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < m_iCameraPosY) {
			//2018.02.08
			clippingY = m_iCameraPosY - iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			if (iSpriteHeight > 0) {
				dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
			}
			iDrawY = m_iCameraPosY;
		}

		if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
		}

		if (iDrawX < m_iCameraPosX) {
			//2018.02.08
			clippingX = m_iCameraPosX - iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			if (iSpriteWidth > 0) {
				dwpSprite = dwpSprite + clippingX;
			}
			iDrawX = m_iCameraPosX;
		}

		if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
		}

		//2018.02.08
		//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
		iDrawX = iDrawX - m_iCameraPosX;
		iDrawY = iDrawY - m_iCameraPosY;
	}
	else {
		// 2019.07.22
		// 화면 기준으로 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < 0) {
			//2018.02.08
			clippingY = -iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			iDrawY = 0;
		}

		if (iDrawY + iSpriteHeight >= iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - iDestHeight);
		}

		if (iDrawX < 0) {
			//2018.02.08
			clippingX = -iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			iDrawX = 0;
		}

		if (iDrawX + iSpriteWidth >= iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - iDestWidth);
		}
	}

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			if (m_dwColorKey != (*dwpSprite & 0x00ffffff)) {
				*dwpDest = *dwpSprite;
			}

			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}


void CSpriteDib::DrawImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, bool bCameraOption)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;
		

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;
	
	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------
	
	if (bCameraOption) {
		//2018.02.08
		//카메라 기준으로 스프라이트 값 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < m_iCameraPosY) {
			//2018.02.08
			clippingY = m_iCameraPosY - iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			if (iSpriteHeight > 0) {
				dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
			}
			iDrawY = m_iCameraPosY;
		}

		if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
		}

		if (iDrawX < m_iCameraPosX) {
			//2018.02.08
			clippingX = m_iCameraPosX - iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			if (iSpriteWidth > 0) {
				dwpSprite = dwpSprite + clippingX;
			}
			iDrawX = m_iCameraPosX;
		}

		if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
		}

		//2018.02.08
		//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
		iDrawX = iDrawX - m_iCameraPosX;
		iDrawY = iDrawY - m_iCameraPosY;
	}
	else {
		// 2019.07.22
			// 화면 기준으로 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < 0) {
			//2018.02.08
			clippingY = -iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			iDrawY = 0;
		}

		if (iDrawY + iSpriteHeight >= iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - iDestHeight);
		}

		if (iDrawX < 0) {
			//2018.02.08
			clippingX = -iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			iDrawX = 0;
		}

		if (iDrawX + iSpriteWidth >= iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - iDestWidth);
		}
	}

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			*dwpDest = *dwpSprite;
			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;
		
		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}



/*********************************
	DrawCutOutImage
	스프라이트 특정영역 잘래내고 출력
**********************************/
void CSpriteDib::DrawCutOutImage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE * bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, RECT SpriteCutOutRect, bool bCameraOption)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;


	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	int clippingY = 0;
	int clippingX = 0;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	// 2019.08.02
	// 스프라이트 잘라내기

	// 위
	if (iSpriteHeight < SpriteCutOutRect.top) {
		return;
	}
	else {
		iSpriteHeight = iSpriteHeight - SpriteCutOutRect.top;
		dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * SpriteCutOutRect.top);
	}

	// 아래
	if (iSpriteHeight < SpriteCutOutRect.bottom) {
		return;
	}
	else {
		iSpriteHeight = iSpriteHeight - SpriteCutOutRect.bottom;
	}

	// 왼쪽
	if (iSpriteWidth < SpriteCutOutRect.left) {
		return;
	}
	else {
		iSpriteWidth = iSpriteWidth - SpriteCutOutRect.left;
		dwpSprite = dwpSprite + SpriteCutOutRect.left;
	}

	// 오른쪽
	if (iSpriteHeight < SpriteCutOutRect.right) {
		return;
	}
	else {
		iSpriteHeight = iSpriteHeight - SpriteCutOutRect.right;
	}


	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	if (bCameraOption) {
		//2018.02.08
		//카메라 기준으로 스프라이트 값 클리핑 처리
		if (iDrawY < m_iCameraPosY) {
			//2018.02.08
			clippingY = m_iCameraPosY - iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			if (iSpriteHeight > 0) {
				dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
			}
			iDrawY = m_iCameraPosY;
		}

		if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
		}

		if (iDrawX < m_iCameraPosX) {
			//2018.02.08
			clippingX = m_iCameraPosX - iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			if (iSpriteWidth > 0) {
				dwpSprite = dwpSprite + clippingX;
			}
			iDrawX = m_iCameraPosX;
		}

		if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
		}

		//2018.02.08
		//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
		iDrawX = iDrawX - m_iCameraPosX;
		iDrawY = iDrawY - m_iCameraPosY;
	}
	else {
		// 2019.07.22
		// 화면 기준으로 클리핑 처리
		if (iDrawY < 0) {
			//2018.02.08
			clippingY = -iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			if (iSpriteHeight > 0) {
				dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
			}
			iDrawY = 0;
		}

		if (iDrawY + iSpriteHeight >= iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - iDestHeight);
		}

		if (iDrawX < 0) {
			//2018.02.08
			clippingX = -iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			if (iSpriteWidth > 0) {
				dwpSprite = dwpSprite + clippingX;
			}
			iDrawX = 0;
		}

		if (iDrawX + iSpriteWidth >= iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - iDestWidth);
		}
	}

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}


	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			*dwpDest = *dwpSprite;
			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}



/*********************************
	DrawSpriteRed
	빨간색 톤으로 이미지 출력
**********************************/
void CSpriteDib::DrawSpriteRed(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	//2018.02.08
	//카메라 기준으로 스프라이트 값 클리핑 처리
	int clippingY;
	int clippingX;

	if (iDrawY < m_iCameraPosY) {
		//2018.02.08
		clippingY = m_iCameraPosY - iDrawY;
		iSpriteHeight = iSpriteHeight - clippingY;
		if (iSpriteHeight > 0) {
			dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
		}
		iDrawY = m_iCameraPosY;
	}

	if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
	}

	if (iDrawX < m_iCameraPosX) {
		//2018.02.08
		clippingX = m_iCameraPosX - iDrawX;
		iSpriteWidth = iSpriteWidth - clippingX;
		if (iSpriteWidth > 0) {
			dwpSprite = dwpSprite + clippingX;
		}
		iDrawX = m_iCameraPosX;
	}

	if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
	}


	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//2018.02.08
	//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
	iDrawX = iDrawX - m_iCameraPosX;
	iDrawY = iDrawY - m_iCameraPosY;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			if (m_dwColorKey != (*dwpSprite & 0x00ffffff)) {
				*((BYTE *)dwpDest + 2) = *((BYTE *)dwpSprite + 2);
				*((BYTE *)dwpDest + 1) = *((BYTE *)dwpSprite + 1) / 2;
				*((BYTE *)dwpDest + 0) = *((BYTE *)dwpSprite + 0) / 2;
			}

			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}




/*********************************
	DrawSpriteCompose
	합성 출력
**********************************/
void CSpriteDib::DrawSpriteCompose(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, double dCompose, bool bCameraOption)
{
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	double dDestCompose = 1.0;

	if (dCompose > 1.0) {
		dCompose = 1.0;
	}
	else if (dCompose < 0) {
		dCompose = 0;
	}

	dDestCompose = dDestCompose - dCompose;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	if (bCameraOption) {
		//2018.02.08
		//카메라 기준으로 스프라이트 값 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < m_iCameraPosY) {
			//2018.02.08
			clippingY = m_iCameraPosY - iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			if (iSpriteHeight > 0) {
				dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
			}
			iDrawY = m_iCameraPosY;
		}

		if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
		}

		if (iDrawX < m_iCameraPosX) {
			//2018.02.08
			clippingX = m_iCameraPosX - iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			if (iSpriteWidth > 0) {
				dwpSprite = dwpSprite + clippingX;
			}
			iDrawX = m_iCameraPosX;
		}

		if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
		}

		//2018.02.08
		//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
		iDrawX = iDrawX - m_iCameraPosX;
		iDrawY = iDrawY - m_iCameraPosY;
	}
	else {
		// 2019.07.22
			// 화면 기준으로 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < 0) {
			//2018.02.08
			clippingY = -iDrawY;
			iSpriteHeight = iSpriteHeight - clippingY;
			iDrawY = 0;
		}

		if (iDrawY + iSpriteHeight >= iDestHeight) {
			iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - iDestHeight);
		}

		if (iDrawX < 0) {
			//2018.02.08
			clippingX = -iDrawX;
			iSpriteWidth = iSpriteWidth - clippingX;
			iDrawX = 0;
		}

		if (iDrawX + iSpriteWidth >= iDestWidth) {
			iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - iDestWidth);
		}
	}

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth > iCountX; iCountX++)
		{
			if (m_dwColorKey != (*dwpSprite & 0x00ffffff)) {
				*((BYTE *)dwpDest + 2) = (*((BYTE *)dwpSprite + 2) * dDestCompose) + (*((BYTE *)dwpDest + 2) * dCompose);
				*((BYTE *)dwpDest + 1) = (*((BYTE *)dwpSprite + 1) * dDestCompose) + (*((BYTE *)dwpDest + 1) * dCompose);
				*((BYTE *)dwpDest + 0) = (*((BYTE *)dwpSprite + 0) * dDestCompose) + (*((BYTE *)dwpDest + 0) * dCompose);
			}

			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}

// 체력바 용
void CSpriteDib::DrawPercentage(int iSpriteIndex, int iDrawX, int iDrawY, BYTE *bypDest, int iDestWidth,
	int iDestHeight, int iDestPitch, int iPercentage) {
	//-----------------------------------------------------------------
	// 최대 스프라이트 개수를 초과 하거나, 로드되지 않는 스프라이트라면 무시
	//-----------------------------------------------------------------
	if (m_iMaxSprite < iSpriteIndex)
		return;

	if (m_stpSprite[iSpriteIndex].bypImage == NULL)
		return;

	//-----------------------------------------------------------------
	// 지역변수로 필요정보 셋팅
	//-----------------------------------------------------------------
	st_SPRITE *stpSprite = &m_stpSprite[iSpriteIndex];

	int iCountY;
	int iCountX;
	int iSpriteHeight = stpSprite->iHeight;
	int iSpriteWidth = stpSprite->iWidth;
	int iSpritePitch = stpSprite->iPitch;
	int iSpriteCenterX = stpSprite->iCenterPointX;
	int iSpriteCenterY = stpSprite->iCenterPointY;
	DWORD *dwpSprite = (DWORD *)stpSprite->bypImage;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;

	//-----------------------------------------------------------------
	// 출력 중점으로 좌표 계산
	//-----------------------------------------------------------------
	iDrawX = iDrawX - iSpriteCenterX;
	iDrawY = iDrawY - iSpriteCenterY;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	//2018.02.08
	//카메라 기준으로 스프라이트 값 클리핑 처리
	int clippingY;
	int clippingX;

	if (iDrawY < m_iCameraPosY) {
		//2018.02.08
		clippingY = m_iCameraPosY - iDrawY;
		iSpriteHeight = iSpriteHeight - clippingY;
		if (iSpriteHeight > 0) {
			dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
		}
		iDrawY = m_iCameraPosY;
	}

	if (iDrawY + iSpriteHeight >= m_iCameraPosY + iDestHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (m_iCameraPosY + iDestHeight));
	}

	if (iDrawX < m_iCameraPosX) {
		//2018.02.08
		clippingX = m_iCameraPosX - iDrawX;
		iSpriteWidth = iSpriteWidth - clippingX;
		if (iSpriteWidth > 0) {
			dwpSprite = dwpSprite + clippingX;
		}
		iDrawX = m_iCameraPosX;
	}

	if (iDrawX + iSpriteWidth >= m_iCameraPosX + iDestWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (m_iCameraPosX + iDestWidth));
	}


	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//2018.02.08
	//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
	iDrawX = iDrawX - m_iCameraPosX;
	iDrawY = iDrawY - m_iCameraPosY;

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempSprite = (BYTE *)dwpSprite;
	bypTempDest = (BYTE *)dwpDest;

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iSpriteHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iSpriteWidth - ((100 - iPercentage) * ((double)iSpriteWidth / (double)100)) > iCountX; iCountX++)
		{
			if (m_dwColorKey != (*dwpSprite & 0x00ffffff)) {
				*dwpDest = *dwpSprite;
			}

			//다음칸 이동
			++dwpDest;
			++dwpSprite;
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;
		bypTempSprite = bypTempSprite + iSpritePitch;

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}

void CSpriteDib::DrawBackground(COLORREF RGBColor, BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch, int iDestColorByte)
{
	int iCountY;
	int iCountX;
	int iR;
	int iG;
	int iB;
	BYTE *bypCopyDest;
	BYTE *bypTempDest;

	bypCopyDest = bypDest;
	bypTempDest = bypCopyDest;

	
	iR = GetRValue(RGBColor);
	iG = GetGValue(RGBColor);
	iB = GetBValue(RGBColor);

	//-----------------------------------------------------------------
	// 전체 크기를 돌면서 픽셀마다 투명색 처리를 하며 그림 출력.
	//-----------------------------------------------------------------
	for (iCountY = 0; iDestHeight > iCountY; iCountY++)
	{
		for (iCountX = 0; iDestWidth > iCountX; iCountX++)
		{
			*(bypCopyDest + 2) = iR;
			*(bypCopyDest + 1) = iG;
			*(bypCopyDest + 0) = iB;

			//다음칸 이동
			bypCopyDest = bypCopyDest + iDestColorByte;

		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + iDestPitch;

		bypCopyDest = bypTempDest;
	}
}

void CSpriteDib::DrawBackground(DWORD dwRGBColor, int iDrawX, int iDrawY, int iWidth, int iHeight, 
	BYTE * bypDest, int iDestWidth, int iDestHeight, int iDestPitch,
	double dCompose, bool bCameraOption, bool bComposeOption)
{
	int iCountY;
	int iCountX;
	DWORD *dwpDest = (DWORD *)bypDest;
	BYTE *bypTempDest;
	double dDestCompose = 1.0;

	if (dCompose > 1.0) {
		dCompose = 1.0;
	}
	else if (dCompose < 0) {
		dCompose = 0;
	}

	dDestCompose = dDestCompose - dCompose;

	//-----------------------------------------------------------------
	// 상단 에 대한 스프라이트 출력 위치 계산. (상단 클리핑)
	// 하단에 사이즈 계산. (하단 클리핑)
	// 왼쪽 출력 위치 계산. (좌측 클리핑)
	// 오른쪽 출력 위치 계산. (우측 클리핑)
	//-----------------------------------------------------------------

	if (bCameraOption) {
		//2018.02.08
		//카메라 기준으로 스프라이트 값 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < m_iCameraPosY) {
			//2018.02.08
			clippingY = m_iCameraPosY - iDrawY;
			iHeight = iHeight - clippingY;
			iDrawY = m_iCameraPosY;
		}

		if (iDrawY + iHeight >= m_iCameraPosY + iDestHeight) {
			iHeight = iHeight - ((iDrawY + iHeight) - (m_iCameraPosY + iDestHeight));
		}

		if (iDrawX < m_iCameraPosX) {
			//2018.02.08
			clippingX = m_iCameraPosX - iDrawX;
			iWidth = iWidth - clippingX;
			iDrawX = m_iCameraPosX;
		}

		if (iDrawX + iWidth >= m_iCameraPosX + iDestWidth) {
			iWidth = iWidth - ((iDrawX + iWidth) - (m_iCameraPosX + iDestWidth));
		}

		//2018.02.08
		//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
		iDrawX = iDrawX - m_iCameraPosX;
		iDrawY = iDrawY - m_iCameraPosY;
	}
	else {
		// 2019.07.22
		// 화면 기준으로 클리핑 처리
		int clippingY;
		int clippingX;

		if (iDrawY < 0) {
			//2018.02.08
			clippingY = -iDrawY;
			iHeight = iHeight - clippingY;
			iDrawY = 0;
		}

		if (iDrawY + iHeight >= iDestHeight) {
			iHeight = iHeight - ((iDrawY + iHeight) - iDestHeight);
		}

		if (iDrawX < 0) {
			//2018.02.08
			clippingX = -iDrawX;
			iWidth = iWidth - clippingX;
			iDrawX = 0;
		}

		if (iDrawX + iWidth >= iDestWidth) {
			iWidth = iWidth - ((iDrawX + iWidth) - iDestWidth);
		}
	}
	


	if (iWidth <= 0 || iHeight <= 0) {
		return;
	}


	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * iDestPitch));
	bypTempDest = (BYTE *)dwpDest;


	if (bComposeOption) {
		//-----------------------------------------------------------------
		// 인자로 받은 가로 세로 길이 만큼 dest를 특정 색으로 채운다 (배경)
		//-----------------------------------------------------------------
		for (iCountY = 0; iHeight > iCountY; iCountY++)
		{
			for (iCountX = 0; iWidth > iCountX; iCountX++)
			{
				*((BYTE *)dwpDest + 2) = (*((BYTE *)&dwRGBColor + 2) * dCompose) + (*((BYTE *)dwpDest + 2) * dDestCompose);
				*((BYTE *)dwpDest + 1) = (*((BYTE *)&dwRGBColor + 1) * dCompose) + (*((BYTE *)dwpDest + 1) * dDestCompose);
				*((BYTE *)dwpDest + 0) = (*((BYTE *)&dwRGBColor + 0) * dCompose) + (*((BYTE *)dwpDest + 0) * dDestCompose);

				//다음칸 이동
				++dwpDest;

			}
			//-----------------------------------------------------------------
			// 다음줄로 이동
			//-----------------------------------------------------------------
			bypTempDest = bypTempDest + iDestPitch;

			dwpDest = (DWORD *)bypTempDest;
		}
	}
	else {
		//-----------------------------------------------------------------
		// 인자로 받은 가로 세로 길이 만큼 dest를 특정 색으로 채운다 (배경)
		//-----------------------------------------------------------------
		for (iCountY = 0; iHeight > iCountY; iCountY++)
		{
			for (iCountX = 0; iWidth > iCountX; iCountX++)
			{
				*dwpDest = dwRGBColor;

				//다음칸 이동
				++dwpDest;

			}
			//-----------------------------------------------------------------
			// 다음줄로 이동
			//-----------------------------------------------------------------
			bypTempDest = bypTempDest + iDestPitch;

			dwpDest = (DWORD *)bypTempDest;
		}
	}
}

//카메라 위치
void CSpriteDib::SetCameraPosition(int iPosX, int iPosY) {
	m_iCameraPosX = iPosX;
	m_iCameraPosY = iPosY;
}

void CSpriteDib::Reset(int iMaxSprite, DWORD dwColorKey)
{
	int iCount;
	//-----------------------------------------------------------------
	// 전체를 돌면서 모두 지우자.
	//-----------------------------------------------------------------
	for (iCount = 0; iCount < m_iMaxSprite; iCount++)
	{
		ReleaseSprite(iCount);
	}
	delete[] m_stpSprite;


	//-----------------------------------------------------------------
	// 최대 읽어올 개수 만큼 미리 할당 받는다.
	//-----------------------------------------------------------------
	m_iMaxSprite = iMaxSprite;
	m_dwColorKey = dwColorKey;
	m_stpSprite = new st_SPRITE[iMaxSprite];

	// 초기화
	iCount = 0;
	while (iCount < iMaxSprite) {
		m_stpSprite[iCount].bypImage = NULL;
		++iCount;
	}

	//기본 카메라 위치
	m_iCameraPosX = 0;
	m_iCameraPosY = 0;
}
