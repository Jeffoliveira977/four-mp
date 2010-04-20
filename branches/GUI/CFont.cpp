#include "CGUI.h"

CFont::CFont( CGUI *Gui, IDirect3DDevice9 * pDevice, int iHeight, char * pszFaceName )
{
	pGui = Gui;
	HRESULT hResult = D3DXCreateFontA( pDevice, -MulDiv( iHeight, GetDeviceCaps( GetDC( 0 ), LOGPIXELSY ), 72 ), 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pszFaceName, &m_pFont );

	if( FAILED( hResult ) )
		MessageBoxA( 0, /*DXGetErrorDescription9A( hResult )*/"Error", "D3DXCreateFontA failed", 0 );
	m_pFont->PreloadCharacters( 0, 255 );
}

CFont::~CFont()
{
	SAFE_RELEASE( m_pFont );
}

void CFont::OnLostDevice()
{
	m_pFont->OnLostDevice();
}

void CFont::OnResetDevice( IDirect3DDevice9 * pDevice )
{
	pDevice; // C4100: 'pDevice' : unreferenced formal parameter

	m_pFont->OnResetDevice();
}

void CFont::DrawString( int iX, int iY, DWORD dwFlags, CColor * pColor, std::string sString, int iWidth )
{
	if( iWidth )
		CutString( iWidth, sString );
		pGui->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

		D3DXMATRIX mat;
		D3DXMatrixTranslation( &mat, static_cast<float>( iX ), static_cast<float>( iY ), 0 );
		pGui->GetSprite()->SetTransform( &mat );

		RECT drawRect = { 0 };
		DWORD dwDrawFlags = DT_NOCLIP | ( ( dwFlags & FT_CENTER ) ? DT_CENTER : 0 ) | ( ( dwFlags & FT_VCENTER ) ? DT_VCENTER : 0 );
		m_pFont->DrawTextA( pGui->GetSprite(), sString.c_str(), -1, &drawRect, dwDrawFlags, pColor->GetD3DCOLOR() );

		pGui->GetSprite()->End();
}

int CFont::GetStringWidth( const char * pszString )
{
	std::string sString( pszString );
	RECT rRect = { 0 };

	for( int i = 0; i <= static_cast<int>( sString.size() ); i++ )
		if( sString[i] == ' ' )
			sString[i] = '.';

	m_pFont->DrawTextA( 0, sString.c_str(), -1, &rRect, DT_CALCRECT, 0 );

	return rRect.right - rRect.left;
}

int CFont::GetStringHeight()
{
	RECT rRect = { 0 };
	m_pFont->DrawTextA( 0, "Y", -1, &rRect, DT_CALCRECT, 0 );

	return rRect.bottom - rRect.top;
}

void CFont::SetColor( int iIndex, CColor cColor )
{
	if( iIndex < 0 || iIndex > 8 )
		return;

	m_cColors[ iIndex ] = cColor;
}

CColor & CFont::GetColor( int iIndex )
{
	if( iIndex < 0 || iIndex > 8 )
		return m_cColors[ 0 ];

	return m_cColors[ iIndex ];
}

void CFont::CutString( int iMaxWidth, std::string & rString )
{
	int iIndex = 0, iLength = rString.length();

	for( int iWidth = 0; iIndex < iLength && iWidth + 10 < iMaxWidth; )
	{
		char szCurrent[ 2 ] = { rString.c_str()[ iIndex ], 0 };
		iWidth += /*pGui->GetFont()->*/GetStringWidth( szCurrent );
		iIndex++;
	}

	if( iIndex < iLength )
		rString[ iIndex - 1 ] = '\0';
}