#include "stdafx.h"
#include "ObjectBackground.h"


ObjectBackground::ObjectBackground(int iPosX, int iPosY)
{
	m_Type = df_TYPE_OBJECT_BACKGROUND;
	m_X = iPosX;
	m_Y = iPosY;

	m_iSpriteIndex = df_SPRITE_STAGE_1;

	m_fLeft = 0;
	m_fRight = 0;
	m_fTop = 0;
	m_fBottom = 0;
}


ObjectBackground::~ObjectBackground()
{
}

bool ObjectBackground::Action()
{
	m_Y = m_Y + df_SPEED_SCROLL;

	return false;
}

void ObjectBackground::Draw()
{
	g_cSprite.DrawSprite(m_iSpriteIndex, m_X, m_Y, g_bypDest, g_iDestWidth, g_iDestHeight, g_iDestPitch);
}
