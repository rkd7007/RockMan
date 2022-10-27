#include "MySprite.h"

CMySprite::CMySprite()
{
	for (int i = 0; i < MAX_SPRITE; i++)
	{
		m_pDonutTexture[i] = NULL;
		m_pDonutSprite[i] = NULL;
	}
}


CMySprite::~CMySprite()
{
	for (int i = 0; i < MAX_SPRITE; i++)
	{
		if (m_pDonutSprite[i] != NULL)
			m_pDonutSprite[i]->Release();

		if (m_pDonutTexture[i] != NULL)
			m_pDonutTexture[i]->Release();
	}
}

void CMySprite::Create(LPDIRECT3DDEVICE9 pDevice, char *filename)
{
	D3DXIMAGE_INFO d3dxImageInfo;
	for (int i = 0; i < MAX_SPRITE; i++)
	{
		if (i == 0) //map
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				1026, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				600, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
		if (i == 1) //player
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				1500, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				501, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
		if (i == 2) //player, boss hp
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				250, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				402, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 0.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
		if (i == 3) //Ready Start
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				600, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				400, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
		if (i == 4) //boss
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				4536, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				3045, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
		if (i == 5) //boss Attack
		{
			D3DXCreateTextureFromFileEx(pDevice,
				filename,
				750, // I had to set width manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1200, // I had to set height manually. D3DPOOL_DEFAULT works for textures but causes problems for D3DXSPRITE.
				1,   // Don't create mip-maps when you plan on using D3DXSPRITE. It throws off the pixel math for sprite animation.
				D3DPOOL_DEFAULT,
				D3DFMT_UNKNOWN,
				D3DPOOL_DEFAULT,
				D3DX_DEFAULT,
				D3DX_DEFAULT,
				D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f),
				&d3dxImageInfo,
				NULL,
				&m_pDonutTexture[i]);
		}
	}

	pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);

	for (int i = 0; i < MAX_SPRITE; i++)
		D3DXCreateSprite(pDevice, &m_pDonutSprite[i]);
}