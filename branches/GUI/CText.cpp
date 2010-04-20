#include "CGUI.h"

CText::CText( CGUI *Gui, int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( Gui, X, Y, Width, Height, String, String2, Callback );

	SetThemeElement( pGui->GetThemeElement( "Text" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "Text", 0 );
	else
		SetElementState( "Norm" );
}

void CText::Draw()
{
	CPos Pos = *GetRelPos();
	if(GetParent()) Pos = *GetParent()->GetAbsPos() + Pos;
	
	GetFont()->DrawString( Pos.GetX(), Pos.GetY(), 0, pString, GetFormatted(), GetWidth() );
}

void CText::PreDraw()
{
	GetString( true );
}

bool CText::MouseMove( CMouse * pMouse, bool )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetElementState( SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetFont()->GetStringWidth( GetFormatted().c_str() ), GetFont()->GetStringHeight() ) )?"MouseOver":"Norm" );
	return 0;
}

void CText::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
}