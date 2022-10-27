#pragma once

#include "MySprite.h"

class CMyMapSprite : public CMySprite
{
protected:
	RECT m_rcFrame;

public:
	CMyMapSprite();
	virtual ~CMyMapSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);
};

