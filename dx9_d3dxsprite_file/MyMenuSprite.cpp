#include "MyMenuSprite.h"
#include <stdio.h>

CMyMenuSprite::CMyMenuSprite()
{
}

CMyMenuSprite::~CMyMenuSprite()
{
}

void CMyMenuSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame, 0, 0, 1026, 600);
}

void CMyMenuSprite::OnDraw()
{
	RECT srcRect = m_rcFrame;
	srcRect.bottom = srcRect.top + m_rcFrame.bottom;
	srcRect.right = srcRect.left + m_rcFrame.right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(0.0f, 0.0f, 0.0f);

	m_pDonutSprite[0]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[0]->Draw(m_pDonutTexture[0],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[0]->End();
}

void CMyMenuSprite::OnUpdate(float time)
{
	OnDraw();
}