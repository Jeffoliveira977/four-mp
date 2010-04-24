#include "CGUI.h"

CDropDown::CDropDown( CGUI *Gui, int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( Gui, X, Y, Width, Height, String, String2, Callback );

	m_bDropped = false;
	m_iSelected = m_iMouseOverIndex = 0;
	iEdge = iButton = 0;

	SetThemeElement( pGui->GetThemeElement( "DropDown" ) );

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
		pLeft->Draw( Pos, iEdge, GetHeight() );
		pButton->Draw( Pos.GetX()+GetWidth()-iButton, Pos.GetY(), iButton, GetHeight() );
		pMiddle->Draw( Pos.GetX()+iEdge-1, Pos.GetY(), GetWidth()-iEdge-iButton+2, GetHeight() );

		GetFont()->DrawString( Pos.GetX() + (int)floor((float)iEdge/2), Pos.GetY() + GetHeight() / 2, FT_VCENTER, pString, m_vEntrys[ m_iSelected ].m_sString.c_str() );
		
		if( m_bDropped && m_vEntrys.size() )
		{
			pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY() + GetHeight(), GetWidth(), GetHeight() * m_vEntrys.size(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

			for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			{
				if( iIndex == m_iMouseOverIndex )
				{
					pGui->FillArea( Pos.GetX() + 1, Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth() - 2, GetHeight(),pSelectedInner->GetD3DCOLOR() );
					GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() * ( iIndex + 1 ) + GetHeight() / 2, FT_VCENTER, pSelectedString, m_vEntrys[ iIndex ].m_sString.c_str() );
				}
				else
					GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + GetHeight() * ( iIndex + 1 ) + GetHeight() / 2, FT_VCENTER, pString, m_vEntrys[ iIndex ].m_sString.c_str() );
			}
		}
	}
}

bool CDropDown::MouseMove( CMouse * pMouse, bool over )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos(), mPos = pGui->GetMouse()->GetPos();

	int iHeight = 0;
	if( m_bDropped )
	{
		iHeight = GetHeight() * ( m_vEntrys.size() + 1 );

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			pMouse->LoadPos();
	}
	else
		iHeight = GetHeight();

	bool inArea = pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), iHeight ) && over;
	SetElementState( SetMouseOver( inArea )?"MouseOver":"Norm" );

	if( GetMouseOver() )
		for( int iIndex = 0; iIndex < static_cast<int>( m_vEntrys.size() ); iIndex++ )
			if( pGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY() + GetHeight() * ( iIndex + 1 ), GetWidth(), GetHeight() ) )
			{
				m_iMouseOverIndex = iIndex;
				break;
			}

	pMouse->SetPos( mPos );
	return inArea;
}

bool CDropDown::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( pGui->GetMouse()->GetLeftButton() )
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

				pGui->GetMouse()->SetLeftButton( 0 );
			}
			else
				m_bDropped = false;
		}
	}
	return 0;
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
	pLeft = pState->GetTexture( "Left" );
	pMiddle = pState->GetTexture( "Middle" );

	if(iButton == 0 || pState->GetInt("Height"))
	{
		SetHeight(pState->GetInt("Height"));
		iEdge = pState->GetInt("SizeEdge");
		iButton = pState->GetInt("SizeButton");
	}
}