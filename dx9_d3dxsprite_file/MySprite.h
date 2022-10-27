#pragma once

#include <d3d9.h>
#include <d3dx9.h>

#define MAX_SPRITE			6

class CMySprite
{
protected:
	LPDIRECT3DTEXTURE9 m_pDonutTexture[MAX_SPRITE];
	LPD3DXSPRITE       m_pDonutSprite[MAX_SPRITE];

public:
	CMySprite();
	virtual ~CMySprite();

	virtual void Create(LPDIRECT3DDEVICE9 pDevice, char *filename);
	virtual void OnDraw() {}
	virtual void OnUpdate(float time) {}
};

