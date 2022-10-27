#include "MyAnimationSprite.h"
#include <stdio.h>

/*
* Animation
* 0 : ����, 1 : idle, 2 : r move, 3 : jump, 4 : l move, 5 : attack, 6 : hit, 7 : die, 8 : win
*/

CMyAnimationSprite::CMyAnimationSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;
	m_stayinterval = 0.0f;
	m_jumpinterval = 0.0f;
	m_hitinterval = 0.0f;
	m_aniType = 0;

	xPos = 70;
	yPos = 250;
	Hp = 100;
	JumpSpeed = 15;
	max, min = 0;
	hitMax = 0;
	val = -1;

	isUp, isDown, isLeft, isRight, isSpace, isShift, stayPlayer, movePlayer, isJump, isHit, aniHit, isEnd, isDie = false;
}

CMyAnimationSprite::~CMyAnimationSprite()
{
}

void CMyAnimationSprite::Create(LPDIRECT3DDEVICE9 pDevice, char *filename)
{
	FILE *fp = fopen(filename, "rt");
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

void CMyAnimationSprite::OnDraw()
{
	RECT srcRect = m_rcFrame[m_aniType][m_nFrame];
	srcRect.bottom = srcRect.top + m_rcFrame[m_aniType][m_nFrame].bottom;
	srcRect.right = srcRect.left + m_rcFrame[m_aniType][m_nFrame].right;

	D3DXVECTOR3 vCenter(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 vPosition(xPos, yPos, 0.0f);

	m_rcCollision.left = (int)xPos;
	m_rcCollision.top = (int)yPos;
	m_rcCollision.right = m_rcCollision.left + m_rcFrame[m_aniType][m_nFrame].right;
	m_rcCollision.bottom = m_rcCollision.top + m_rcFrame[m_aniType][m_nFrame].bottom;

	m_pDonutSprite[1]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[1]->Draw(m_pDonutTexture[1],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[1]->End();
}

void CMyAnimationSprite::OnUpdate(float time)
{
	m_interval += time;

	if (m_interval > m_fDuration[m_aniType][m_nFrame])
	{
		m_interval = 0.0f;
		m_nFrame++;
		if (m_nFrame >= m_infoFrame[m_aniType])
		{
			m_nFrame = 0;

			//�׾�����
			if (m_aniType == 7)
				m_nFrame = 5;
		}
		if (isUp) { yPos -= 8; m_aniType = 2; }
		if (isDown)	{ yPos += 8; m_aniType = 2; }
		if (isLeft) { xPos -= 8; m_aniType = 4; }
		if (isRight) { xPos += 8; m_aniType = 2; }
		if (isSpace)
		{
			m_aniType = 3;
			m_nFrame = 0;
			max = yPos - 40;
			min = yPos;
			isSpace = false;
		}
		if (isShift)
		{
			m_aniType = 5;
			m_nFrame = 0;
			isShift = false;
		}
		if (aniHit)
		{
			m_aniType = 6;
			m_nFrame = 0;
			hitMax = xPos - 30;
			aniHit = false;
		}
		//������ �� �ִ� ����
		if (xPos < 0) xPos = 0;
		if (xPos > 600) xPos = 600;
		if (yPos < 110) yPos = 110;
		if (yPos > 340) yPos = 340;

		//Animation
		Jump();
		Attack();
		Hit();
	}

	//�ݺ� ���� �� �ǵ��� ��
	if (isJump)
	{
		m_jumpinterval += time;
		if (m_jumpinterval >= 0.9f)
		{
			m_jumpinterval = 0.0f;
			isJump = false;
		}
	}

	//�ΰ��ӿ� �����ϰ� ī��Ʈ �ٿ� ����
	if (stayPlayer)
	{
		m_stayinterval += time;
		if (m_stayinterval >= 0.9f) m_aniType = 1;
		//3�ʵ��� ��� �� (ī��Ʈ �ٿ��� ���� ��) ������ �� �ֵ��� �Ѵ�
		if (m_stayinterval >= 3.0f)
		{
			movePlayer = true;
			stayPlayer = false;
		}
	}
	
	//�ݶ��̴� üũ�� �������� ���� �ʵ��� hit check timer ����
	if (isHit)
	{
		m_hitinterval += time;
		if (m_hitinterval >= 2.0f)
		{
			m_hitinterval = 0.0f;
			isHit = false;
		}
	}

	//playe�� �׾�����
	if (isEnd && isDie)
		m_aniType = 7;

	//boss�� �׾�����
	if (isEnd && !isDie)
		m_aniType = 8;

	OnDraw();
}

void CMyAnimationSprite::Jump()
{
	if (m_aniType == 3)
	{
		yPos += (JumpSpeed * val);
		//�ش� ��ǥ���� �ö����� �ٽ� ��������
		if (yPos <= max) val *= -1;
		//���� ��ǥ���� ����������
		if (min == yPos)
		{
			//�ִϸ��̼� idle�� ����
			val = -1;
			m_aniType = 1;
			m_nFrame = 0;
		}
	}
}

void CMyAnimationSprite::Attack()
{
	if (m_aniType == 5)
	{
		if (m_nFrame == 1)
		{
			m_aniType = 1;
			m_nFrame = 0;
		}
	}
}

void CMyAnimationSprite::Hit()
{
	if (m_aniType == 6)
	{
		xPos -= 10;

		if (xPos <= hitMax)
		{
			m_aniType = 1;
			m_nFrame = 0;
		}
	}
}

BOOL CMyAnimationSprite::isTrigger(RECT rect)
{
	if (m_rcCollision.left < rect.right &&
		m_rcCollision.top < rect.bottom &&
		m_rcCollision.right > rect.left &&
		m_rcCollision.bottom > rect.top)
		return true;
	return false;
}