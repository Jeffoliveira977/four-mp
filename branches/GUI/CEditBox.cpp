#include "CGUI.h"

CEditBox::CEditBox( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	SetHeight( BUTTON_HEIGHT );

	m_iStart = 0;
	SetIndex( 0 );
	m_bCursorState = false;
	SetAction( 0 );
	if( Callback )
	{
		/*CVar * pCvar = gpGui->Cvars[ Callback ];
	
		if( pCvar )
			SetAction( pCvar->GetAction() );
		else
			MessageBoxA( 0, "Cvar invalid", GetString().c_str(), 0 );*/
		SetAction( Callback );
	}
	SetThemeElement( gpGui->GetThemeElement( "EditBox" ) );
	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "EditBox", 0 );
	else
		SetElementState( "Norm" );
}

void CEditBox::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SElementState * pState = GetElementState();
	if( pState )
	{
		gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );
		gpGui->GetFont()->DrawString( Pos.GetX() + 4, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString, &GetString()[ GetStart() ], GetWidth() );
		
		if( m_bCursorState && HasFocus() )
			gpGui->FillArea( Pos.GetX() + 2 + m_iCursorX, Pos.GetY() + 2, 2, GetHeight() - 4, pCursor->GetD3DCOLOR() );
	}
}

void CEditBox::PreDraw()
{
	if( !m_tCursorTimer.Running() )
	{
		m_bCursorState = !m_bCursorState;
		m_tCursorTimer.Start( 0.6f );
	}
}

void CEditBox::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );
}

void CEditBox::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( gpGui->GetMouse()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				int iX = gpGui->GetMouse()->GetPos().GetX();
				int iAbsX = ( *GetParent()->GetAbsPos() + *GetRelPos() ).GetX();

				std::string sString( &GetString()[ GetStart() ] );
				
				if( iX >= iAbsX + gpGui->GetFont()->GetStringWidth( sString.c_str() ) )
					SetIndex( sString.length() );
				else
				{
					for( int i = 0; i <= static_cast<int>( sString.length() ); i++ )
					{
						if( iX <= iAbsX + gpGui->GetFont()->GetStringWidth( sString.c_str() ) )
						{
							sString[ i ] = 0;
							if( iX > iAbsX + gpGui->GetFont()->GetStringWidth( sString.c_str() ) )
								SetIndex( i );
						}
						sString = &GetString()[ GetStart() ];
					}
				}

				GetParent()->SetFocussedElement( this );
			}
		}
	}
	else if( sKey.m_bDown && HasFocus() )
	{
		switch( sKey.m_vKey )
		{
		case VK_END:
			{
				std::string sString = GetString();

				SetIndex( strlen( &sString[ GetStart() ] ) );

				while( gpGui->GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || m_iCursorX > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}
				break;
			}
		case VK_HOME:
			{
				SetStart( 0 );
				SetIndex( 0 );

				break;
			}
		case VK_BACK:
			{
				if( GetIndex() )
				{
					std::string sString = GetString();

					sString.erase( GetStart() + GetIndex() - 1,  1 );

					SetString( sString );
					SetIndex( GetIndex() - 1 );
				}
				else if( GetStart() )
				{
					SetStart( GetStart() - 1 );
					SetIndex( 1 );
				}

				break;
			}
		case VK_DELETE:
			{
				std::string sString = GetString();

				if( GetIndex() <= static_cast<int>( sString.length() ) )
					sString.erase( GetStart() + m_iIndex, 1 );

				SetString( const_cast<char*>( sString.c_str() ) );

				break;
			}
		case VK_LEFT:
			{
				if( !GetIndex() && GetStart() )
					SetStart( GetStart() - 1 );
				else if( GetIndex() )
					SetIndex( GetIndex() - 1 );

				break;
			}
		case VK_RIGHT:
			{
				SetIndex( GetIndex() + 1 );

				std::string sString = GetString();
				sString[ GetIndex() ] = 0;

				while( gpGui->GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || m_iCursorX > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		case VK_RETURN:
			{
				GetParent()->SetFocussedElement( 0 );

				tAction pAction = GetAction();

				if( pAction )
					pAction( GetString().c_str(), this );

				break;
			}
		default:
			{
				std::string sString( GetString() );

				int iPrevLen = sString.length();

				BYTE bKeys[256] = { 0 };
				GetKeyboardState( bKeys );

				WORD wKey = 0;
				ToAscii( sKey.m_vKey, HIWORD( sKey.m_lParam )&0xFF, bKeys, &wKey, 0 );

				char szKey[2] = { static_cast<char>( wKey ), 0 };
				if( GetStart() + m_iIndex >= 0 && GetStart() + m_iIndex <= static_cast<int>( sString.length() ) )
				{
					if( wKey != 22 )
						sString.insert( GetStart() + m_iIndex, szKey );
					else
					{
						if( !OpenClipboard( 0 ) )
							break;

						HANDLE hData = GetClipboardData( CF_TEXT );
						char * pszBuffer = static_cast<char*>( GlobalLock( hData ) );
						
						if( pszBuffer )
							sString.insert( GetStart() + m_iIndex, pszBuffer );

						GlobalUnlock( hData );
						CloseClipboard();
					}
				}

				SetString( const_cast<char*>( sString.c_str() ) );
				if( sKey.m_vKey == ' ' )
					SetIndex( GetIndex() + 1 );
				else
					SetIndex( GetIndex() + sString.length() - iPrevLen );

				while( gpGui->GetFont()->GetStringWidth( &GetString().c_str()[ GetStart() ] ) > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		}
	}
}

int CEditBox::GetIndex()
{
	return m_iIndex;
}

void CEditBox::SetIndex( int iIndex )
{
	std::string sString( &GetString()[ GetStart() ] );

	if( iIndex > static_cast<int>( sString.length() ) || iIndex < 0 )
		return;
	sString[ iIndex ] = 0;
	m_iCursorX = gpGui->GetFont()->GetStringWidth( sString.c_str() );

	m_iIndex = iIndex;
}

int CEditBox::GetStart()
{
	return m_iStart;
}

void CEditBox::SetStart( int iStart )
{
	m_iStart = iStart;
}

void CEditBox::SetAction( tAction pCallback )
{
	m_pCallback = pCallback;
}

tAction CEditBox::GetAction()
{
	return m_pCallback;
}

void CEditBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pString = pState->GetColor( "String" );
	pCursor = pState->GetColor( "Cursor" );
}