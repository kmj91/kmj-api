#include "stdafx.h"
#include "UnitLocation.h"


UnitLocation::UnitLocation(int iPosX, int iPosY)
{
	int iCnt;
	int ierror;
	wchar_t szStr[256];

	// 타입
	m_iType = df_TYPE_LOCATION;
	m_iUnitType = df_UNIT_TYPE_LOCATION_RECTANGLE;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	//-------------------------------------
	// 로케이션은 스프라이트가 없습니다
	// 직접 사각형을 그립니다
	// 스프라이트 번호
	//-------------------------------------
	m_iSpriteIndex = df_SPRITE_EMPT_SPRITE;
	// 가로, 세로 길이
	m_iWidth = df_DEFAULT_TILE_SIZE;
	m_iHeight = df_DEFAULT_TILE_SIZE;
	// 오브젝트 범위
	m_iLeft = m_iX;
	m_iTop = m_iY;
	m_iRight = m_iLeft + m_iWidth;
	m_iBottom = m_iTop + m_iHeight;
	// 그리기 우선순위
	m_iRenderPriority = iPosY;
	// 지우기 플래그
	m_bEraseFlag = false;
	// 선택 플래그
	m_bSelectFlag = false;
	// 시작 스프라이트
	m_iStartSprite = df_SPRITE_EMPT_SPRITE;
	m_iEndSprite = df_SPRITE_EMPT_SPRITE;
	m_iAnimeDelay = 0;
	m_iAnimeCount = 0;

	// 로케이션 고유 번호
	m_iLocationNumber = ++g_iLocationNumber;

	// 문자열화
	swprintf_s(szStr, 256, L"%d", m_iLocationNumber);

	// 버튼 이름 길이
	m_iNameSize = wcsnlen_s(szStr, 20);
	// 이름 저장할 메모리 동적 생성
	m_wchpName = (wchar_t *)malloc(m_iNameSize * 2);

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		m_wchpName[iCnt] = szStr[iCnt];
		++iCnt;
	}

	// 폰트 페이스 생성
	m_face = new FT_Face[m_iNameSize];

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		// 폰트 읽음
		ierror = FT_New_Face(g_library, "font\\malgun.ttf", 0, &m_face[iCnt]);
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_New_Face 실패"), TEXT("에러"), MB_OK);
		}

		// 크기 설정
		// 글리프가 저장된 페이스, 폭, 높이, 출력장비 해상도 가로, 세로
		ierror = FT_Set_Char_Size(m_face[iCnt], 20 * 64, 0, GetDeviceCaps(g_hdc, LOGPIXELSX), GetDeviceCaps(g_hdc, LOGPIXELSY));
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_Set_Char_Size 실패"), TEXT("에러"), MB_OK);
		}

		// 문자의 글리프 비트맵 구함
		ierror = FT_Load_Char(m_face[iCnt], m_wchpName[iCnt], FT_LOAD_RENDER | FT_LOAD_NO_BITMAP);
		if (ierror != 0) {
			// 에러 처리
			MessageBox(NULL, TEXT("FT_New_Face 실패"), TEXT("에러"), MB_OK);
			return;
		}

		++iCnt;
	}
}


UnitLocation::~UnitLocation()
{
	int iCnt;

	iCnt = 0;
	while (iCnt < m_iNameSize) {
		// 폰트 페이스 정리
		FT_Done_Face(m_face[iCnt]);
		++iCnt;
	}

	// 메모리 정리
	free(m_wchpName);
}

bool UnitLocation::Action()
{
	// 지우기 옵션
	if (m_bEraseFlag) {
		return true;
	}

	return false;
}

