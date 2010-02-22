#include "CGUI.h"

CVerticalSliderBar::CVerticalSliderBar( int X, int Y, int Width, int Height, int minValue, int maxValue, int step, int value, const char * String, const char * String2, tAction Callback )
{
	SetDragged( false );
	SetAction( 0 );
	m_iMinValue = 0, m_iMaxValue = 0, m_iValue = 0;

	SetSliderElement( X, Y, Width, Height, minValue, maxValue, step, value, String, String2, Callback );

	SetThemeElement( gpGui->GetThemeElement( "HorizontalSliderBar" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "HorizontalSliderBar", 0 );
	else
		SetElementState( "Norm" );
}

void CVerticalSliderBar::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	D3DCOLOR d3dLineColor = pLines->GetD3DCOLOR();
	
	gpGui->DrawLine( Pos.GetX() + TITLEBAR_HEIGHT / 2, Pos.GetY(),					Pos.GetX() + TITLEBAR_HEIGHT / 2,		Pos.GetY() + GetHeight(), 1,		d3dLineColor );
	gpGui->DrawLine( Pos.GetX() + TITLEBAR_HEIGHT / 4, Pos.GetY(),					Pos.GetX() + TITLEBAR_HEIGHT / 4 * 3,	Pos.GetY(), 1,						d3dLineColor );
	gpGui->DrawLine( Pos.GetX() + TITLEBAR_HEIGHT / 4, Pos.GetY() + GetHeight(),		Pos.GetX() + TITLEBAR_HEIGHT / 4 * 3,	Pos.GetY() + GetHeight(), 1,		d3dLineColor );
	gpGui->DrawLine( Pos.GetX() + TITLEBAR_HEIGHT / 4, Pos.GetY() + GetHeight() / 2,	Pos.GetX() + TITLEBAR_HEIGHT / 4 * 3,	Pos.GetY() + GetHeight() / 2, 1,	d3dLineColor );

	pSlider->Draw( CPos( Pos.GetX() + 2, Pos.GetY() + GetHeight() - static_cast<int>( floor( static_cast<float>( GetHeight() ) / GetMaxValue() * GetValue() ) ) - 5 ), BUTTON_HEIGHT, 10 );

	gpGui->GetFont()->DrawString( Pos.GetX() - TITLEBAR_HEIGHT + ( TITLEBAR_HEIGHT * 3 ) / 2, Pos.GetY() + GetHeight() + 5, FT_CENTER, pString, GetFormatted() );
}

void CVerticalSliderBar::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if( GetDragged() )
	{
		CPos mousePos = pMouse->GetPos();

		if( mousePos.GetX() == -1 && mousePos.GetY() == -1 )
			mousePos = pMouse->GetSavedPos();

			if( mousePos.GetY() < Pos.GetY() )
				SetValue( GetMaxValue() );
			else if( mousePos.GetY() > Pos.GetY() + GetHeight() )
				SetValue( GetMinValue() );
			else
			{
				for( int iIndex = GetMinValue(); iIndex < GetMaxValue(); iIndex++ )
					if( mousePos.GetY() >= Pos.GetY() + floor( static_cast<float>( GetHeight() ) / GetMaxValue() * iIndex ) && mousePos.GetY() <= Pos.GetY() + floor( (float)GetHeight() / GetMaxValue() * ( iIndex + 1 ) ) )
					{
						SetValue( GetMaxValue() - iIndex );
						break;
					}
			}

			if( GetAction() )
				GetAction()( reinterpret_cast<const char*>( GetValue() ), this );
	}
	else
		SetElementState( SetMouseOver( gpGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY(), TITLEBAR_HEIGHT, GetHeight() ) )?"MouseOver":"Norm" );
}

void CVerticalSliderBar::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pLines = pState->GetColor( "Lines" );
	pString = pState->GetColor( "String" );

	pSlider = pState->GetTexture( "Slider" );
}