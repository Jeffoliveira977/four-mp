#include "CGUI.h"
#include <stdarg.h>
#include <stdio.h>

CGUI::CGUI( IDirect3DDevice9 * pDevice )
{
	InitializeCriticalSection(&cs);

	if( !pDevice )
		MessageBoxA( 0, "pDevice invalid.", 0, 0 );

	m_pDevice = pDevice;
	m_wFocus = 0;

	D3DXCreateSprite( pDevice, &m_pSprite );
	D3DXCreateLine( pDevice, &m_pLine );

	m_pMouse = new CMouse( this, pDevice, m_pSprite );
	m_pKeyboard = new CKeyboard(this);
	m_pFont = 0;

	Cvars[ "$Value" ] = new CVar( SliderValue );
	Cvars[ "$MaxValue" ] = new CVar( MaxValue );
	Cvars[ "$MinValue" ] = new CVar( MinValue );

	SetVisible( false );
}

CGUI::~CGUI()
{
	EnterCriticalSection(&cs);

	for( std::map<std::string,CVar*>::iterator iIter = Cvars.begin(); iIter != Cvars.end(); iIter++ )
		SAFE_DELETE( iIter->second );

	SAFE_DELETE( m_pFont );
	SAFE_DELETE( m_pMouse );
	SAFE_DELETE( m_pKeyboard );

	SAFE_RELEASE( m_pSprite );
	SAFE_RELEASE( m_pLine );

	SAFE_DELETE( m_wFocus );

	for( int i = 0; i < (int)m_eLine[0].size(); i++ )
		SAFE_DELETE( m_eLine[0][i] );
	m_eLine[0].clear();

	for( int i = 0; i < (int)m_eLine[1].size(); i++ )
		SAFE_DELETE( m_eLine[1][i] );
	m_eLine[1].clear();

	for( int i = 0; i < (int)m_eBox[0].size(); i++ )
		SAFE_DELETE( m_eBox[0][i] );
	m_eBox[0].clear();

	for( int i = 0; i < (int)m_eBox[1].size(); i++ )
		SAFE_DELETE( m_eBox[1][i] );
	m_eBox[1].clear();

	for( int i = 0; i < (int)m_eText[0].size(); i++ )
		SAFE_DELETE( m_eText[0][i] );
	m_eText[0].clear();

	for( int i = 0; i < (int)m_eText[1].size(); i++ )
		SAFE_DELETE( m_eText[1][i] );
	m_eText[1].clear();

	for( int i = 0; i < (int)m_eImage[0].size(); i++ )
		SAFE_DELETE( m_eImage[0][i] );
	m_eImage[0].clear();

	for( int i = 0; i < (int)m_eImage[1].size(); i++ )
		SAFE_DELETE( m_eImage[1][i] );
	m_eImage[1].clear();
	
	for( int i = 0; i < static_cast<int>( m_vWindows.size() ); i++ )
		SAFE_DELETE( m_vWindows[i] );
	m_vWindows.clear();
	
	LeaveCriticalSection(&cs);
	DeleteCriticalSection(&cs);
}

void CGUI::LoadFont(int size, char *font)
{
	m_pFont = new CFont( this, GetDevice(), size, font );
}

void CGUI::SetFontColors(int Index, int r, int g, int b, int a)
{
	m_pFont->SetColor( Index, CColor( r, g, b, a ) );
}

void CGUI::SetVarInt(const char *name, int value)
{
	Cvars[ name ] = new CVar( value );
}

void CGUI::SetVarString(const char *name, std::string value)
{
	Cvars[ name ] = new CVar( value );
}

void CGUI::SetVarBool(const char *name, bool value)
{
	Cvars[ name ] = new CVar( value );
}

void CGUI::LoadInterfaceFromFile( const char * pszFilePath, const char * dir )
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

							sStream << pThemeElement->Value() << "/" << dir << pTextureElement->Attribute( "path" );
							
							pState->mTextures[ pTextureElement->Attribute( "string" ) ] = new CTexture( GetSprite(), sStream.str().c_str(), new CColor(pTextureElement) );
						}
						for( TiXmlElement * pIntElement = pStateElement->FirstChildElement( "Int" ); pIntElement; pIntElement = pIntElement->NextSiblingElement( "Int" ) )
						{
							pszString = pIntElement->Attribute( "string" );

							if( !pszString )
								continue;

							pState->mInts[ pszString ] = atoi(pIntElement->Attribute("value"));
					}

						m_mThemes[ pThemeElement->Value() ][ pElementElement->Value() ] = sCurElement;
					}
				}
		}
	}
}

