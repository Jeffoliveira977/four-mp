#pragma once

#include "CGUI.h"

class CTexture
{
	IDirect3DTexture9 * m_pTexture;
	D3DSURFACE_DESC m_TexDesc;

	ID3DXSprite * m_pSprite;
	IDirect3DDevice9 * m_pDevice;
public:
	~CTexture();
	CTexture( ID3DXSprite * pSprite, const char * szPath );

	IDirect3DTexture9 * GetTexture()
	{
		return m_pTexture;
	}

	void Draw( CPos cpPos, int iWidth, int iHeight );
};