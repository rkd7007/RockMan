#pragma once

#include "MySprite.h"

class CMyReadyStartSprite : public CMySprite
{
protected:
	RECT m_rcFrame[2];
	unsigned short m_nFrame;
	float m_interval;

	int xPos, yPos;

public:
	CMyReadyStartSprite();
	virtual ~CMyReadyStartSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

