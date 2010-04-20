#include "CGUI.h"

CEditBox::CEditBox( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	SetHeight( 20 );

	SizeEdge = 0;

	m_iStart = 0;
	CenterAlign = 0;
	SetIndex( 0 );
	m_bCursorState = false;
	SetAction( 0 );
	if( Callback )
	{
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
		pEdit[0]->Draw( Pos, SizeEdge, GetHeight() );
		pEdit[2]->Draw( Pos.GetX()+GetWidth()-SizeEdge, Pos.GetY(), SizeEdge, GetHeight() );
		pEdit[1]->Draw( Pos.GetX()+SizeEdge-1, Pos.GetY(), GetWidth()-2*SizeEdge+2, GetHeight() );

		if(CenterAlign)
			GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_VCENTER|FT_CENTER, pString, &GetString()[ GetStart() ], GetWidth() );
		else
			GetFont()->DrawString( Pos.GetX() + 4, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString, &GetString()[ GetStart() ], GetWidth() );

		int hgt = (GetHeight()-GetFont()->GetStringHeight())/2;

		if( m_bCursorState && HasFocus() && GetEnabled() )
			gpGui->FillArea( Pos.GetX() + 2 + m_iCursorX, Pos.GetY() + hgt, 2, GetHeight() - 2*hgt, pCursor->GetD3DCOLOR() );
	}
}

void CEditBox::SetAlignCenter(bool al)
{
	CenterAlign = al;
}

void CEditBox::PreDraw()
{
	SetElementState( HasFocus()?"Active":"Norm" );

	if( !m_tCursorTimer.Running() )
	{
		m_bCursorState = !m_bCursorState;
		m_tCursorTimer.Start( 0.6f );
	}
}

bool CEditBox::MouseMove( CMouse * pMouse, bool over )
{
	if(!GetEnabled()) return 0;
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	bool inArea = pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) && over;
	SetMouseOver( inArea );
	return inArea;
}

bool CEditBox::KeyEvent( SKey sKey )
{
	if(!GetEnabled()) return 0;
	if( !sKey.m_vKey )
	{
		if( gpGui->GetMouse()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				SendMsg(CLICK, 0);

				int iX = gpGui->GetMouse()->GetPos().GetX();
				int iAbsX = ( *GetParent()->GetAbsPos() + *GetRelPos() ).GetX();

				std::string sString( &GetString()[ GetStart() ] );
				
				if( iX >= iAbsX + GetFont()->GetStringWidth( sString.c_str() ) )
					SetIndex( sString.length() );
				else
				{
					for( int i = 0; i <= static_cast<int>( sString.length() ); i++ )
					{
						if( iX <= iAbsX + GetFont()->GetStringWidth( sString.c_str() ) )
						{
							sString[ i ] = 0;
							if( iX > iAbsX + GetFont()->GetStringWidth( sString.c_str() ) )
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

				while( GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || m_iCursorX > GetWidth() - 5 )
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

				while( GetFont()->GetStringWidth( &sString.c_str()[ GetStart() ] ) > GetWidth() - 5 || m_iCursorX > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		case VK_RETURN:
			{
				GetParent()->SetFocussedElement( 0 );

				SendMsg(END, 0);

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
				SendMsg(CHANGE, wKey);

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

				while( GetFont()->GetStringWidth( &GetString().c_str()[ GetStart() ] ) > GetWidth() - 5 )
				{
					SetStart( GetStart() + 1 );
					SetIndex( GetIndex() - 1 );
				}

				break;
			}
		}
	}
	return 0;
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
	m_iCursorX = GetFont()->GetStringWidth( sString.c_str() );

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

void CEditBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
	pCursor = pState->GetColor( "Cursor" );

	if(SizeEdge == 0 || pState->GetInt("Height"))
	{
		SizeEdge = pState->GetInt("SizeEdge");
		SetHeight( pState->GetInt("Height") );
	}

	pEdit[0] = pState->GetTexture( "Left" );
	pEdit[1] = pState->GetTexture( "Middle" );
	pEdit[2] = pState->GetTexture( "Right" );
}