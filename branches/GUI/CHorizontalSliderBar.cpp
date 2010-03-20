#include "CGUI.h"

CHorizontalSliderBar::CHorizontalSliderBar()
{
	SetDragged( false );
	SetAction( 0 );
	m_iMinValue = 0, m_iMaxValue = 0, m_iValue = 0;
}

CHorizontalSliderBar::CHorizontalSliderBar( int X, int Y, int Width, int Height, int minValue, int maxValue, int step, int value, const char * String, const char * String2, tAction Callback )
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

void CHorizontalSliderBar::SetSliderElement( int X, int Y, int Width, int Height, int minValue, int maxValue, int step, int value, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );

	if( String )
		SetShowString( false );
	else
		SetShowString( true );

	SetMinValue( minValue );
	SetMaxValue( maxValue );
	SetStep( step );
	SetValue( value );

	/*TiXmlElement * pUpdater = pElement->FirstChildElement( "Updater" );
	if( pUpdater )
	{
		if( pUpdater->GetText() )
		{
			CVar * pCvar = gpGui->Cvars[ pUpdater->GetText() ];

			if( pCvar )
				m_pUpdater = pCvar->GetAction();
			else
				m_pUpdater = 0;
		}
		else
			m_pUpdater = 0;
	}
	else
		m_pUpdater = 0;*/
}

void CHorizontalSliderBar::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SElementState * pState = GetElementState();
	if( pState )
	{
		D3DCOLOR d3dLineColor = pLines->GetD3DCOLOR();

		gpGui->DrawLine( Pos.GetX(),					Pos.GetY() + TITLEBAR_HEIGHT / 2,Pos.GetX() + GetWidth(),		Pos.GetY() + TITLEBAR_HEIGHT / 2,		1, d3dLineColor );
		gpGui->DrawLine( Pos.GetX(),					Pos.GetY() + TITLEBAR_HEIGHT / 4, Pos.GetX(),					Pos.GetY() + TITLEBAR_HEIGHT / 4 * 3,	1, d3dLineColor );
		gpGui->DrawLine( Pos.GetX() + GetWidth(),		Pos.GetY() + TITLEBAR_HEIGHT / 4, Pos.GetX() + GetWidth(),		Pos.GetY() + TITLEBAR_HEIGHT / 4 * 3,	1, d3dLineColor );
		gpGui->DrawLine( Pos.GetX() + GetWidth() / 2,	Pos.GetY() + TITLEBAR_HEIGHT / 4, Pos.GetX() + GetWidth() / 2,	Pos.GetY() + TITLEBAR_HEIGHT / 4 * 3,	1, d3dLineColor );

		pSlider->Draw( CPos( Pos.GetX() + static_cast<int>( floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( GetValue() - GetMinValue() ) ) ) - 5, Pos.GetY() + 2 ), 10, TITLEBAR_HEIGHT - 4 );

		GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() - 15, FT_CENTER, pString, GetFormatted() );
	}
}

void CHorizontalSliderBar::PreDraw()
{
	return;
	if( !GetFormatted().length() )
		GetString( !GetShowString() );

	if( m_pUpdater )
		m_pUpdater( this, CHANGE, GetValue() );
}

void CHorizontalSliderBar::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	if( GetDragged() )
	{
		CPos mousePos = pMouse->GetPos();

		if( mousePos.GetX() == -1 && mousePos.GetY() == -1 )
			mousePos = pMouse->GetSavedPos();

			if( mousePos.GetX() < Pos.GetX() )
				SetValue( GetMinValue() );
			else if( mousePos.GetX() > Pos.GetX() + GetWidth() )
				SetValue( GetMaxValue() );
			else
			{
				for( int iIndex = GetMinValue(); iIndex < GetMaxValue(); iIndex++ )
					if( mousePos.GetX() >= Pos.GetX() + floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( iIndex - GetMinValue() ) ) && mousePos.GetX() <= Pos.GetX() + floor( static_cast<float>( GetWidth() ) / ( GetMaxValue() - GetMinValue() ) * ( iIndex + 1 - GetMinValue() ) ) )
					{
						SetValue( iIndex );
						break;
					}
			}

			if( GetAction() )
				GetAction()( this, CHANGE, GetValue() );
	}
	else
		SetElementState( SetMouseOver( gpGui->GetMouse()->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), TITLEBAR_HEIGHT ) )?"MouseOver":"Norm" );
}

