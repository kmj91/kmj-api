
#include "Object.h"
class ObjectSprite : public Object
{
public:
	ObjectSprite(int iSpriteIndex, int iPosX, int iPosY, bool bCameraOption);
	virtual ~ObjectSprite();

	virtual bool Action();
	virtual void Draw();
	virtual bool Click();

private:
	int m_iSpriteIndex;

	double m_dLeft;
	double m_dRight;
	double m_dTop;
	double m_dBottom;

	bool m_bCameraOption;
};