void LOG(const char *s, int i = 0) 
{ 
	FILE *fi = fopen("gui.log", "a");
	fprintf(fi, "%s %d", s, i);
	fclose(fi);
}

void CGUI::UpdateFromFile( const char * pszFilePath )
{
	LOG("-1");
	TiXmlDocument Document;

	LOG("0");

	if( !Document.LoadFile( pszFilePath ) )
	{
		LOG("NOT LOAD");
		MessageBoxA(NULL, Document.ErrorDesc(), "UpdateGUI", MB_OK);
		return;
	}

	LOG("1");

	TiXmlHandle hDoc( &Document );

	TiXmlElement * pGUI = hDoc.FirstChildElement( "GUI" ).Element();
	if( !pGUI ) 
	{
		MessageBoxA(NULL, "XML Error", "UpdateGUI", MB_OK);
		return;
	}

	LOG("2");

	for( TiXmlElement * pThemeElement = pGUI->FirstChildElement(); pThemeElement; pThemeElement = pThemeElement->NextSiblingElement() )
	{
		int Index = 0;
		CWindow *wParent = NULL;

		LOG("FOR-1");

		if(strcmp(pThemeElement->Value(), "Over") == 0) Index = 1;
		if(strcmp(pThemeElement->Attribute("parent"), "none") != 0)
			wParent = GetWindowByString(pThemeElement->Attribute("parent"), 1);

		LOG("FOR-2");

		for( TiXmlElement * pElementElement = pThemeElement->FirstChildElement(); pElementElement; pElementElement = pElementElement->NextSiblingElement() )
		{
			const char *Element = pElementElement->Value();

			LOG("for-1");
			if(!Element) LOG("for-1 !Element");
			LOG(Element);

			if(strcmp(Element, "Element") == 0)
			{
				LOG("for-2 Element");
				CElement *pElement;
				if(wParent == NULL)
					pElement = GetWindowByString(pElementElement->Attribute("name"), 1);
				else
					pElement = wParent->GetElementByString(pElementElement->Attribute("name"), 1);

				if(!pElement) LOG("for !pElement");
				
				for(TiXmlElement * pElem = pElementElement->FirstChildElement( "Base" ); pElem; pElem = pElem->NextSiblingElement( "Base" ))
				{
					const char *name = pElem->Attribute("string");
					const char *value = pElem->Attribute("value");

					LOG("sfor-1");
					LOG(name);
					LOG(value);

					if(strcmp(name, "height") == 0) pElement->SetHeight(atoi(value));
					else if(strcmp(name, "width") == 0) pElement->SetWidth(atoi(value));
					else if(strcmp(name, "name") == 0) pElement->SetString(value);
					else if(strcmp(name, "x") == 0) pElement->SetRelPos(atoi(value), -1);
					else if(strcmp(name, "y") == 0) pElement->SetRelPos(-1, atoi(value));
					else if(strcmp(name, "style") == 0) 
					{
						pElement->SetThemeElement( GetThemeElement( value ), atoi(pElem->Attribute("number")));
						pElement->SetElementState("Norm", atoi(pElem->Attribute("number")));
					}

					LOG("sfor-2");
				}

				LOG("for-3");

				TiXmlElement * pElem = pElementElement->FirstChildElement( "Font" );
				if(pElem)
				{
					pElement->SetFont(atoi(pElem->Attribute("size")), (char*)pElem->Attribute("name"), 
						pElem->Attribute("bold")[0]=='1', pElem->Attribute("italic")[0]=='1');
				}

				LOG("for-4");
			}
			else if(strcmp(Element, "Line") == 0)
			{
				LOG("for-2 Line");

				CLine *tLine = new CLine(this, atoi(pElementElement->Attribute("sx")), atoi(pElementElement->Attribute("sy")),
					atoi(pElementElement->Attribute("ex")),	atoi(pElementElement->Attribute("ey")),
					atoi(pElementElement->Attribute("size")), new CColor(pElementElement), wParent);

				LOG("for-3");

				if(!wParent) m_eLine[Index].push_back(tLine);
				else wParent->m_eLine[Index].push_back(tLine);

				LOG("for-4");
			}
			else if(strcmp(Element, "Box") == 0)
			{
				LOG("for-2 Box");
				CColor *Inner, *Border;

				TiXmlElement * pColorElement = pElementElement->FirstChildElement( "Color" ); 
				if(strcmp(pColorElement->Attribute("string"), "Inner") == 0) Inner = new CColor(pColorElement);
				else if(strcmp(pColorElement->Attribute("string"), "Border") == 0) Border = new CColor(pColorElement);

				pColorElement = pColorElement->NextSiblingElement( "Color" );
				if(strcmp(pColorElement->Attribute("string"), "Inner") == 0) Inner = new CColor(pColorElement);
				else if(strcmp(pColorElement->Attribute("string"), "Border") == 0) Border = new CColor(pColorElement);

				LOG("for-3");

				CBox *tBox = new CBox(this, atoi(pElementElement->Attribute("x")), atoi(pElementElement->Attribute("y")),
					atoi(pElementElement->Attribute("width")), atoi(pElementElement->Attribute("height")),
					Inner, Border, wParent);

				LOG("for-4");

				if(!wParent) m_eBox[Index].push_back(tBox);
				else wParent->m_eBox[Index].push_back(tBox);

				LOG("for-5");
			}
			else if(strcmp(Element, "Text") == 0)
			{
				LOG("for-2 Text");
				CText *tText = new CText(this, atoi(pElementElement->Attribute("x")), atoi(pElementElement->Attribute("y")),
					atoi(pElementElement->Attribute("width")), 20, pElementElement->Attribute("string"), 
					pElementElement->Attribute("name"), NULL);

				LOG("for-3");
				if(!wParent) m_eText[Index].push_back(tText);
				else wParent->AddElement(tText);

				LOG("for-4");
			}
			else if(strcmp(Element, "Image") == 0)
			{
				LOG("for-2 Image");
				CTexture *tTexture = new CTexture(GetSprite(), pElementElement->Attribute("src"), new CColor(pElementElement));

				LOG("for-3");
				CImage *tImg = new CImage(this, atoi(pElementElement->Attribute("x")), atoi(pElementElement->Attribute("y")),
					atoi(pElementElement->Attribute("width")), atoi(pElementElement->Attribute("height")),
					tTexture, wParent);

				LOG("for-4");

				if(!wParent) m_eImage[Index].push_back(tImg);
				else wParent->m_eImage[Index].push_back(tImg);

				LOG("for-5");
			}
		}
		LOG("FOR-3");
	}
	LOG("END");
}