void UnitLocation::Draw()
{
	int iStrCnt;
	int iWidth;
	int iHeight;
	int iCntY;
	int iCntX;
	int penx;
	int peny;
	int ierror;
	int iX;
	int iY;
	DWORD dwColor;
	BYTE byColor;
	DWORD *dwpDest;
	BYTE *bypSprite;
	BYTE *bypTempDest;

	// 테두리
	g_cSprite->DrawRectangleBorder(0x00ff00, 2, m_iLeft, m_iTop, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, true);

	penx = 0 * 64;
	peny = 200 * 64;

	// 위치 좌표
	iX = m_iLeft - g_CameraRect.left + 10;
	iY = m_iTop - g_CameraRect.top + 10;

	// 번호
	iStrCnt = 0;
	while (iStrCnt < m_iNameSize) {
		// 화면으로 출력
		iWidth = m_face[iStrCnt]->glyph->bitmap.width;
		iHeight = m_face[iStrCnt]->glyph->bitmap.rows;

		// 문자 버퍼
		dwpDest = (DWORD *)g_bypDest;
		dwpDest = (DWORD *)((BYTE *)(dwpDest + iX + (penx >> 6)) + (iY * g_iDestPitch));
		bypSprite = m_face[iStrCnt]->glyph->bitmap.buffer;
		bypTempDest = (BYTE *)dwpDest;

		iCntY = 0;
		while (iCntY < iHeight) {
			iCntX = 0;
			while (iCntX < iWidth) {
				// 값이 클수록 검정색을 나타낸다
				//dwColor = 255 - *bypSprite;
				dwColor = *bypSprite;
				if (dwColor == 255) {
					*dwpDest = RGB(dwColor, dwColor, dwColor);
				}
				else if (dwColor != 0) {
					dwColor = *bypSprite;
					*((BYTE *)dwpDest + 2) = (dwColor * 0.5) + (0xffffff * 0.5);
					*((BYTE *)dwpDest + 1) = (dwColor * 0.5) + (0xffffff * 0.5);
					*((BYTE *)dwpDest + 0) = (dwColor * 0.5) + (0xffffff * 0.5);
				}

				//다음칸 이동
				++dwpDest;
				++bypSprite;
				++iCntX;
			}
			//-----------------------------------------------------------------
			// 다음줄로 이동, 화면과 스프라이트 모두..
			//-----------------------------------------------------------------
			++iCntY;
			bypTempDest = bypTempDest + g_iDestPitch;
			dwpDest = (DWORD *)bypTempDest;
			bypSprite = &m_face[iStrCnt]->glyph->bitmap.buffer[iCntY * iWidth];

		}

		penx += m_face[iStrCnt]->glyph->advance.x;
		peny += m_face[iStrCnt]->glyph->advance.y;

		++iStrCnt;
	}

	// 선택되어있나?
	if (m_bSelectFlag) {
		g_cSprite->DrawRectangleBorder(0x1e96c8, 4, m_iLeft, m_iTop, m_iWidth, m_iHeight, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch, true);
	}
}

bool UnitLocation::Click()
{
	int iMouseX;
	int iMouseY;

	// 클릭 가능 상태가 아니면
	if (g_iMouseState != df_MOUSE_STATE_EMPT && g_iMouseState != df_MOUSE_STATE_SELECT_UNIT && g_iMouseState != df_MOUSE_STATE_SELECT_LOCATION) {
		return false;
	}


	//--------------------------------------
	// 전체 맵의 범위
	//--------------------------------------
	iMouseX = g_iMapMouseX;
	iMouseY = g_iMapMouseY;

	if (m_iLeft < iMouseX && iMouseX < m_iRight &&
		m_iTop < iMouseY && iMouseY < m_iBottom) {

		// 선택된 유닛이 있나?
		if (g_iMouseState == df_MOUSE_STATE_SELECT_LOCATION) {
			// 선택된 유닛 해제
			g_SelectedObjectUnit->m_bSelectFlag = false;
		}
		else {
			// 마우스 상태 변경
			g_iMouseState = df_MOUSE_STATE_SELECT_LOCATION;
		}

		// 선택됨
		m_bSelectFlag = true;
		// 내가 선택됨 내 포인터 저장
		g_SelectedObjectUnit = this;

		// 마우스 클릭 타입
		if (m_iLeft <= iMouseX && iMouseX < m_iLeft + 10) {
			g_iMouseClickObjectType = df_MOUSE_CLICK_UNIT_LOCATION_LEFT;
		}
		else if (m_iTop <= iMouseY && iMouseY < m_iTop + 10) {
			g_iMouseClickObjectType = df_MOUSE_CLICK_UNIT_LOCATION_TOP;
		}
		else if (m_iRight - 10 <= iMouseX && iMouseX < m_iRight) {
			g_iMouseClickObjectType = df_MOUSE_CLICK_UNIT_LOCATION_RIGHT;
		}
		else if (m_iBottom - 10 <= iMouseY && iMouseY < m_iBottom) {
			g_iMouseClickObjectType = df_MOUSE_CLICK_UNIT_LOCATION_BOTTOM;
		}
		else {
			g_iMouseClickObjectType = df_MOUSE_CLICK_UNIT_LOCATION;
		}

		return true;
	}

	return false;
}

