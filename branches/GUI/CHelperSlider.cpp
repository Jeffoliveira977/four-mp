#include "CGUI.h"

void CHelperSlider::SetDragged( bool bDragged )
{
	m_bDragged = bDragged;
}

bool CHelperSlider::GetDragged()
{
	return m_bDragged;
}

CHelperSlider::CHelperSlider(CGUI *Gui, CPos relPos, int iHeight )
{
	pGui = Gui;
	SetRelPos( relPos );
	SetHeight( iHeight );

	Show = 0;

	m_bMouseOver[ 0 ] = m_bMouseOver[ 1 ] = m_bMouseOver[ 2 ] = false;
	SetDragged( false );

	SetThemeElement( pGui->GetThemeElement( "HelperSlider" ) );
	SetThemeElement( pGui->GetThemeElement( "HelperSlider" ), 2 );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "HelperSlider", 0 );
	else
	{
		SetElementState( "Norm" );
		SetElementState( "Norm", 2 );
	}

	SetThemeElement( pGui->GetThemeElement( "HorizontalSliderBar" ), 1 );

	if( !GetThemeElement( 1 ) )
		MessageBoxA( 0, "Theme element invalid.", "HorizontalSliderBar", 0 );
	else
		SetElementState( "Norm", 1 );
}

void CHelperSlider::Draw( CPos basePos )
{
	if(!Show) return;
	CPos Pos = basePos + *GetRelPos();

	pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), HELPERSLIDER_WIDTH, GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	pUpArrow->Draw( CPos( Pos.GetX() + 1, Pos.GetY() + 1 ), HELPERSLIDER_WIDTH - 2, HELPERSLIDER_WIDTH - 2 );

	pDownArrow->Draw( CPos( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - HELPERSLIDER_WIDTH ), HELPERSLIDER_WIDTH - 2, HELPERSLIDER_WIDTH - 2 );

	if( GetMaxValue() < 2 )
		return;

	pSlider->Draw( CPos( Pos.GetX() + 1, Pos.GetY() - 1 + HELPERSLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( ( HELPERSLIDER_WIDTH - 2 ) * 3 ) ) ) / GetMaxValue() * GetValue() ) ) ), HELPERSLIDER_WIDTH - 2, HELPERSLIDER_WIDTH - 2 );
}

void CHelperSlider::PreDraw()
{
	if(!Show) return;
	if( m_bPressed[ 0 ] && !m_tUpArrow.Running() )
	{
		m_bPressed[ 0 ] = false;
		SetElementState( m_bMouseOver[ 0 ]?"MouseOver":"Norm" );
	}
	if( m_bPressed[ 1 ] && !m_tDownArrow.Running() )
	{
		m_bPressed[ 1 ] = false;
		SetElementState( m_bMouseOver[ 1 ]?"MouseOver":"Norm", 2 );
	}
}

void CHelperSlider::MouseMove( CPos basePos, CMouse * pMouse )
{
	if(!Show) return;
	CPos Pos = basePos + *GetRelPos();

	SetElementState( ( ( m_bMouseOver[ 0 ]	= pMouse->InArea( Pos.GetX() + 1, Pos.GetY() + 1, HELPERSLIDER_WIDTH - 2, HELPERSLIDER_WIDTH - 2 ) ) != 0 )?"MouseOver":"Norm" );
	SetElementState( ( ( m_bMouseOver[ 1 ]	= pMouse->InArea( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - HELPERSLIDER_WIDTH, HELPERSLIDER_WIDTH - 2, HELPERSLIDER_WIDTH - 2 ) ) != 0 )?"MouseOver":"Norm", 2 );

	if( GetMaxValue() < 2 )
		return;

	if( GetDragged() )
	{
		CPos mPos = pMouse->GetPos();

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			mPos = pMouse->GetSavedPos();

		if( mPos.GetY() < Pos.GetY() + HELPERSLIDER_WIDTH )
			SetValue( GetMinValue() );
		else if( mPos.GetY() > Pos.GetY() + GetHeight() - HELPERSLIDER_WIDTH )
			SetValue( GetMaxValue() );
		else
			for( int i = GetMinValue(); i <= GetMaxValue(); i++ )
			{
				int iY = Pos.GetY() - 1 + HELPERSLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( ( HELPERSLIDER_WIDTH - 2 ) * 3 ) ) ) / GetMaxValue() * i ) );
				if( mPos.GetY() >= iY && mPos.GetY() <= iY + HELPERSLIDER_WIDTH / 2 )
				{
					SetValue( i );
					break;
				}
			}
	}
	else
		SetElementState( ( ( m_bMouseOver[ 2 ]	= pMouse->InArea( Pos.GetX(),  Pos.GetY() + HELPERSLIDER_WIDTH + static_cast<int>( floor( static_cast<float>( ( GetHeight() - ( HELPERSLIDER_WIDTH * 3 ) ) ) / GetMaxValue() * GetValue() ) ), HELPERSLIDER_WIDTH, HELPERSLIDER_WIDTH ) ) != 0 )?"MouseOver":"Norm", 1 );
}

bool CHelperSlider::KeyEvent( CPos basePos, SKey sKey )
{
	CPos Pos = basePos + *GetRelPos();

	if( !sKey.m_vKey )
	{
		if( pGui->GetMouse()->GetLeftButton() )
		{
			if( m_bMouseOver[ 0 ] )
			{
				if( GetValue() > GetMinValue() )
					SetValue( GetValue() - 1 );

				m_bPressed[ 0 ] = true;
				SetElementState( "Pressed" );

				m_tUpArrow.Start( 0.1f );
			}
			else if( m_bMouseOver[ 1 ] )
			{
				if( GetValue() < GetMaxValue() )
					SetValue( GetValue() + 1 );

				m_bPressed[ 1 ] = true;
				SetElementState( "Pressed", 2 );

				m_tDownArrow.Start( 0.1f );
			}
			else if( pGui->GetMouse()->InArea( Pos.GetX() + 1, Pos.GetY() + HELPERSLIDER_WIDTH, HELPERSLIDER_WIDTH, GetHeight() - HELPERSLIDER_WIDTH * 2 ) )
			{	
				SetElementState( "Pressed", 1 );

				SetDragged( true );
				MouseMove( Pos, pGui->GetMouse() );
			}
		}
		else
		{
			SetElementState( m_bMouseOver[ 2 ]?"MouseOver":"Norm", 1 );
			SetDragged( false );
		}

		if( !GetDragged() && pGui->GetMouse()->GetWheel() )
		{
			int iState = pGui->GetMouse()->GetWheel( 0 );

			int iTenth = ( GetMaxValue() / 10 )?GetMaxValue() / 10:1;

			if( iState == 1 )
				SetValue( GetValue() - iTenth );
			else if( iState == 2 )
				SetValue( GetValue() + iTenth );	
		}
	}
	return true;
}

void CHelperSlider::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	if( !iIndex )
	{
		pInner = pState->GetColor( "Inner" );
		pBorder = pState->GetColor( "Border" );

		pUpArrow = pState->GetTexture( "UpArrow" );
	}
	else if( iIndex == 1 )
		pSlider = pState->GetTexture( "Slider" );
	else
		pDownArrow = pState->GetTexture( "DownArrow" );
}