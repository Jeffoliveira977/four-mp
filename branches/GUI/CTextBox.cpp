#include "CGUI.h"

CTextBox::CTextBox( CGUI *Gui, int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	m_iLimit = 128;
	SetElement( Gui, X, Y, Width, Height, String, String2, Callback );

	pSlider = new CScrollBar( Gui, CPos( GetWidth() + 2, 0 ), GetHeight() );

	SetThemeElement( gpGui->GetThemeElement( "TextBox" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "TextBox", 0 );
	else
		SetElementState( "Norm" );

}

void CTextBox::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	int iAddHeight = GetFont()->GetStringHeight();
	int iMaxHeight = GetHeight() - iAddHeight;
	if( m_vStrings.size() )
		for( int i = pSlider->GetMaxValue(), iHeight = 0; i >= pSlider->GetValue() && iHeight < iMaxHeight; i-- )
		{
			GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + iHeight, 0, pString, m_vStrings[ i ], GetWidth() - pSlider->GetSliderWidth() );
			iHeight += iAddHeight;
		}

	pSlider->Draw( Pos );
}

void CTextBox::PreDraw()
{
	pSlider->PreDraw();
}

bool CTextBox::MouseMove( CMouse * pMouse, bool over )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	bool inArea = pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) && over;
	SetMouseOver( inArea );

	pSlider->MouseMove( Pos, pMouse );
	return inArea;
}

bool CTextBox::KeyEvent( SKey sKey )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();
	if(GetMouseOver() && gpGui->GetMouse()->GetLeftButton())
		SendMsg(CLICK, 0);

	if( GetMouseOver() || ( !sKey.m_bDown && !gpGui->GetMouse()->GetWheel() )  )
		pSlider->KeyEvent( Pos, sKey );
	
	return 0;
}

void CTextBox::AddString( std::string sString )
{
	if( !sString.length() )
		return;

	std::vector<std::string> vPending;
	int iLength = static_cast<int>( sString.length() );
	for( int i = iLength - 1; i > 0; i-- )
	{
		if( sString[ i ] == '\n' )
		{
			sString[ i ] = '\0';
			
			if( i + 1 < iLength )
			{
				if( sString[ i + 1 ] == '\r' )
				{
					if( i + 2 < iLength )
						vPending.push_back( &sString.c_str()[ i + 2 ] );
				}
				else
					vPending.push_back( &sString.c_str()[ i + 1 ] );
			}
		}
	}

	m_vStrings.insert( m_vStrings.begin(), sString.c_str() );

	if((int)m_vStrings.size() >= m_iLimit)
		RemoveStrings(m_iLimit, 0);

	pSlider->SetMaxValue( m_vStrings.size()-1 );

	int iHeight = 0;
	for( int i = pSlider->GetValue(); i <= pSlider->GetMaxValue(); i++ )
	{
		float fWidth = static_cast<float>( GetFont()->GetStringWidth( m_vStrings[ i ].c_str() ) );
		int iLines = static_cast<int>( ceilf( fWidth / ( GetWidth() -  pSlider->GetSliderWidth() ) ) );

		int iTempHeight = iLines*GetFont()->GetStringHeight();
		iHeight += iTempHeight;
	
		while( iHeight > GetHeight() - GetFont()->GetStringHeight() )
		{
			pSlider->SetValue( pSlider->GetValue() + iLines );
			iHeight -= iTempHeight;
		}
	}

	for( std::vector<std::string>::reverse_iterator iIter = vPending.rbegin(); iIter != vPending.rend(); iIter++ )
		AddString( *iIter );

}

void CTextBox::Clear()
{
	m_vStrings.clear();
}

void CTextBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
}

void CTextBox::ShowSlider(bool bShow)
{
	pSlider->SetShow(bShow);
}

void CTextBox::SetMaxStrings(int limit)
{
	m_iLimit = limit;
}

int CTextBox::GetMaxStrings()
{
	return m_iLimit;
}

void CTextBox::RemoveStrings(int Row, int Count)
{
	if((int)m_vStrings.size() > Row)
		m_vStrings.erase(m_vStrings.begin()+Row, (!Count?m_vStrings.end():m_vStrings.begin()+Row+Count));
}