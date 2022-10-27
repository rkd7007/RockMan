#pragma once

#include "MySprite.h"

class CMyGameEndSprite : public CMySprite
{
protected:
	RECT m_rcFrame[2];
	unsigned short m_nFrame;

	bool isWin;

public:
	CMyGameEndSprite();
	virtual ~CMyGameEndSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);

	void bWin() { isWin = true; };
};