void CGUI::FillArea( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dColor )
{
	DrawLine( iX + iWidth / 2, iY, iX + iWidth / 2, iY + iHeight, iWidth, d3dColor );
}

void CGUI::DrawLine( int iStartX, int iStartY, int iEndX, int iEndY, int iWidth, D3DCOLOR d3dColor )
{
	m_pLine->SetWidth( static_cast<float>( iWidth ) );

	D3DXVECTOR2 d3dxVector[2];
	
	d3dxVector[0] = D3DXVECTOR2( static_cast<float>( iStartX ), static_cast<float>( iStartY ) );
	d3dxVector[1] = D3DXVECTOR2( static_cast<float>( iEndX ), static_cast<float>( iEndY ) );
	
	m_pLine->Begin();
	m_pLine->Draw( d3dxVector, 2, d3dColor );
	m_pLine->End();
}

void CGUI::DrawOutlinedBox( int iX, int iY, int iWidth, int iHeight, D3DCOLOR d3dInnerColor, D3DCOLOR d3dBorderColor )
{
	FillArea( iX + 1, iY + 1, iWidth - 2,  iHeight - 2, d3dInnerColor );

	DrawLine( iX,				iY,					iX,					iY + iHeight,		1, d3dBorderColor );
	DrawLine( iX + 1,			iY,					iX + iWidth - 1,	iY,					1, d3dBorderColor );
	DrawLine( iX + 1,			iY + iHeight - 1,	iX + iWidth - 1,	iY + iHeight - 1,	1, d3dBorderColor );
	DrawLine( iX + iWidth - 1,	iY,					iX + iWidth - 1,	iY + iHeight,		1, d3dBorderColor );
}

