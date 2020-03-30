#include "stdafx.h"
#include "ObjectLightDib.h"


ObjectLightDib::ObjectLightDib(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey)
{
	CreateDibBuffer(iMapWidth, iMapHeight, iColorBit, dwColorKey);
}


ObjectLightDib::~ObjectLightDib()
{
	ReleaseDibBuffer();
}

bool ObjectLightDib::Action()
{
	return false;
}

//------------------------------------------------
// 조명을 그립니다
// 정렬을 통해 마지막에 수행되어야할 것입니다.
// 대상 스프라이트 영역에맞춰서 RGB값을 밝게 조정합니다
//------------------------------------------------
void ObjectLightDib::Draw()
{
	int iCountY;
	int iCountX;
	int iSpriteHeight = m_iHeight;
	int iSpriteWidth = m_iWidth;
	int iSpritePitch = m_iPitch;
	int iSpriteCenterX = 0;	// 중점 좌표도 늘어남
	int iSpriteCenterY = 0;	// 중점 좌표도 늘어남
	DWORD *dwpSprite = (DWORD *)m_bypBuffer;
	DWORD *dwpDest = (DWORD *)g_bypDest;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	int clippingY;
	int clippingX;
	int iDrawX = 0;
	int iDrawY = 0;
	int iCameraPosX = g_CameraRect.left;
	int iCameraPosY = g_CameraRect.top;
	int iDestWidth = g_iDestWidth;
	int iDestHeight = g_iDestHeight;
	int iDestPitch = g_iDestPitch;
	int iRGBValue;
	int iMaxRGB;
	double dMultiplyValue;		// 곱셈 값
	bool bReviseFlag;			// 보정
	BYTE *bypCopyDest;
	int iRGBCnt;

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

	if (iDrawY < iCameraPosY) {
		//2018.02.08
		clippingY = iCameraPosY - iDrawY;
		iSpriteHeight = iSpriteHeight - clippingY;
		if (iSpriteHeight > 0) {
			dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * clippingY);
		}
		iDrawY = iCameraPosY;
	}

	if (iDrawY + iSpriteHeight >= iCameraPosY + iDestHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - (iCameraPosY + iDestHeight));
	}

	if (iDrawX < iCameraPosX) {
		//2018.02.08
		clippingX = iCameraPosX - iDrawX;
		iSpriteWidth = iSpriteWidth - clippingX;
		if (iSpriteWidth > 0) {
			dwpSprite = dwpSprite + clippingX;
		}
		iDrawX = iCameraPosX;
	}

	if (iDrawX + iSpriteWidth >= iCameraPosX + iDestWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) - (iCameraPosX + iDestWidth));
	}

	//2018.02.08
	//카메라 기준으로 iDrawX iDrawY 값 좌표 맞추기
	iDrawX = iDrawX - iCameraPosX;
	iDrawY = iDrawY - iCameraPosY;

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
			bypCopyDest = (BYTE *)dwpDest;
			// 해당 부분 색깔
			iMaxRGB = max(max(*((BYTE *)bypCopyDest + 2), *((BYTE *)bypCopyDest + 1)), *((BYTE *)bypCopyDest + 0));
			
			// 굉장히 밝은 RGB값이 있음
			if (iMaxRGB > 160) {
				bReviseFlag = true;
			}
			else {
				bReviseFlag = false;
			}

			iRGBCnt = 0;
			while (iRGBCnt < 3) {
				iRGBValue = *((BYTE *)bypCopyDest + iRGBCnt);

				if (0xffff00 == (*dwpSprite & 0x00ffffff)) {
					// 매우 어두운 색
					if (iRGBValue < 40) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 4;
						}
					}
					// 어두컴컴
					else if (iRGBValue < 80) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 2;
						}
					}
					// 밝음
					else if (iRGBValue < 120) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.7;
						}
					}
					// 조금 밝음
					else if (iRGBValue < 160) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.3;
						}
					}
					// 매우 밝음
					else {
						dMultiplyValue = 1;
					}

					*((BYTE *)bypCopyDest + iRGBCnt) = (int)((*((BYTE *)bypCopyDest + iRGBCnt) + 0) * dMultiplyValue);
				}
				else if (0xffff64 == (*dwpSprite & 0x00ffffff)) {
					// 매우 어두운 색
					if (iRGBValue < 40) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 3;
						}
					}
					// 어두컴컴
					else if (iRGBValue < 80) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.7;
						}
					}
					// 밝음
					else if (iRGBValue < 120) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.5;
						}
					}
					// 조금 밝음
					else if (iRGBValue < 160) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.3;
						}
					}
					// 매우 밝음
					else {
						dMultiplyValue = 1;
					}

					*((BYTE *)bypCopyDest + iRGBCnt) = (int)((*((BYTE *)bypCopyDest + iRGBCnt) + 0) * dMultiplyValue);
				}
				else if (0xffffc8 == (*dwpSprite & 0x00ffffff)) {
					// 매우 어두운 색
					if (iRGBValue < 40) {
						if (bReviseFlag) {
							dMultiplyValue = 1.2;
						}
						else {
							dMultiplyValue = 2;
						}
					}
					// 어두컴컴
					else if (iRGBValue < 80) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.5;
						}
					}
					// 밝음
					else if (iRGBValue < 120) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.3;
						}
					}
					// 조금 밝음
					else if (iRGBValue < 160) {
						if (bReviseFlag) {
							dMultiplyValue = 1.1;
						}
						else {
							dMultiplyValue = 1.2;
						}
					}
					// 매우 밝음
					else {
						dMultiplyValue = 1;
					}

					*((BYTE *)bypCopyDest + iRGBCnt) = (int)((*((BYTE *)bypCopyDest + iRGBCnt) + 0) * dMultiplyValue);
				}

				++iRGBCnt;
			}

			//if (0xffff00 == (*dwpSprite & 0x00ffffff)) {
			//	// 매우 어두운 색
			//	if (iRGBValue < 40) {
			//		dMultiplyValue = 4;
			//	}
			//	// 어두컴컴
			//	else if (iRGBValue < 80) {
			//		dMultiplyValue = 2.5;
			//	}
			//	// 밝음
			//	else if (iRGBValue < 120) {
			//		dMultiplyValue = 1.7;
			//	}
			//	// 조금 밝음
			//	else if (iRGBValue < 160) {
			//		dMultiplyValue = 1.3;
			//	}
			//	// 매우 밝음
			//	else {
			//		dMultiplyValue = 1;
			//	}

			//	*((BYTE *)bypCopyDest + 2) = (int)((*((BYTE *)bypCopyDest + 2) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 1) = (int)((*((BYTE *)bypCopyDest + 1) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 0) = (int)((*((BYTE *)bypCopyDest + 0) + 0) * dMultiplyValue);
			//}
			//else if (0xffff64 == (*dwpSprite & 0x00ffffff)) {
			//	// 매우 어두운 색
			//	if (iRGBValue < 40) {
			//		dMultiplyValue = 3;
			//	}
			//	// 어두컴컴
			//	else if (iRGBValue < 80) {
			//		dMultiplyValue = 1.7;
			//	}
			//	// 밝음
			//	else if (iRGBValue < 120) {
			//		dMultiplyValue = 1.5;
			//	}
			//	// 조금 밝음
			//	else if (iRGBValue < 160) {
			//		dMultiplyValue = 1.3;
			//	}
			//	// 매우 밝음
			//	else {
			//		dMultiplyValue = 1;
			//	}

			//	*((BYTE *)bypCopyDest + 2) = (int)((*((BYTE *)bypCopyDest + 2) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 1) = (int)((*((BYTE *)bypCopyDest + 1) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 0) = (int)((*((BYTE *)bypCopyDest + 0) + 0) * dMultiplyValue);
			//}
			//else if (0xffffc8 == (*dwpSprite & 0x00ffffff)) {
			//	// 매우 어두운 색
			//	if (iRGBValue < 40) {
			//		dMultiplyValue = 2;
			//	}
			//	// 어두컴컴
			//	else if (iRGBValue < 80) {
			//		dMultiplyValue = 1.5;
			//	}
			//	// 밝음
			//	else if (iRGBValue < 120) {
			//		dMultiplyValue = 1.3;
			//	}
			//	// 조금 밝음
			//	else if (iRGBValue < 160) {
			//		dMultiplyValue = 1.2;
			//	}
			//	// 매우 밝음
			//	else {
			//		dMultiplyValue = 1;
			//	}

			//	*((BYTE *)bypCopyDest + 2) = (int)((*((BYTE *)bypCopyDest + 2) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 1) = (int)((*((BYTE *)bypCopyDest + 1) + 0) * dMultiplyValue);
			//	*((BYTE *)bypCopyDest + 0) = (int)((*((BYTE *)bypCopyDest + 0) + 0) * dMultiplyValue);
			//}

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

