#include "CGUI.h"

CCheckBox::CCheckBox(CGUI *Gui, int X, int Y, int Width, int Height, bool Checked, const char * String, const char * String2, tAction Callback )
{
	SetElement(Gui, X, Y, Width, Height, String, String2, Callback );
	SetChecked( Checked );

	SetThemeElement( pGui->GetThemeElement( "CheckBox" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "No color scheme element found.", "CheckBox", 0 );
	else
		SetElementState( "Norm" );
}

bool CCheckBox::GetChecked()
{
	return m_bChecked;
}
void CCheckBox::SetChecked( bool bChecked )
{
	m_bChecked = bChecked;
}

void CCheckBox::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), 15, 15, pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );
	GetFont()->DrawString( Pos.GetX() + 20, Pos.GetY(), 0, pString, GetFormatted() );

	if( GetChecked() )
	{
		D3DCOLOR d3dCrossColor = pCross->GetD3DCOLOR();

		pGui->DrawLine( Pos.GetX(), Pos.GetY(), Pos.GetX() + 14, Pos.GetY() + 14, 1, d3dCrossColor );
		pGui->DrawLine( Pos.GetX() + 14, Pos.GetY(), Pos.GetX(), Pos.GetY() + 14, 1, d3dCrossColor );
	}
}

void CCheckBox::PreDraw()
{
	GetString( true );
}

void CCheckBox::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetElementState( SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetFont()->GetStringWidth( GetFormatted().c_str() ) + 20, GetFont()->GetStringHeight() ) )?"MouseOver":"Norm" );
}

void CCheckBox::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( GetMouseOver() && pGui->GetMouse()->GetLeftButton( 0 ) )
		{
			m_bChecked = !m_bChecked;
			
			if( GetAction() )
				GetAction()( this, SELECT, m_bChecked );
		}
	}
}

void CCheckBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );
	SetFont(pGui->GetFont());

	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pString = pState->GetColor( "String" );
	pCross = pState->GetColor( "Cross" );
}