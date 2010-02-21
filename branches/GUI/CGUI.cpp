#include "CGUI.h"
#include "CD3DRender.h"

CGUI * gpGui = 0;

CGUI::CGUI( IDirect3DDevice9 * pDevice )
{
	if( !pDevice )
		MessageBoxA( 0, "pDevice invalid.", 0, 0 );

	m_pDevice = pDevice;

	D3DXCreateSprite( pDevice, &m_pSprite );

#ifdef USE_D3DX
	D3DXCreateLine( pDevice, &m_pLine );
#else
	m_pRender = new CD3DRender( 128 );
	m_pRender->Initialize( pDevice );
#endif

	m_pMouse = new CMouse( pDevice );
	m_pKeyboard = new CKeyboard();
	m_pFont = 0;

	Cvars[ "$Value" ] = new CVar( SliderValue );
	Cvars[ "$MaxValue" ] = new CVar( MaxValue );
	Cvars[ "$MinValue" ] = new CVar( MinValue );

	SetVisible( false );
	m_bReload = false;
}

CGUI::~CGUI()
{
	for( std::map<std::string,CVar*>::iterator iIter = Cvars.begin(); iIter != Cvars.end(); iIter++ )
		SAFE_DELETE( iIter->second )

	SAFE_DELETE( m_pFont )
	SAFE_DELETE( m_pMouse )
	SAFE_DELETE( m_pKeyboard )

	SAFE_RELEASE( m_pSprite )

#ifdef USE_D3DX
	SAFE_RELEASE( m_pLine )
#else
	SAFE_DELETE( m_pRender )
#endif

	for( int i = 0; i < static_cast<int>( m_vWindows.size() ); i++ )
		SAFE_DELETE( m_vWindows[i] )
}

void CGUI::LoadFont(int size, char *font)
{
	m_pFont = new CFont( gpGui->GetDevice(), size, font );
}

void CGUI::SetFontColors(int Index, int r, int g, int b, int a)
{
	m_pFont->SetColor( Index, CColor( r, g, b, a ) );
}

void CGUI::SetVarInt(const char *name, int value)
{
	gpGui->Cvars[ name ] = new CVar( value );
}

void CGUI::SetVarString(const char *name, std::string value)
{
	gpGui->Cvars[ name ] = new CVar( value );
}

void CGUI::SetVarBool(const char *name, bool value)
{
	gpGui->Cvars[ name ] = new CVar( value );
}

void CGUI::LoadInterfaceFromFile( const char * pszFilePath )
{
	TiXmlDocument Document;

	if( !Document.LoadFile( pszFilePath ) )
	{
		MessageBoxA( 0, Document.ErrorDesc(), "XML Error", 0 );
		return;
	}
	
	TiXmlHandle hDoc( &Document );

	TiXmlElement * pGUI = hDoc.FirstChildElement( "GUI" ).Element();
	if( pGUI )
	{
		TiXmlElement * pColorThemes = pGUI->FirstChildElement( "ColorThemes" );
		if( pColorThemes )
		{
			const char * pszDefaultTheme = pColorThemes->Attribute( "default" );
			if( pszDefaultTheme )
					m_sCurTheme = pszDefaultTheme;

			for( TiXmlElement * pThemeElement = pColorThemes->FirstChildElement(); pThemeElement; pThemeElement = pThemeElement->NextSiblingElement() )
				for( TiXmlElement * pElementElement = pThemeElement->FirstChildElement(); pElementElement; pElementElement = pElementElement->NextSiblingElement() )
				{
					SElement * sCurElement = new SElement();

					const char * pszDefault = pElementElement->Attribute( "default" );
					if( pszDefault )
						sCurElement->sDefaultState = std::string( pszDefault );

					for( TiXmlElement * pStateElement = pElementElement->FirstChildElement( "State" ); pStateElement; pStateElement = pStateElement->NextSiblingElement( "State" ) )
					{
						const char * pszString = pStateElement->Attribute( "string" );

						if( !pszString )
							continue;

						SElementState * pState = sCurElement->m_mStates[ pszString ] = new SElementState();

						pState->pParent = sCurElement;

						for( TiXmlElement * pColorElement = pStateElement->FirstChildElement( "Color" ); pColorElement; pColorElement = pColorElement->NextSiblingElement( "Color" ) )
						{
							pszString = pColorElement->Attribute( "string" );

							if( !pszString )
								continue;

							pState->mColors[ pszString ] = new CColor( pColorElement );
						}
						for( TiXmlElement * pTextureElement = pStateElement->FirstChildElement( "Texture" ); pTextureElement; pTextureElement = pTextureElement->NextSiblingElement( "Texture" ) )
						{
							std::stringstream sStream;

							sStream << pThemeElement->Value() << "/" << pTextureElement->Attribute( "path" );

							pState->mTextures[ pTextureElement->Attribute( "string" ) ] = new CTexture( GetSprite(), sStream.str().c_str() );
						}

						m_mThemes[ pThemeElement->Value() ][ pElementElement->Value() ] = sCurElement;
					}
				}
		}
	}
}

