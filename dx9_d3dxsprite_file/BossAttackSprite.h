#pragma once

#include "MySprite.h"

#define MAX_ANI					2
#define MAX_ANI_FRAME     13

class CBossAttackSprite : public CMySprite
{
protected:
	RECT m_rcFrame[MAX_ANI][MAX_ANI_FRAME];
	unsigned char  m_maxAni;
	unsigned short m_nFrame;
	float m_interval;

	int m_infoFrame[MAX_ANI];
	unsigned char m_aniType;

	float m_fDuration[MAX_ANI][MAX_ANI_FRAME];

	int xPos[3], yPos;

public:
	bool isAttack1, isAttack2;
	RECT  m_rcCollision[3];

	CBossAttackSprite();
	virtual ~CBossAttackSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