// 마우스 클릭 드래그
void UnitLocation::Drag()
{
	int iMouseX;
	int iMouseY;
	int iDestX;
	int iDestY;

	//--------------------------------------
	// 전체 맵의 범위
	// 타일에 맞춰서
	//--------------------------------------
	iMouseX = g_iMapMouseX / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE;
	iMouseY = g_iMapMouseY / df_DEFAULT_TILE_SIZE * df_DEFAULT_TILE_SIZE;

	iDestX = iMouseX - m_iX;
	iDestY = iMouseY - m_iY;

	// 음수면
	if (iDestX < 0) {
		m_iWidth = (-iDestX);
		m_iLeft = iMouseX;
	}
	else if (iDestX == 0) {
		m_iWidth = df_DEFAULT_TILE_SIZE;
		m_iLeft = m_iX;
		m_iRight = m_iLeft + m_iWidth;
	}
	else {
		m_iWidth = iDestX;
		m_iRight = iMouseX;
		m_iLeft = m_iRight - m_iWidth;
	}

	if (iDestY < 0) {
		m_iHeight = (-iDestY);
		m_iTop = iMouseY;
	}
	else if (iDestY == 0) {
		m_iHeight = df_DEFAULT_TILE_SIZE;
		m_iTop = m_iY;
		m_iBottom = m_iTop + m_iHeight;
	}
	else {
		m_iHeight = iDestY;
		m_iBottom = iMouseY;
		m_iTop = m_iBottom - m_iHeight;
	}
}

void UnitLocation::Move(int iMoveValueX, int iMoveValueY)
{
	m_iX = m_iX - iMoveValueX;
	m_iY = m_iY - iMoveValueY;
	m_iLeft = m_iLeft - iMoveValueX;
	m_iTop = m_iTop - iMoveValueY;
	m_iRight = m_iRight - iMoveValueX;
	m_iBottom = m_iBottom - iMoveValueY;
}

void UnitLocation::ReSizeLeft(int iMoveValue)
{
	if (m_iLeft - iMoveValue >= m_iRight) {
		return;
	}

	m_iLeft = m_iLeft - iMoveValue;
	m_iWidth = m_iWidth + iMoveValue;
}

void UnitLocation::ReSizeTop(int iMoveValue)
{
	if (m_iTop - iMoveValue >= m_iBottom) {
		return;
	}

	m_iTop = m_iTop - iMoveValue;
	m_iHeight = m_iHeight + iMoveValue;
}

void UnitLocation::ReSizeRight(int iMoveValue)
{
	if (m_iRight - iMoveValue <= m_iLeft) {
		return;
	}
	m_iRight = m_iRight - iMoveValue;
	m_iWidth = m_iWidth - iMoveValue;
}

void UnitLocation::ReSizeBottom(int iMoveValue)
{
	if (m_iBottom - iMoveValue <= m_iTop) {
		return;
	}

	m_iBottom = m_iBottom - iMoveValue;
	m_iHeight = m_iHeight - iMoveValue;
}

