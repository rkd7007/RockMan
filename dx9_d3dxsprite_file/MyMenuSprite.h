#pragma once

#include "MySprite.h"

class CMyMenuSprite : public CMySprite
{
protected:
	RECT m_rcFrame;

public:
	CMyMenuSprite();
	virtual ~CMyMenuSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