void CGUI::FillArea( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dColor )
{
#ifdef USE_D3DX
	DrawLine( iX + iWidth / 2, iY, iX + iWidth / 2, iY + iHeight, iWidth, d3dColor );
#else
	m_pRender->D3DAddQuad( iX, iY, iWidth, iHeight, d3dColor);
#endif
}

void CGUI::DrawLine( int iStartX, int iStartY, int iEndX, int iEndY, int iWidth, D3DCOLOR d3dColor )
{
#ifdef USE_D3DX
	m_pLine->SetWidth( static_cast<float>( iWidth ) );

	D3DXVECTOR2 d3dxVector[2];
	
	d3dxVector[0] = D3DXVECTOR2( static_cast<float>( iStartX ), static_cast<float>( iStartY ) );
	d3dxVector[1] = D3DXVECTOR2( static_cast<float>( iEndX ), static_cast<float>( iEndY ) );
	
	m_pLine->Begin();
	m_pLine->Draw( d3dxVector, 2, d3dColor );
	m_pLine->End();
#else
	iWidth;

	m_pRender->Begin( D3DPT_LINELIST );
	m_pRender->D3DColour( d3dColor );
	m_pRender->D3DVertex2f( static_cast<float>( iStartX ), static_cast<float>( iStartY ) );
	m_pRender->D3DVertex2f( static_cast<float>( iEndX ), static_cast<float>( iEndY ) );
	m_pRender->End();
#endif
}

void CGUI::DrawOutlinedBox( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dInnerColor, D3DCOLOR d3dBorderColor )
{
	FillArea( iX + 1, iY + 1, iWidth - 2,  iHeight - 2, d3dInnerColor );

	DrawLine( iX,				iY,					iX,					iY + iHeight,		1, d3dBorderColor );
	DrawLine( iX + 1,			iY,					iX + iWidth - 1,	iY,					1, d3dBorderColor );
	DrawLine( iX + 1,			iY + iHeight - 1,	iX + iWidth - 1,	iY + iHeight - 1,	1, d3dBorderColor );
	DrawLine( iX + iWidth - 1,	iY,					iX + iWidth - 1,	iY + iHeight,		1, d3dBorderColor );
}

CWindow * CGUI::AddWindow( CWindow * pWindow ) 
{
	m_vWindows.push_back( pWindow );
	return m_vWindows.back();
}

void CGUI::BringToTop( CWindow * pWindow )
{
	for( int i = 0; i < static_cast<int>( m_vWindows.size() ); i++ )
		if( m_vWindows[i] == pWindow )
			m_vWindows.erase( m_vWindows.begin() + i );
	m_vWindows.insert(  m_vWindows.end(), pWindow );
}

void CGUI::Draw()
{
	if( !gpGui->IsVisible() )
		return;

	PreDraw();

	for( int iIndex = 0; iIndex < static_cast<int>( m_vWindows.size() ); iIndex++ )
	{
		if( !m_vWindows[ iIndex ]->IsVisible() )
			continue;

		m_vWindows[ iIndex ]->Draw();
	}

	GetMouse()->Draw();
}

void CGUI::PreDraw()
{
	if( !m_tPreDrawTimer.Running() )
	{
		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;
			
			m_vWindows[ iIndex ]->PreDraw();
		}
		m_tPreDrawTimer.Start( 0.1f );
	}
}

