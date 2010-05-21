#pragma once

#include <stdio.h>
#include <string>
#include "D3D9.h"
#include "CColor.h"

class CGUI;

class CFont
{
	CGUI *pGui;
	CColor m_cColors[9];

	ID3DXFont * m_pFont;
public:
	CFont( CGUI *Gui, IDirect3DDevice9 * pDevice, int iHeight, char * pszFaceName, bool bold = 0, bool italic = 0 );
	~CFont();

	void OnLostDevice();
	void OnResetDevice();

	void DrawString( int iX, int iY, DWORD dwFlags, CColor * pColor, std::string sString, int iWidth = 0 );

	int GetStringWidth( const char * pszString );
	int GetStringHeight();

	void SetColor( int iIndex, CColor cColor );
	CColor & GetColor( int iIndex );

	void CutString( int iWidth, std::string & rString );
};