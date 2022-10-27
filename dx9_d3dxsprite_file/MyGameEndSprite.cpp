#include "MyGameEndSprite.h"
#include <stdio.h>

CMyGameEndSprite::CMyGameEndSprite()
{
	m_nFrame = 0;

	isWin = false;
}

CMyGameEndSprite::~CMyGameEndSprite()
{
}

void CMyGameEndSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame[0], 0, 0, 600, 215);
	SetRect(&m_rcFrame[1], 0, 215, 600, 215);
}

void CMyGameEndSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(200.0f, 150.0f, 0.0f);

	m_pDonutSprite[3]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[3]->Draw(m_pDonutTexture[3],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[3]->End();
}

void CMyGameEndSprite::OnUpdate(float time)
{
	if (isWin)
		m_nFrame = 0;
	else
		m_nFrame = 1;

	OnDraw();
}