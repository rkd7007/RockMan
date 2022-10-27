#pragma once

#include "MySprite.h"

class CMyPlayerHpSprite : public CMySprite
{
protected:
	RECT m_rcFrame[6];
	unsigned short m_nFrame;
	bool isHp_80, isHp_60, isHp_40, isHp_20, isHp_0;

public:
	CMyPlayerHpSprite();
	virtual ~CMyPlayerHpSprite();

	void Create(LPDIRECT3DDEVICE9 pDevice, char* filename);
	void OnDraw();
	void OnUpdate(float time);

	void ChangeHpSprite(int hp)
	{
		if (hp == 80) isHp_80 = true;
		else if (hp == 60) { isHp_60 = true; isHp_80 = false; }
		else if (hp == 40) { isHp_40 = true; isHp_60 = false; isHp_80 = false; }
		else if (hp == 20) { isHp_20 = true; isHp_40 = false; isHp_60 = false; isHp_80 = false; }
		else if (hp == 0) { isHp_0 = true; isHp_20 = false; isHp_40 = false; isHp_60 = false; isHp_80 = false; }
	}
};