bool ObjectLightDib::Click()
{
	return false;
}

void ObjectLightDib::ResizeMap(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey)
{
	ReleaseDibBuffer(true);
	CreateDibBuffer(iMapWidth, iMapHeight, iColorBit, dwColorKey);
}

// 조명 스프라이트 생성
void ObjectLightDib::CreateLight(int iSpriteIndex, int iDrawX, int iDrawY, int iIncreaseValue)
{
	int iCountY;
	int iCountX;
	int iSpriteHeight = g_cSprite->m_stpSprite[iSpriteIndex].iHeight * iIncreaseValue;
	int iSpriteWidth = g_cSprite->m_stpSprite[iSpriteIndex].iWidth * iIncreaseValue;
	int iSpritePitch = g_cSprite->m_stpSprite[iSpriteIndex].iPitch;
	int iSpriteCenterX = g_cSprite->m_stpSprite[iSpriteIndex].iCenterPointX *  iIncreaseValue;
	int iSpriteCenterY = g_cSprite->m_stpSprite[iSpriteIndex].iCenterPointY *  iIncreaseValue;
	DWORD *dwpSprite = (DWORD *)g_cSprite->m_stpSprite[iSpriteIndex].bypImage;
	DWORD *dwpDest = (DWORD *)m_bypBuffer;
	BYTE *bypTempSprite;
	BYTE *bypTempDest;
	int iclippingY;
	int iclippingX;
	int iSpriteCntX = 1;	// 다음 스프라이트 RGB로 가기위한 카운트
	int iSpriteCntY = 1;	// 다음 스프라이트 RGB로 가기위한 카운트
	int iSpriteRgbSum;			// RGB 합
	int iDestRgbSum;			// RGB 합
	BYTE *bypCopySprite;
	BYTE *bypCopyDest;

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

	// 2018.02.08
	// 전체 맵 기준으로 스프라이트 값 클리핑 처리
	if (iDrawY < 0) {
		//2018.02.08
		iclippingY = -iDrawY;
		iSpriteHeight = iSpriteHeight - iclippingY;
		if (iSpriteHeight > 0) {
			// 나머지가 있으면 그 부분부터 카운트 함
			iSpriteCntY = (iclippingY % iIncreaseValue) + 1;
			// 실제 잘려지는 값은 원본 크기 값 비율로 바꿔서 잘라줌
			dwpSprite = (DWORD *)((BYTE *)dwpSprite + iSpritePitch * (iclippingY / iIncreaseValue));
		}
		iDrawY = 0;
	}

	if (iDrawY + iSpriteHeight >= m_iHeight) {
		iSpriteHeight = iSpriteHeight - ((iDrawY + iSpriteHeight) - m_iHeight);
	}

	if (iDrawX < 0) {
		//2018.02.08
		iclippingX = -iDrawX;
		iSpriteWidth = iSpriteWidth - iclippingX;
		if (iSpriteWidth > 0) {
			// 나머지가 있으면 그 부분부터 카운트 함
			iSpriteCntX = (iclippingX % iIncreaseValue) + 1;
			// 실제 잘려지는 값은 원본 크기 값 비율로 바꿔서 잘라줌
			dwpSprite = dwpSprite + (iclippingX / iIncreaseValue);
		}
		iDrawX = 0;
	}

	if (iDrawX + iSpriteWidth >= m_iWidth) {
		iSpriteWidth = iSpriteWidth - ((iDrawX + iSpriteWidth) -  m_iWidth);
	}

	if (iSpriteWidth <= 0 || iSpriteHeight <= 0) {
		return;
	}

	//-----------------------------------------------------------------
	// 화면에 찍을 위치로 이동한다.
	//-----------------------------------------------------------------
	dwpDest = (DWORD *)((BYTE *)(dwpDest + iDrawX) + (iDrawY * m_iPitch));
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
				bypCopySprite = (BYTE *)dwpSprite;
				bypCopyDest = (BYTE *)dwpDest;
				// 스프라이트 색깔
				iSpriteRgbSum = *((BYTE *)bypCopySprite + 2) + *((BYTE *)bypCopySprite + 1) + *((BYTE *)bypCopySprite + 0);
				// 기존 메모리 색깔
				iDestRgbSum = *((BYTE *)bypCopyDest + 2) + *((BYTE *)bypCopyDest + 1) + *((BYTE *)bypCopyDest + 0);

				// 더 값이 낮으면 (밝은색)
				if (iSpriteRgbSum < iDestRgbSum) {
					*dwpDest = *dwpSprite;
				}	
			}

			//다음칸 이동
			++dwpDest;

			if (iSpriteCntX < iIncreaseValue) {
				++iSpriteCntX;
			}
			else {
				++dwpSprite;
				iSpriteCntX = 1;
			}
		}
		//-----------------------------------------------------------------
		// 다음줄로 이동, 화면과 스프라이트 모두..
		//-----------------------------------------------------------------
		bypTempDest = bypTempDest + m_iPitch;

		if (iSpriteCntY < iIncreaseValue) {
			++iSpriteCntY;
		}
		else {
			bypTempSprite = bypTempSprite + iSpritePitch;
			iSpriteCntY = 1;
		}

		dwpDest = (DWORD *)bypTempDest;
		dwpSprite = (DWORD *)bypTempSprite;
	}
}

