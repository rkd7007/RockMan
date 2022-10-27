#include "BossHpSprite.h"
#include <stdio.h>

CBossHpSprite::CBossHpSprite()
{
	m_nFrame = 0;
	isHp_80, isHp_60, isHp_40, isHp_20, isHp_0 = false;
}

CBossHpSprite::~CBossHpSprite()
{
}

void CBossHpSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	CMySprite::Create(pDevice, filename);
	SetRect(&m_rcFrame[0], 0, 0, 250, 67); // hp = 100
	SetRect(&m_rcFrame[1], 0, 67, 250, 67); // hp = 80
	SetRect(&m_rcFrame[2], 0, 134, 250, 67); // hp = 60
	SetRect(&m_rcFrame[3], 0, 201, 250, 67); // hp = 40
	SetRect(&m_rcFrame[4], 0, 268, 250, 67); // hp = 20
	SetRect(&m_rcFrame[5], 0, 335, 250, 67); // hp = 0
}

void CBossHpSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(750.0f, 0.0f, 0.0f);

	m_pDonutSprite[2]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[2]->Draw(m_pDonutTexture[2],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[2]->End();
}

void CBossHpSprite::OnUpdate(float time)
{
	if (isHp_80) m_nFrame = 1;
	else if (isHp_60) m_nFrame = 2;
	else if (isHp_40) m_nFrame = 3;
	else if (isHp_20) m_nFrame = 4;
	else if (isHp_0) m_nFrame = 5;
	else m_nFrame = 0;

	OnDraw();
}