#include "BossAnimationSprite.h"
#include <stdio.h>
#include <time.h>

/*
* Animation
* 0 : idle, 1 : l move, 2 : r move, 3 : attack_1, 4 : attack_2, 5 : hit, 6 : die, 7 : win
*/

CBossAnimationSprite::CBossAnimationSprite()
{
	m_nFrame = 0;
	m_interval = 0.0f;
	m_aniType = 0;
	m_stayinterval = 0.0f;
	m_randAninterval = 0.0f;

	xPos = 800;
	yPos = 200;
	Hp = 100;
	hitMax = 0;

	stayBoss, isAttack1, isAttack2, isStart, isEnd, isHit, isDie = false;

	srand(time(NULL));
}

CBossAnimationSprite::~CBossAnimationSprite()
{
}

void CBossAnimationSprite::Create(LPDIRECT3DDEVICE9 pDevice, char* filename)
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

void CBossAnimationSprite::OnDraw()
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

	m_pDonutSprite[4]->Begin(D3DXSPRITE_ALPHABLEND);

	m_pDonutSprite[4]->Draw(m_pDonutTexture[4],
		&srcRect,
		&vCenter,
		&vPosition,
		D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	m_pDonutSprite[4]->End();
}

void CBossAnimationSprite::OnUpdate(float time)
{
	m_interval += time;

	if (isStart)
		m_randAninterval += time;

	if (m_interval > m_fDuration[m_aniType][m_nFrame])
	{
		m_interval = 0.0f;
		m_nFrame++;
		if (m_nFrame >= m_infoFrame[m_aniType])
		{
			m_nFrame = 0;

			//죽었으면
			if (m_aniType == 6)
				m_nFrame = 4;
		}
		//왼쪽으로 움직임
		if (m_aniType == 1)
		{
			xPos -= 5;

			if (m_nFrame >= m_infoFrame[m_aniType] - 1)
			{
				m_nFrame = 0;
				m_aniType = 0;
			}
		}
		//오른쪽으로 움직임
		if (m_aniType == 2)
		{
			xPos += 5;

			if (m_nFrame >= m_infoFrame[m_aniType] - 1)
			{
				m_nFrame = 0;
				m_aniType = 0;
			}
		}
		//공격 1 - 물방울
		if (m_aniType == 3)
		{
			isAttack1 = true;

			if (m_nFrame >= m_infoFrame[m_aniType] - 1)
			{
				m_aniType = 0;
				m_nFrame = 0;
			}
		}
		//공격 2 - 물벼락
		if (m_aniType == 4)
		{
			isAttack2 = true;

			if (m_nFrame >= m_infoFrame[m_aniType] - 1)
			{
				m_aniType = 0;
				m_nFrame = 0;
			}
		}
		//공격에 맞았으면
		if (isHit)
		{
			m_aniType = 5;
			m_nFrame = 0;
			hitMax = xPos + 30;
			isHit = false;
		}

		//움직일 수 있는 영역
		if (xPos < 700) { xPos = 700; m_aniType = 0; }
		if (xPos > 900) { xPos = 900; m_aniType = 0; }

		HitAni();
	}

	//인게임에 돌입하고 카운트 다운 실행
	if (stayBoss)
	{
		m_stayinterval += time;
		//3초동안 대기 후 (카운트 다운이 끝난 후) 움직일 수 있도록 한다
		if (m_stayinterval >= 3.0f)
		{
			m_stayinterval = 0.0f;
			isStart = true;
			stayBoss = false;
		}
	}

	//애니메이션 랜덤으로 선택
	if (m_randAninterval >= 2.0f && !isEnd)
	{
		m_randAninterval = 0.0f;
		m_aniType = rand() % 4 + 1; // 1~4 (idle 제외)
	}

	//boss가 죽었으면
	if (isEnd && isDie)
		m_aniType = 6;

	//player가 죽었으면
	if (isEnd && !isDie)
	{
		yPos = -50;
		m_aniType = 7;
	}

	OnDraw();
}

void CBossAnimationSprite::HitAni()
{
	if (m_aniType == 5)
	{
		xPos += 10;

		if (xPos >= hitMax)
		{
			m_aniType = 1;
			m_nFrame = 0;
		}
	}
}

BOOL CBossAnimationSprite::isTrigger(RECT rect)
{
	if (m_rcCollision.left < rect.right &&
		m_rcCollision.top < rect.bottom &&
		m_rcCollision.right > rect.left &&
		m_rcCollision.bottom > rect.top)
		return true;
	return false;
}