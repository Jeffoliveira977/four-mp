#include "CGUI.h"

CFont::CFont( IDirect3DDevice9 * pDevice, int iHeight, char * pszFaceName )
{
#ifdef USE_D3DX
	HRESULT hResult = D3DXCreateFontA( pDevice, -MulDiv( iHeight, GetDeviceCaps( GetDC( 0 ), LOGPIXELSY ), 72 ), 0, FW_NORMAL, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, pszFaceName, &m_pFont );

	if( FAILED( hResult ) )
		MessageBoxA( 0, /*DXGetErrorDescription9A( hResult )*/"Error", "D3DXCreateFontA failed", 0 );

	m_pFont->PreloadCharacters( 0, 255 );
#else
	m_pFont = new CD3DFont( pszFaceName, iHeight, 0 );
	m_pFont->Initialize( pDevice );
#endif
}

CFont::~CFont()
{
#ifdef USE_D3DX
	SAFE_RELEASE( m_pFont );
#else
	m_pFont->Invalidate();
#endif
}

void CFont::OnLostDevice()
{
#ifdef USE_D3DX
	m_pFont->OnLostDevice();
#else
	m_pFont->Invalidate();
#endif
}

void CFont::OnResetDevice( IDirect3DDevice9 * pDevice )
{
#ifdef USE_D3DX
	pDevice; // C4100: 'pDevice' : unreferenced formal parameter

	m_pFont->OnResetDevice();
#else
	m_pFont->Initialize( pDevice );
#endif
}

void CFont::DrawString( int iX, int iY, DWORD dwFlags, CColor * pColor, std::string sString, int iWidth )
{
	if( iWidth )
		CutString( iWidth, sString );
#ifdef USE_D3DX
		gpGui->GetSprite()->Begin( D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE );

		D3DXMATRIX mat;
		D3DXMatrixTranslation( &mat, static_cast<float>( iX ), static_cast<float>( iY ), 0 );
		gpGui->GetSprite()->SetTransform( &mat );

		RECT drawRect = { 0 };
		DWORD dwDrawFlags = DT_NOCLIP | ( ( dwFlags & FT_CENTER ) ? DT_CENTER : 0 ) | ( ( dwFlags & FT_VCENTER ) ? DT_VCENTER : 0 );
		m_pFont->DrawTextA( gpGui->GetSprite(), sString.c_str(), -1, &drawRect, dwDrawFlags, pColor->GetD3DCOLOR() );

		gpGui->GetSprite()->End();
#else
		m_pFont->Print( static_cast<float>( iX ), static_cast<float>( iY ), pColor->GetD3DCOLOR(), sString.c_str(), dwFlags );
#endif
}

int CFont::GetStringWidth( const char * pszString )
{
#ifdef USE_D3DX
	std::string sString( pszString );
	RECT rRect = { 0 };

	for( int i = 0; i <= static_cast<int>( sString.size() ); i++ )
		if( sString[i] == ' ' )
			sString[i] = '.';

	m_pFont->DrawTextA( 0, sString.c_str(), -1, &rRect, DT_CALCRECT, 0 );

	return rRect.right - rRect.left;
#else
	return m_pFont->DrawLength( (char*)pszString );
#endif
}

int CFont::GetStringHeight()
{
#ifdef USE_D3DX
	RECT rRect = { 0 };
	m_pFont->DrawTextA( 0, "Y", -1, &rRect, DT_CALCRECT, 0 );

	return rRect.bottom - rRect.top;
#else
	return m_pFont->DrawHeight();
#endif
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
		iWidth += gpGui->GetFont()->GetStringWidth( szCurrent );
		iIndex++;
	}

	if( iIndex < iLength )
		rString[ iIndex - 1 ] = '\0';
}