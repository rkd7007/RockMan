#pragma once

#include "MySprite.h"

#define MAX_ANI					9
#define MAX_ANI_FRAME     12

class CMyAnimationSprite : public CMySprite
{
protected:
	RECT m_rcFrame[MAX_ANI][MAX_ANI_FRAME];
	unsigned char  m_maxAni;
	unsigned short m_nFrame;
	float m_interval;
	float m_stayinterval;
	float m_jumpinterval;
	float m_hitinterval;

	int m_infoFrame[MAX_ANI];
	unsigned char m_aniType;

	float m_fDuration[MAX_ANI][MAX_ANI_FRAME];

	bool isUp, isDown, isLeft, isRight, isSpace, isShift; //누른 키 체크
	bool stayPlayer, movePlayer;
	bool isJump; //점프 중인지 아닌지 체크
	bool isHit, aniHit;
	bool isEnd;
	bool isDie;

	int Hp; //체력
	int JumpSpeed; //점프 속도
	int max, min;
	int hitMax;
	int val;

	//캐릭터 콜라이더
	RECT  m_rcCollision;

public:
	int xPos, yPos; //x, y좌표

	CMyAnimationSprite();
	virtual ~CMyAnimationSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char *filename);
	void OnDraw();
	void OnUpdate(float time);

	int KeyDownUp(bool key, char name)
	{
		if (name == 'U') return isUp = key;
		if (name == 'D') return isDown = key;
		if (name == 'L') return isLeft = key;
		if (name == 'R') return isRight = key;
		if (name == 'S') return isSpace = key;
		if (name == 'A') return isShift = key;
	}

	void ChangeAnitype(int type) { m_aniType = type; }
	void ChangePlayerHp(int hp) { Hp = hp; }
	void Stay() { stayPlayer = true; }
	void Jumping() { isJump = true; };
	void Jump();
	void Attack();
	void Hit();
	void HitStay() { isHit = true; aniHit = true; }
	void End(bool key) { isEnd = true; isDie = key; }

	BOOL JumpCheck()
	{
		if (isJump) return true;
		else return false;
	}

	BOOL PlayerHp(int hp)
	{
		if (hp == 80 && Hp == 80 || hp == 60 && Hp == 60 || hp == 40 && Hp == 40 || hp == 20 && Hp == 20 || hp == 0 && Hp == 0)	return true;
		else return false;
	}

	BOOL ReadyGo()
	{
		if (movePlayer) return true;
		else return false;
	}

	BOOL HitTimer()
	{
		if (isHit) return true;
		else return false;
	}

	BOOL isTrigger(RECT rect);
};

