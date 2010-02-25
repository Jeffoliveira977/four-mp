#include "CGUI.h"

CDropDown::CDropDown( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	SetHeight( BUTTON_HEIGHT );

	m_bDropped = false;
	m_iSelected = m_iMouseOverIndex = 0;

	SetThemeElement( gpGui->GetThemeElement( "DropDown" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "DropDown", 0 );
	else
		SetElementState( "Norm" );
}

void CDropDown::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SElementState * pState = GetElementState();

	if( pState )
	{
		gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );
		gpGui->DrawLine( Pos.GetX() + GetWidth() - 20, Pos.GetY() + 1, Pos.GetX() + GetWidth() - 20, Pos.GetY() + GetHeight() - 1, 1, pBorder->GetD3DCOLOR() );
		
		gpGui->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString, m_vEntrys[ m_iSelected ].m_sString.c_str() );
		
		if( m_bDropped && m_vEntrys.size() )
		{
			gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY() + GetHeight(), GetWidth(), GetHeight() * m_vEntrys.size(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

			for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			{
				if( iIndex == m_iMouseOverIndex )
				{
					gpGui->FillArea( Pos.GetX() + 1, Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth() - 2, GetHeight(),pSelectedInner->GetD3DCOLOR() );
					gpGui->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() * ( iIndex + 1 ) + GetHeight() / 2, FT_VCENTER, pSelectedString, m_vEntrys[ iIndex ].m_sString.c_str() );
				}
				else
					gpGui->GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() * ( iIndex + 1 ) + GetHeight() / 2, FT_VCENTER, pString, m_vEntrys[ iIndex ].m_sString.c_str() );
			}
		}

		pButton->Draw( CPos( Pos.GetX() + GetWidth() - 19, Pos.GetY() + 1 ), 18, 18 );
	}
}

void CDropDown::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos(), mPos = gpGui->GetMouse()->GetPos();

	int iHeight = 0;
	if( m_bDropped )
	{
		iHeight = GetHeight() * ( m_vEntrys.size() + 1 );

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			pMouse->LoadPos();
	}
	else
		iHeight = GetHeight();

	SetElementState( SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), iHeight ) )?"MouseOver":"Norm" );

	if( GetMouseOver() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			if( gpGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth(), GetHeight() ) )
			{
				m_iMouseOverIndex = iIndex;
				break;
			}

	pMouse->SetPos( mPos );
}

void CDropDown::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( gpGui->GetMouse()->GetLeftButton() )
		{
			if( GetMouseOver() )
			{
				if( m_bDropped )
				{	
					m_iSelected = m_iMouseOverIndex;
					if( GetAction() )
						GetAction()( this, SELECT, m_iSelected );

					m_bDropped = false;
					SetElementState( "Norm" );
				}
				else
				{
					GetParent()->BringToTop( this );

					m_bDropped = true;
					SetElementState( "Pressed" );
				}

				gpGui->GetMouse()->SetLeftButton( 0 );
			}
			else
				m_bDropped = false;
		}
	}
}

void CDropDown::AddElement( std::string sElem, std::string sValue )
{
	m_vEntrys.push_back( SEntry( sElem, sValue ) );
}

std::string CDropDown::GetValue()
{
	return m_vEntrys[ m_iSelected ].m_sValue;
}

void CDropDown::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pSelectedInner = pState->GetColor( "SelectedInner" );
	pSelectedString = pState->GetColor( "SelectedString" );

	pButton = pState->GetTexture( "Button" );
}