// 조명 스프라이트 지우기 (지우고 다시 남은 조명 스프라이트들을 메모리에 그림)
void ObjectLightDib::DeleteLight(ObjectLight * DeleteObjectLightPtr)
{
	CList<ObjectLight*>::iterator iter;
	CList<ObjectLight*>::iterator iter_end;
	ObjectLight * ObjectLightTemp;

	// 메모리 초기화
	memset(m_bypBuffer, 0xffffff, m_iBufferSize);

	iter = g_ObjectLightList->begin();
	iter_end = g_ObjectLightList->end();
	while (iter != iter_end) {
		ObjectLightTemp = *iter;

		// 삭제할 조명인가?
		if (ObjectLightTemp == DeleteObjectLightPtr) {
			iter = g_ObjectLightList->erase(iter);
			delete ObjectLightTemp;
			continue;
		}

		ObjectLightTemp->Draw();
		++iter;
	}
}

void ObjectLightDib::ClearLightBuffer()
{
	CList<ObjectLight*>::iterator iter;
	CList<ObjectLight*>::iterator iter_end;
	ObjectLight * ObjectLightTemp;

	// 조명 리스트 정리
	iter = g_ObjectLightList->begin();
	iter_end = g_ObjectLightList->end();
	while (iter != iter_end) {
		ObjectLightTemp = *iter;
		iter = g_ObjectLightList->erase(iter);
		// 조명 삭제
		delete ObjectLightTemp;
	}
}