void CGUI::AddWindow( CWindow * pWindow ) 
{
	EnterCriticalSection(&cs);
	m_vWindows.push_back( pWindow );
	LeaveCriticalSection(&cs);
}

void CGUI::BringToTop( CWindow * pWindow )
{
	EnterCriticalSection(&cs);
	for( int i = 0; i < static_cast<int>( m_vWindows.size() ); i++ )
	{
		if(!m_vWindows[i]) continue;
		if( m_vWindows[i] == pWindow )
			m_vWindows.erase( m_vWindows.begin() + i );
}

	if(m_wFocus)
		m_wFocus->LostFocus();
	if(pWindow)
		pWindow->GotFocus();

	m_vWindows.insert(  m_vWindows.end(), pWindow );
	m_wFocus = pWindow;
	LeaveCriticalSection(&cs);
}

void CGUI::Draw()
{
	if( !IsVisible() )
	{
			return;
}

	EnterCriticalSection(&cs);
	PreDraw();

	
	for(int i = 0; i < (int)m_eLine[0].size(); i++)
		m_eLine[0][i]->Draw();

	
	for(int i = 0; i < (int)m_eBox[0].size(); i++)
		m_eBox[0][i]->Draw();

	
	for(int i = 0; i < (int)m_eText[0].size(); i++)
		m_eText[0][i]->Draw();

	
	for(int i = 0; i < (int)m_eImage[0].size(); i++)
		m_eImage[0][i]->Draw();

	
	for( int iIndex = 0; iIndex < static_cast<int>( m_vWindows.size() ); iIndex++ )
	{
		if(!m_vWindows[ iIndex ]) continue;
		if( !m_vWindows[ iIndex ]->IsVisible() )
			continue;

		m_vWindows[ iIndex ]->Draw();
}

	
	for(int i = 0; i < (int)m_eLine[1].size(); i++)
		m_eLine[1][i]->Draw();

	
	for(int i = 0; i < (int)m_eBox[1].size(); i++)
		m_eBox[1][i]->Draw();

	
	for(int i = 0; i < (int)m_eText[1].size(); i++)
		m_eText[1][i]->Draw();

	
	for(int i = 0; i < (int)m_eImage[1].size(); i++)
		m_eImage[1][i]->Draw();

	
	GetMouse()->Draw();
	LeaveCriticalSection(&cs);

}

void CGUI::PreDraw()
{
	if( !m_tPreDrawTimer.Running() )
	{
		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if(!m_vWindows[ iIndex ]) continue;
			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;

			m_vWindows[ iIndex ]->PreDraw();
	}

		m_tPreDrawTimer.Start( 0.1f );
	}
}

