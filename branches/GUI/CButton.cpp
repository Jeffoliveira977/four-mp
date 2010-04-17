#include "CGUI.h"

CButton::CButton(CGUI *Gui, int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement(Gui, X, Y, Width, Height, String, String2, Callback );
	SetHeight( BUTTON_HEIGHT );

	m_bVisibleBack = 1;

	SetThemeElement( pGui->GetThemeElement( "Button" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "Button", 0 );
	else
		SetElementState( "Norm" );
}

void CButton::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if(m_bVisibleBack) pButton->Draw( Pos, GetWidth(), GetHeight() );
	GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_CENTER|FT_VCENTER, pString, GetString().c_str() );
}

void CButton::PreDraw()
{
	if( !m_tPressed.Running() )
		SetElementState( GetMouseOver()?"MouseOver":"Norm" );
}

void CButton::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetElementState( SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) )?"MouseOver":"Norm" );
}

void CButton::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( GetMouseOver() && pGui->GetMouse()->GetLeftButton( 0 ) )
		{
			SetElementState( "Pressed" );

			if( GetAction() )
			{
				GetAction()( this, CLICK, 0 );
			}

			m_tPressed.Start( 0.1f );
		}
	}
}

void CButton::SetBackVisible(bool v)
{
	m_bVisibleBack = v;
}

bool CButton::GetBackVisible()
{
	return m_bVisibleBack;
}

void CButton::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	SetFont(pGui->GetFont());

	pButton = pState->GetTexture( "Button" );
	pString = pState->GetColor( "String" );
}
