#pragma once

#include "MySprite.h"

#define MAX_ANI					8
#define MAX_ANI_FRAME     21

class CBossAnimationSprite : public CMySprite
{
protected:
	RECT m_rcFrame[MAX_ANI][MAX_ANI_FRAME];
	unsigned char  m_maxAni;
	unsigned short m_nFrame;
	float m_interval;
	float m_stayinterval;
	float m_randAninterval;

	int m_infoFrame[MAX_ANI];
	unsigned char m_aniType;

	float m_fDuration[MAX_ANI][MAX_ANI_FRAME];

	int xPos, yPos; //x, yÁÂÇ¥
	int Hp; //Ã¼·Â
	int hitMax;

	bool stayBoss;
	bool isStart;
	bool isEnd;
	bool isDie;
	bool isHit;

	RECT  m_rcCollision;

public:
	bool isAttack1, isAttack2;

	CBossAnimationSprite();
	virtual ~CBossAnimationSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);

	void ChangeBossHp(int hp) { Hp = hp; }
	void Stay() { stayBoss = true; }
	void End(bool key) { isEnd = true; isDie = key; }
	void Hit() { isHit = true; }
	void HitAni();

	BOOL BossHp(int hp)
	{
		if (hp == 80 && Hp == 80 || hp == 60 && Hp == 60 || hp == 40 && Hp == 40 || hp == 20 && Hp == 20 || hp == 0 && Hp == 0)	return true;
		else return false;
	}

	BOOL isTrigger(RECT rect);
};