void CGUI::MouseMove( CMouse * pMouse )
{
	EnterCriticalSection(&cs);
	CElement * pDragging = GetMouse()->GetDragging();

	if( !pDragging )
	{
		bool bGotWindow = false;

		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if(!m_vWindows[ iIndex ]) continue;
			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;

			int iHeight = 0;

			if( !m_vWindows[ iIndex ]->GetMaximized() )
				iHeight = m_vWindows[ iIndex ]->GetTitleBarHeight();

			if( !bGotWindow && GetMouse()->InArea( m_vWindows[ iIndex ], iHeight ) )
			{
				m_vWindows[ iIndex ]->MouseMove( pMouse );
				bGotWindow = true;
				break;
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
	LeaveCriticalSection(&cs);
}

bool CGUI::KeyEvent( SKey sKey )
{
	EnterCriticalSection(&cs);
	bool bTop = false;

	if( !sKey.m_vKey && ( sKey.m_bDown || ( GetMouse()->GetWheel() && !sKey.m_bDown ) ) )
	{
		CMouse * pMouse = GetMouse();

		//std::vector<CWindow*> vRepeat;

		for( int iIndex = static_cast<int>( m_vWindows.size() ) - 1; iIndex >= 0; iIndex-- )
		{
			if(!m_vWindows[ iIndex ]) continue;

			if( !m_vWindows[ iIndex ]->IsVisible() )
				continue;

			if( !bTop )
			{
				int iHeight = 0;

				if( !m_vWindows[ iIndex ]->GetMaximized() )
					iHeight = m_vWindows[ iIndex ]->GetTitleBarHeight();

				if( pMouse->InArea( m_vWindows[ iIndex ], iHeight ) && !bTop )
				{
					m_vWindows[ iIndex ]->KeyEvent( sKey );
					bTop = true;
					break;
				}
				/*else
					vRepeat.push_back( m_vWindows[ iIndex ] );*/
			}
			/*else
			{
				pMouse->SavePos();
				pMouse->SetPos( CPos( -1, -1 ) );
				m_vWindows[ iIndex ]->KeyEvent( sKey );
				pMouse->LoadPos();
			}*/
	}

		/*for( int iIndex = 0; iIndex < static_cast<int>( vRepeat.size() ); iIndex++ )
		{
			if(!vRepeat[ iIndex ]) continue;

			pMouse->SavePos();
			pMouse->SetPos( CPos( -1, -1 ) );
			vRepeat[ iIndex ]->KeyEvent( sKey );
			pMouse->LoadPos();
		}*/
	}
	else
	{
		bTop = false;

		/*for( */int iIndex = static_cast<int>( m_vWindows.size() ) - 1; /*iIndex >= 0; iIndex-- )
		{
			if(!m_vWindows[ iIndex ]) continue;*/
			if( m_vWindows[ iIndex ]->IsVisible() )
			{
				if( m_vWindows[ iIndex ]->GetFocussedElement() && m_vWindows[ iIndex ]->GetMaximized() )
					bTop = true;
				
				m_vWindows[ iIndex ]->KeyEvent( sKey );
			}
		/*}*/

		if( !sKey.m_bDown )
			bTop = false;
	}
	LeaveCriticalSection(&cs);
	return bTop;
}

void CGUI::OnLostDevice()
{
	//m_pDevice = 0;

	if( GetFont() )
		GetFont()->OnLostDevice();

	GetSprite()->OnLostDevice();
	
	m_pLine->OnLostDevice();
	m_pMouse->OnLostDevice();

	for(int i = 0; i < (int)m_eImage[0].size(); i++)
		m_eImage[0][i]->OnLostDevice();

	for(int i = 0; i < (int)m_eImage[1].size(); i++)
		m_eImage[1][i]->OnLostDevice();

	for(int i = 0; i < (int)m_vWindows.size(); i++)
		m_vWindows[i]->OnLostDevice();

	
	for (std::map<std::string, tTheme>::const_iterator p = m_mThemes.begin(); p != m_mThemes.end(); ++p)
		for (tTheme::const_iterator x = p->second.begin(); x != p->second.end(); ++x)
			for(std::map<std::string, SElementState*>::const_iterator y = x->second->m_mStates.begin(); y != x->second->m_mStates.end(); ++y)
				for(std::map<std::string, CTexture*>::const_iterator z = y->second->mTextures.begin(); z != y->second->mTextures.end(); ++z)
					z->second->OnLostDevice();			

}

void CGUI::OnResetDevice()
{
	//m_pDevice = pDevice;

	if( GetFont() )
		GetFont()->OnResetDevice(  );
	GetSprite()->OnResetDevice();
	
	m_pLine->OnResetDevice();
	m_pMouse->OnResetDevice();

	for(int i = 0; i < (int)m_eImage[0].size(); i++)
		m_eImage[0][i]->OnResetDevice();

	for(int i = 0; i < (int)m_eImage[1].size(); i++)
		m_eImage[1][i]->OnResetDevice();

	for(int i = 0; i < (int)m_vWindows.size(); i++)
		m_vWindows[i]->OnResetDevice();

	for (std::map<std::string, tTheme>::const_iterator p = m_mThemes.begin(); p != m_mThemes.end(); ++p)
		for (tTheme::const_iterator x = p->second.begin(); x != p->second.end(); ++x)
			for(std::map<std::string, SElementState*>::const_iterator y = x->second->m_mStates.begin(); y != x->second->m_mStates.end(); ++y)
				for(std::map<std::string, CTexture*>::const_iterator z = y->second->mTextures.begin(); z != y->second->mTextures.end(); ++z)
					z->second->OnResetDevice();	

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
		if( m_vWindows[ i ] )
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

bool CGUI::IsFocus(CWindow * w)
{
	return w == m_wFocus;
}