void ObjectLightDib::CreateDibBuffer(int iMapWidth, int iMapHeight, int iColorBit, DWORD dwColorKey)
{
	m_iType = df_TYPE_LIGHT;

	m_dwColorKey = dwColorKey;

	m_iWidth = iMapWidth;
	m_iHeight = iMapHeight;
	m_iColorBit = iColorBit;

	m_iPitch = (iMapWidth * (iColorBit / 8) + 3) & ~3;
	m_iBufferSize = m_iPitch * m_iHeight;

	m_bypBuffer = (BYTE*)malloc(m_iBufferSize);

	// 메모리 초기화
	memset(m_bypBuffer, 0xffffff, m_iBufferSize);
}

void ObjectLightDib::ReleaseDibBuffer(bool bResizeFlag)
{
	CList<ObjectLight*>::iterator iter;
	CList<ObjectLight*>::iterator iter_end;
	ObjectLight * ObjectLightTemp;
	
	// 메모리 정리
	free(m_bypBuffer);
	
	// 라이트 객체는 정리 안함
	if (bResizeFlag) {
		return;
	}

	// 조명 리스트 정리
	iter = g_ObjectLightList->begin();
	iter_end = g_ObjectLightList->end();
	while (iter != iter_end) {
		ObjectLightTemp = *iter;
		iter = g_ObjectLightList->erase(iter);
		// 조명 삭제
		delete ObjectLightTemp;
	}
}