void CHorizontalSliderBar::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		SetDragged( GetMouseOver() && gpGui->GetMouse()->GetLeftButton() );

		SetElementState( GetDragged()?"Pressed":( GetMouseOver()?"MouseOver":"Norm" ) );

		if( GetDragged() )
			MouseMove( gpGui->GetMouse() );
	}
}

int CHorizontalSliderBar::GetMinValue()
{
	return m_iMinValue;
}

int CHorizontalSliderBar::GetMaxValue()
{
	return m_iMaxValue;
}

int CHorizontalSliderBar::GetValue()
{
	return m_iValue;
}

int CHorizontalSliderBar::GetStep()
{
	return m_iStep;
}

void CHorizontalSliderBar::SetMinValue( int iMinValue )
{
	m_iMinValue = iMinValue;
}

void CHorizontalSliderBar::SetMaxValue( int iMaxValue )
{
	m_iMaxValue = iMaxValue;
}

void CHorizontalSliderBar::SetValue( int iValue )
{
	if( iValue > GetMaxValue() )
		m_iValue = GetMaxValue();
	else if( iValue < GetMinValue() )
		m_iValue = GetMinValue();
	else
		m_iValue = iValue;

	if( GetShowString() )
	{
		std::stringstream sStream;
		sStream << GetValue() << "/" << GetMaxValue();
		SetString( sStream.str() );
		GetString();
	}
	else
		GetString( true );
}

void CHorizontalSliderBar::SetStep( int iStep )
{
	m_iStep = iStep;
}

bool CHorizontalSliderBar::GetDragged()
{
	return m_bDragged;
}

void CHorizontalSliderBar::SetDragged( bool bDragged )
{
	m_bDragged = bDragged;

	if( m_bDragged )
		SetElementState( "Pressed" );
}

void CHorizontalSliderBar::SetShowString( bool bShow )
{
	m_bShowString = bShow;
}

bool CHorizontalSliderBar::GetShowString()
{
	return m_bShowString;
}

void CHorizontalSliderBar::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );
	SetFont(gpGui->GetFont());

	pLines = pState->GetColor( "Lines" );
	pString = pState->GetColor( "String" );

	pSlider = pState->GetTexture( "Slider" );
}

// SliderBar specific GUI cfunctions
std::string MinValue( const char *, CElement * pElement )
{
	std::stringstream sStream;

	if( pElement )
	{
		CHorizontalSliderBar * pSlider = (CHorizontalSliderBar*)pElement;

		try
		{
			sStream << pSlider->GetMinValue();
		}
		catch( ... )
		{
			MessageBoxA( 0, "$MinValue failed", pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}

std::string MaxValue( const char *, CElement * pElement )
{
	std::stringstream sStream;

	if( pElement )
	{
		CHorizontalSliderBar * pSlider = (CHorizontalSliderBar*)pElement;

		try
		{
			sStream << pSlider->GetMaxValue();
		}
		catch( ... )
		{
			MessageBoxA( 0, "$MaxValue failed", pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}

std::string SliderValue( const char *, CElement * pElement )
{
	std::stringstream sStream;

	if( pElement )
	{
		CHorizontalSliderBar * pSlider = (CHorizontalSliderBar*)pElement;

		try
		{
			sStream << pSlider->GetValue();
		}
		catch( ... )
		{
			MessageBoxA( 0, "$Value failed", pElement->GetString().c_str(), 0 );
		}
	}
	return sStream.str();
}
// SliderBar specific GUI cfunctions