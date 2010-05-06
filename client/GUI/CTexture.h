#pragma once

#include "CGUI.h"

class CTexture
{
	IDirect3DTexture9 * m_pTexture;
	D3DSURFACE_DESC m_TexDesc;

	ID3DXSprite * m_pSprite;
	IDirect3DDevice9 * m_pDevice;
	CColor *m_pColor;
public:
	~CTexture();
	CTexture( ID3DXSprite * pSprite, const char * szPath );
	CTexture( ID3DXSprite * pSprite, const char * szPath, CColor *color );

	IDirect3DTexture9 * GetTexture()
	{
		return m_pTexture;
	}

	void SetColor(CColor *color);
	CColor *GetColor();
	void Draw( CPos cpPos, int iWidth = -1, int iHeight = -1 );
	void Draw( int iX, int iY, int iWidth = -1, int iHeight = -1 );
};