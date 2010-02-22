#include "CGUI.h"

CButton::CButton( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	SetHeight( BUTTON_HEIGHT );

	SetThemeElement( gpGui->GetThemeElement( "Button" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "Button", 0 );
	else
		SetElementState( "Norm" );
}

void CButton::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	pButton->Draw( Pos, GetWidth(), GetHeight() );
	gpGui->GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_CENTER|FT_VCENTER, pString, GetString().c_str() );
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
		if( GetMouseOver() && gpGui->GetMouse()->GetLeftButton( 0 ) )
		{
			SetElementState( "Pressed" );

			if( GetAction() )
			{
				GetAction()( 0, this );
			}

			m_tPressed.Start( 0.1f );
		}
	}
}

void CButton::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pButton = pState->GetTexture( "Button" );
	pString = pState->GetColor( "String" );
}