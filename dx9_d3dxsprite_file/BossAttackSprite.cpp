#include "BossAttackSprite.h"
#include <stdio.h>

CBossAttackSprite::CBossAttackSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;
	m_aniType = 0;

	xPos[0] = 70;
	xPos[1] = 300;
	xPos[2] = 550;
	yPos = -100;

	isAttack1, isAttack2 = false;
}

CBossAttackSprite::~CBossAttackSprite()
{
}

void CBossAttackSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
{
	FILE* fp = fopen(filename, "rt");
	if (fp == NULL)
		return;

	char imgname[128];

	fscanf(fp, "%s", imgname);
	CMySprite::Create(pDevice, imgname);

	fscanf(fp, "%d", &m_maxAni);

	for (int ani = 0; ani < m_maxAni; ani++)
	{
		int aniIndex, nFrame;
		fscanf(fp, "%d %d", &aniIndex, &nFrame);

		m_infoFrame[aniIndex] = nFrame;

		int left, top, width, height;
		float duration;
		for (int i = 0; i < nFrame; i++)
		{
			fscanf(fp, "%d %d %d %d %f", &left, &top, &width, &height, &duration);
			SetRect(&m_rcFrame[aniIndex][i], left, top, width, height);
			m_fDuration[aniIndex][i] = duration;
		}
	}

	fclose(fp);
}

void CBossAttackSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_aniType][m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_aniType][m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_aniType][m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition1(xPos[0], yPos, 0.0f);
	D3DXVECTOR3 vPosition2(xPos[1], yPos, 0.0f);
	D3DXVECTOR3 vPosition3(xPos[2], yPos, 0.0f);

	for (int i = 0; i < 3; i++)
	{
		m_rcCollision[i].left = (int)xPos[i];
		m_rcCollision[i].top = (int)yPos;
		m_rcCollision[i].right = m_rcCollision[i].left + m_rcFrame[m_aniType][m_nFrame].right;
		m_rcCollision[i].bottom = m_rcCollision[i].top + m_rcFrame[m_aniType][m_nFrame].bottom;
	}

	m_pDonutSprite[5]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[5]->Draw(m_pDonutTexture[5],
		&srcRect,
		&vCenter,
		&vPosition1,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[5]->Draw(m_pDonutTexture[5],
		&srcRect,
		&vCenter,
		&vPosition2,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[5]->Draw(m_pDonutTexture[5],
		&srcRect,
		&vCenter,
		&vPosition3,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[5]->End();
}

void CBossAttackSprite::OnUpdate(float time)
{
	m_interval += time;

	if (isAttack1)
	{
		xPos[0] = -200;
		xPos[1] = 135;
		xPos[2] = 380;
		yPos = 200;

		m_aniType = 0;
	}

	if (isAttack2)
	{
		xPos[0] = 70;
		xPos[1] = 300;
		xPos[2] = 550;
		yPos = 180;

		m_aniType = 1;
	}

	if (m_interval > m_fDuration[m_aniType][m_nFrame])
	{
		m_interval = 0.0f;
		m_nFrame++;
		if (m_nFrame >= m_infoFrame[m_aniType])
		{
			m_nFrame = 0;
			yPos = -100;
			isAttack1 = false;
			isAttack2 = false;
		}
	}

	OnDraw();
}