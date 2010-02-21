#include "CGUI.h"

CText::CText( int X, int Y, int Width, int Height, const char * String, const char * String2, const char * Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );

	SetThemeElement( gpGui->GetThemeElement( "Text" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "Text", 0 );
	else
		SetElementState( "Norm" );
}

void CText::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();
	
	gpGui->GetFont()->DrawString( Pos.GetX(), Pos.GetY(), 0, pString, GetFormatted(), GetWidth() );
}

void CText::PreDraw()
{
	GetString( true );
}

void CText::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetElementState( SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), gpGui->GetFont()->GetStringWidth( GetFormatted().c_str() ), gpGui->GetFont()->GetStringHeight() ) )?"MouseOver":"Norm" );
}

void CText::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
}