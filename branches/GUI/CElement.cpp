#include "CGUI.h"

void CElement::SetElement( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback, bool abs)
{
	if(!abs)
		SetRelPos( CPos( X, Y ) );
	else
		SetAbsPos( CPos( X, Y ) );
	SetWidth( Width );
	SetHeight( Height );

	if( String )
	{
		SetString( String );
	}

	if( String2 )
	{
		SetString( String2, 1 );
	}

	if( Callback )
	{
		/*CVar * pCvar = gpGui->Cvars[ Callback ];
	
		if( pCvar )
			SetAction( pCvar->GetAction() );
		else
		{
			SetAction( 0 );
			MessageBoxA( 0, "Cvar invalid", Callback, 0 );
		}*/
		SetAction( Callback );
	}
	else
		SetAction( 0 );

	pFont = 0;
	SetMouseOver( false );
}

void CElement::SetParent( CWindow * pParent )
{
	m_pParent = pParent;
}

CWindow * CElement::GetParent()
{
	return m_pParent;
}

void CElement::SetAction( tAction pAction )
{
	m_pAction = pAction;
}

tAction CElement::GetAction()
{
	return m_pAction;
}

void CElement::SetRelPos( CPos relPos )
{
	m_relPos = relPos;
}

CPos * CElement::GetRelPos()
{
	return &m_relPos;
}

void CElement::SetAbsPos( CPos absPos )
{
	m_absPos = absPos;
}

CPos * CElement::GetAbsPos()
{
	return &m_absPos;
}

void CElement::SetWidth( int iWidth )
{
	m_iWidth = iWidth;
}

int CElement::GetWidth()
{
	return m_iWidth;
}

void CElement::SetHeight( int iHeight )
{
	m_iHeight = iHeight;
}

int CElement::GetHeight()
{
	return m_iHeight;
}

bool CElement::HasFocus()
{
	return GetParent()->GetFocussedElement() == this;
}

void CElement::SetString( std::string sString, int iIndex )
{
	if( static_cast<int>( sString.length() ) > 255 )
		return;

	m_sRaw[ iIndex ] = sString;
}

std::string CElement::GetString( bool bReplaceVars, int iIndex )
{
	m_sFormatted[ iIndex ] = m_sRaw[ iIndex ];

	if( bReplaceVars && m_sRaw[ iIndex ].find( "$" ) != std::string::npos )
	{
		for( std::map<std::string,CVar*>::reverse_iterator iIter = gpGui->Cvars.rbegin(); iIter != gpGui->Cvars.rend(); iIter++ )
		{
			int iPos = m_sFormatted[ iIndex ].find( iIter->first );

			if( iPos == static_cast<int>( std::string::npos ) )
				continue;

			int iLen = iIter->first.length();
			if( static_cast<int>( m_sFormatted[ iIndex ].length() ) - iPos >= iLen )
			{
				bool bReplace = true;

				for( int i = 0; i < iLen; i++ )
					if( iIter->first[ i ] != m_sFormatted[ iIndex ][ iPos + i ] )
						bReplace = false;

				if( bReplace && iIter->second )
				{
					if( iIter->second->GetString().length() )
						m_sFormatted[ iIndex ].replace( iPos, iIter->first.length(), iIter->second->GetString() );
					else
						m_sFormatted[ iIndex ].replace( iPos, iIter->first.length(), iIter->second->GetActionString( 0, this ) );
				}
			}
		}

		return m_sFormatted[ iIndex ];
	}
	return m_sRaw[ iIndex ];
}

std::string CElement::GetFormatted( int iIndex )
{
	return m_sFormatted[ iIndex ];
}

bool CElement::GetMouseOver()
{
	return m_bMouseOver;
}

bool CElement::SetMouseOver( bool bMouseOver )
{
	if(GetAction())
		if(bMouseOver) GetAction()(this, MOUSE_OVER, 0);
		else GetAction()(this, MOUSE_OUT, 0);
	return m_bMouseOver = bMouseOver;
}

SElement * CElement::SetThemeElement( SElement * pThemeElement, int iIndex )
{
	return m_pThemeElement[ iIndex ] = pThemeElement;
}

SElement * CElement::GetThemeElement( int iIndex )
{
	return m_pThemeElement[ iIndex ];
}

void CElement::SetElementState( std::string sState, int iIndex )
{
	m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ sState ];

	if( !m_pElementState )
		m_pElementState[ iIndex ] = GetThemeElement( iIndex )->m_mStates[ GetThemeElement( iIndex )->sDefaultState ];

	UpdateTheme( iIndex );
}

SElementState * CElement::GetElementState( int iIndex )
{
	return m_pElementState[ iIndex ];
}

void CElement::LostFocus()
{
	if(GetAction())
		GetAction()(this, LOST_FOCUS, 0);
}

void CElement::GotFocus()
{
	if(GetAction())
		GetAction()(this, GOT_FOCUS, 0);
}

void CElement::SendMsg(CMSG msg, int Param)
{
	if(GetAction())
		GetAction()(this, msg, Param);
}

void CElement::UpdateTheme( int )
{
}

void CElement::Draw()
{
}

void CElement::PreDraw()
{
}

void CElement::MouseMove( CMouse * )
{
}

void CElement::KeyEvent( SKey )
{
}

void CElement::SetFont(int size, char *name)
{
	pFont = new CFont(gpGui->GetDevice(), size, name);
}

void CElement::SetFont(CFont *font)
{
	pFont = font;
}

CFont * CElement::GetFont()
{
	if(!pFont)
		return gpGui->GetFont();
	return pFont;
}