void CGUI::MouseMove( CMouse * pMouse )
{
	CElement * pDragging = GetMouse()->GetDragging();
	if( !pDragging )
	{
		bool bGotWindow = false;

		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;

			int iHeight = 0;

			if( !m_vWindows[ iIndex ]->GetMaximized() )
				iHeight = TITLEBAR_HEIGHT;

			if( !bGotWindow && gpGui->GetMouse()->InArea( m_vWindows[ iIndex ], iHeight ) )
			{
				m_vWindows[ iIndex ]->MouseMove( pMouse );
				bGotWindow = true;
			}
			else
			{
				pMouse->SavePos();
				pMouse->SetPos( -1, -1 );
				m_vWindows[ iIndex ]->MouseMove( pMouse );
				pMouse->LoadPos();
			}
		}
	}
	else
		pDragging->MouseMove( pMouse );
}

bool CGUI::KeyEvent( SKey sKey )
{
	bool bTop = false;

	if( !sKey.m_vKey && ( sKey.m_bDown || ( GetMouse()->GetWheel() && !sKey.m_bDown ) ) )
	{
		CMouse * pMouse = gpGui->GetMouse();

		std::vector<CWindow*> vRepeat;

		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;

			if( !bTop )
			{
				int iHeight = 0;

				if( !m_vWindows[ iIndex ]->GetMaximized() )
					iHeight = TITLEBAR_HEIGHT;

				if( pMouse->InArea( m_vWindows[ iIndex ], iHeight ) && !bTop )
				{
					m_vWindows[ iIndex ]->KeyEvent( sKey );
					bTop = true;
				}
				else
					vRepeat.push_back( m_vWindows[ iIndex ] );
			}
			else
			{
				pMouse->SavePos();
				pMouse->SetPos( CPos( -1, -1 ) );
				m_vWindows[ iIndex ]->KeyEvent( sKey );
				pMouse->LoadPos();
			}
		}

		for( int iIndex = 0; iIndex < static_cast<int>( vRepeat.size() ); iIndex++ )
		{
			pMouse->SavePos();
			pMouse->SetPos( CPos( -1, -1 ) );
			vRepeat[ iIndex ]->KeyEvent( sKey );
			pMouse->LoadPos();
		}
	}
	else
	{
		bTop = false;

		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
			if( m_vWindows[ iIndex ]->IsVisible() )
			{
				if( m_vWindows[ iIndex ]->GetFocussedElement() && m_vWindows[ iIndex ]->GetMaximized() )
					bTop = true;

				m_vWindows[ iIndex ]->KeyEvent( sKey );
			}

		if( !sKey.m_bDown )
			bTop = false;
	}

	return bTop;
}

void CGUI::OnLostDevice()
{
	m_pDevice = 0;

	if( GetFont() )
		GetFont()->OnLostDevice();
	GetSprite()->OnLostDevice();
	
#ifdef USE_D3DX
	m_pLine->OnLostDevice();
#else
	m_pRender->Invalidate();
	SAFE_DELETE( m_pRender )
#endif
}

void CGUI::OnResetDevice( IDirect3DDevice9 * pDevice )
{
	m_pDevice = pDevice;

	if( GetFont() )
		GetFont()->OnResetDevice( pDevice );
	GetSprite()->OnResetDevice();
	
#ifdef USE_D3DX
	m_pLine->OnResetDevice();
#else
	m_pRender = new CD3DRender( 128 );
	m_pRender->Initialize( pDevice );
#endif
}

CMouse * CGUI::GetMouse()
{
	return m_pMouse;
}

CKeyboard * CGUI::GetKeyboard()
{
	return m_pKeyboard;
}

IDirect3DDevice9 * CGUI::GetDevice()
{
	return m_pDevice;
}

CFont * CGUI::GetFont()
{
	return m_pFont;
}

ID3DXSprite * CGUI::GetSprite()
{
	return m_pSprite;
}

CWindow * CGUI::GetWindowByString( std::string sString, int iIndex )
{
	for( int i = 0; i < static_cast<int>( m_vWindows.size() ); i++ )
		if( m_vWindows[ i ]->GetString( false, iIndex ) == sString )
			return m_vWindows[ i ];
	return 0;
}

SElement * CGUI::GetThemeElement( std::string sElement )
{
	return m_mThemes[ m_sCurTheme ][ sElement ];
}

void CGUI::SetVisible( bool bVisible )
{
	m_bVisible = bVisible;
}

bool CGUI::IsVisible()
{
	return m_bVisible;
}

bool CGUI::ShouldReload()
{
	return m_bReload;
}

void CGUI::Reload()
{
	m_bReload = true;
}