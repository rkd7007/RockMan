#include "MyReadyStartSprite.h"
#include <stdio.h>

CMyReadyStartSprite::CMyReadyStartSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	xPos = 600;
	yPos = 150;
}

CMyReadyStartSprite::~CMyReadyStartSprite()
{
}

void CMyReadyStartSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame[0], 0, 0, 600, 215);
	SetRect(&m_rcFrame[1], 0, 215, 600, 215);
}

void CMyReadyStartSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(xPos, yPos, 0.0f);

	m_pDonutSprite[3]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[3]->Draw(m_pDonutTexture[3],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[3]->End();
}

void CMyReadyStartSprite::OnUpdate(float time)
{
	m_interval += time;

	if (m_interval > 0.1f)
	{
		m_interval = 0.0f;
		xPos -= 70;

		if (xPos <= -500)
		{
			m_nFrame = 1;
			xPos = 600;
			yPos = 200;
		}
	}

	OnDraw();
}