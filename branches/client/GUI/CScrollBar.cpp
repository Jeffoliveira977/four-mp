#include "CGUI.h"

void CScrollBar::SetDragged( bool bDragged )
{
	m_bDragged = bDragged;
}

bool CScrollBar::GetDragged()
{
	return m_bDragged;
}

CScrollBar::CScrollBar( CGUI *Gui, CPos relPos, int inHeight )
{
	SetElement( Gui, 0, 0, 10, inHeight, NULL, NULL, NULL );
	SetHeight( inHeight );

	ibHeight = ibWidth = isHeight = 0;

	m_bShow = 1;

	m_bMouseOver[ 0 ] = m_bMouseOver[ 1 ] = m_bMouseOver[ 2 ] = false;
	SetDragged( false );

	SetThemeElement( pGui->GetThemeElement( "ScrollBar" ) );
	SetThemeElement( pGui->GetThemeElement( "ScrollBar" ), 1 );
	SetThemeElement( pGui->GetThemeElement( "ScrollBar" ), 2 );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "ScrollBar", 0 );
	else
	{
		SetElementState( "Norm" );
		SetElementState( "Norm", 1 );
		SetElementState( "Norm", 2 );
	}

	SetRelPos( CPos(relPos.GetX() - ibWidth, relPos.GetY()) );

	SetMinValue( 0 );
	SetMaxValue( 1 );
	SetStep( 1 );
	SetValue( 0 );
}

int CScrollBar::GetWidth()
{
	return ibWidth;
}

void CScrollBar::Draw( CPos basePos )
{
	if(!GetShow()) return;
	CPos Pos = basePos + *GetRelPos();

	pGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), ibWidth, GetHeight(), back->GetD3DCOLOR(), back->GetD3DCOLOR() );

	pUpArrow->Draw( CPos( Pos.GetX() + 1, Pos.GetY() + 1 ), ibWidth - 2, ibHeight - 2 );
	pDownArrow->Draw( CPos( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - ibHeight ), ibWidth - 2, ibHeight - 2 );

	if( GetMaxValue() < 2 )
		return;

	pSlider->Draw( Pos.GetX() + 1, Pos.GetY() - 1 + ibHeight + static_cast<int>( floor( (float)(GetHeight() - 2*ibHeight - isHeight) / GetMaxValue() * GetValue() ) ), ibWidth - 2, isHeight - 2 );
}

void CScrollBar::PreDraw()
{
	if(!GetShow()) return;
	if( (m_bPressed[ 0 ] && !pGui->GetMouse()->GetLeftButton()) || !m_bMouseOver[ 0 ] )
	{
		m_bPressed[ 0 ] = false;
		SetElementState( m_bMouseOver[ 0 ]?"MouseOver":"Norm" );
	}
	if( (m_bPressed[ 2 ] && !pGui->GetMouse()->GetLeftButton()) || m_bMouseOver[ 2 ] )
	{
		m_bPressed[ 2 ] = false;
		SetElementState( m_bMouseOver[ 2 ]?"MouseOver":"Norm", 2 );
	}
}

