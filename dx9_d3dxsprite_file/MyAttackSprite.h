#pragma once

#include "MySprite.h"

class CMyAttackSprite : public CMySprite
{
protected:
	RECT m_rcFrame[3];
	unsigned short m_nFrame;
	float m_interval;

	int xPos, yPos;
	bool isAttack; //player가 공격 키를 눌렀으면 공격 나감

public:
	RECT  m_rcCollision;

	CMyAttackSprite();
	virtual ~CMyAttackSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);

	void AttackPostion(int x, int y) { xPos = x; yPos = y; };
	void PlayAttack() { isAttack = true; };
	void PlayerHit() { isAttack = false; };

	BOOL Attacking()
	{
		if (isAttack) return true;
		return false;
	}
};

