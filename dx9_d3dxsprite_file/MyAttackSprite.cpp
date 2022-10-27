#include "MyAttackSprite.h"
#include <stdio.h>

CMyAttackSprite::CMyAttackSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;

	xPos = 0;
	yPos = -100;

	isAttack = false;
}

CMyAttackSprite::~CMyAttackSprite()
{
}

void CMyAttackSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame[0], 228, 367, 30, 27);
	SetRect(&m_rcFrame[1], 259, 365, 30, 27);
	SetRect(&m_rcFrame[2], 290, 369, 65, 37);
}

void CMyAttackSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(xPos, yPos + 20, 0.0f);

	m_rcCollision.left = (int)xPos;
	m_rcCollision.top = (int)yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_nFrame].bottom;

	m_pDonutSprite[1]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[1]->Draw(m_pDonutTexture[1],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[1]->End();
}

void CMyAttackSprite::OnUpdate(float time)
{
	m_interval += time;

	if (isAttack)
	{
		if (m_interval > 0.2f)
		{
			m_interval = 0.0f;
			xPos += 80;

			if (m_nFrame == 2) m_nFrame = 2;
			else
				m_nFrame++;

			if (xPos > 1024)
			{
				xPos = -100;
				m_nFrame = 0;
				isAttack = false;
			}
		}
	}
	else
		xPos = -100;

	OnDraw();
}