bool CScrollBar::MouseMove( CPos basePos, CMouse * pMouse )
{
	if(!GetShow()) return 0;
	
	CPos Pos = basePos + *GetRelPos();
	SetElementState( ( ( m_bMouseOver[ 0 ]	= pMouse->InArea( Pos.GetX() + 1, Pos.GetY() + 1, ibWidth - 2, ibHeight - 2 ) ) != 0 )?"MouseOver":"Norm" );
	SetElementState( ( ( m_bMouseOver[ 2 ]	= pMouse->InArea( Pos.GetX() + 1, Pos.GetY() + 1 + GetHeight() - ibHeight, ibWidth - 2, ibHeight - 2 ) ) != 0 )?"MouseOver":"Norm", 2 );

	if( GetMaxValue() < 2 )
		return 0;

	if( GetDragged() )
	{
		CPos mPos = pMouse->GetPos();

		if( mPos.GetX() == -1 && mPos.GetY() == -1 )
			mPos = pMouse->GetSavedPos();

		if( mPos.GetY() < Pos.GetY() + isHeight )
			SetValue( GetMinValue() );
		else if( mPos.GetY() > Pos.GetY() + GetHeight() - isHeight )
			SetValue( GetMaxValue() );
		else
			for( int i = GetMinValue(); i <= GetMaxValue(); i++ )
			{
				int iY = Pos.GetY() - 1 + ibHeight + static_cast<int>( floor( (float)( GetHeight() - ibHeight * 2 - isHeight) / GetMaxValue() * i ) );
				if( mPos.GetY() >= iY && mPos.GetY() <= iY + isHeight / 2 )
				{
					SetValue( i );
					break;
				}
			}
	}
	else
		SetElementState( ( ( m_bMouseOver[ 1 ]	= pMouse->InArea( Pos.GetX(),  Pos.GetY() + ibHeight + static_cast<int>( floor( (float)( GetHeight() - 2 * ibHeight - isHeight) / GetMaxValue() * GetValue() ) ), ibWidth, isHeight ) ) != 0 )?"MouseOver":"Norm", 1 );

	return 0;
}

bool CScrollBar::KeyEvent( CPos basePos, SKey sKey )
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
			}
			else if( m_bMouseOver[ 2 ] )
			{
				if( GetValue() < GetMaxValue() )
					SetValue( GetValue() + 1 );

				m_bPressed[ 2 ] = true;
				SetElementState( "Pressed", 2 );
			}
			else if( pGui->GetMouse()->InArea( Pos.GetX() + 1, Pos.GetY() + ibHeight, ibWidth, GetHeight() - ibHeight * 2 ) )
			{	
				SetElementState( "Pressed", 1 );
				m_bPressed[ 1 ] = true;

				SetDragged( true );
				MouseMove( Pos, pGui->GetMouse() );
			}
		}
		else
		{
			SetElementState( m_bMouseOver[ 1 ]?"MouseOver":"Norm", 1 );
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

int CScrollBar::GetMinValue()
{
	return m_iMinValue;
}

int CScrollBar::GetMaxValue()
{
	return m_iMaxValue;
}

int CScrollBar::GetValue()
{
	return m_iValue;
}

int CScrollBar::GetStep()
{
	return m_iStep;
}

void CScrollBar::SetMinValue( int iMinValue )
{
	m_iMinValue = iMinValue;
}

void CScrollBar::SetMaxValue( int iMaxValue )
{
	m_iMaxValue = iMaxValue;
}

void CScrollBar::SetValue( int iValue )
{
	if( iValue > GetMaxValue() )
		m_iValue = GetMaxValue();
	else if( iValue < GetMinValue() )
		m_iValue = GetMinValue();
	else
		m_iValue = iValue;

	SendMsg(CHANGE, iValue);
}

void CScrollBar::SetStep( int iStep )
{
	m_iStep = iStep;
}

void CScrollBar::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	if( iIndex == 1 )
	{
		if(ibHeight == 0)
		{
			ibHeight = pState->GetInt("bHeight");
			ibWidth = pState->GetInt("bWidth");
			isHeight = pState->GetInt("sHeight");
			back = pState->GetColor( "BG" );
		}
		pSlider = pState->GetTexture( "Scroll" );
	}
	else if( iIndex == 0 )
	{
		pUpArrow = pState->GetTexture( "ArrowUp" );
	}
	else if(iIndex == 2)
	{
		pDownArrow = pState->GetTexture( "ArrowDown" );
	}
}

std::string MinValue( const char *, CElement * pElement )
{
	std::stringstream sStream;

	if( pElement )
	{
		CScrollBar * pSlider = (CScrollBar*)pElement;

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
		CScrollBar * pSlider = (CScrollBar*)pElement;

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
		CScrollBar * pSlider = (CScrollBar*)pElement;

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

void CScrollBar::SetShow(bool bShow)
{
	m_bShow = bShow;
}

bool CScrollBar::GetShow()
{
	return m_bShow;
}

int CScrollBar::GetSliderWidth()
{
	return ibHeight;
}