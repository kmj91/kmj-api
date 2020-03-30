#include "stdafx.h"
#include "ObjectLight.h"


ObjectLight::ObjectLight(int iSpriteIndex, int iPosX, int iPosY, int iIncreaseValue)
{
	// 타입
	m_iType = df_TYPE_LIGHT;
	// 위치
	m_iX = iPosX;
	m_iY = iPosY;
	// 스프라이트 번호
	m_iSpriteIndex = iSpriteIndex;
	// 오브젝트 범위
	m_iLeft = m_iX - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointX * iIncreaseValue);
	m_iTop = m_iY - (g_cSprite->m_stpSprite[m_iSpriteIndex].iCenterPointY * iIncreaseValue);
	m_iRight = m_iLeft + g_cSprite->m_stpSprite[m_iSpriteIndex].iWidth * iIncreaseValue;
	m_iBottom = m_iTop + g_cSprite->m_stpSprite[m_iSpriteIndex].iHeight * iIncreaseValue;
	// 그리기 우선순위
	m_iRenderPriority = iPosY;
	// 곱셈 값
	m_iIncreaseValue = iIncreaseValue;


	// OBjectLightDib 에 해당 조명 스프라이트를 그린다
	Draw();
}


ObjectLight::~ObjectLight()
{
}

bool ObjectLight::Action()
{
	return false;
}


void ObjectLight::Draw()
{
	// OBjectLightDib 에 해당 조명 스프라이트를 생성
	g_ObjectLightDib->CreateLight(m_iSpriteIndex, m_iX, m_iY, m_iIncreaseValue);
}

bool ObjectLight::Click()
{
	return false;
}
