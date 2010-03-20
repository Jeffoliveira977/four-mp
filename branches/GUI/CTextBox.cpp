#include "CGUI.h"

CTextBox::CTextBox( int X, int Y, int Width, int Height, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );

	pSlider = new CHelperSlider( CPos( GetWidth() - HELPERSLIDER_WIDTH + 2, 0 ), GetHeight() );

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
	if( m_vStrings.size() )
		for( int i = pSlider->GetValue(), iHeight = 0; i <= pSlider->GetMaxValue() && iHeight < GetHeight() - GetFont()->GetStringHeight(); i++ )
		{
			GetFont()->DrawString( Pos.GetX() + 3, Pos.GetY() + iHeight, 0, pString, m_vStrings[ i ], GetWidth() - HELPERSLIDER_WIDTH );
			iHeight += iAddHeight;
		}

	pSlider->Draw( Pos );
}

void CTextBox::PreDraw()
{
	pSlider->PreDraw();
}

void CTextBox::MouseMove( CMouse * pMouse )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	SetMouseOver( pMouse->InArea( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight() ) );

	pSlider->MouseMove( Pos, pMouse );
}

void CTextBox::KeyEvent( SKey sKey )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();
	if(GetMouseOver() && gpGui->GetMouse()->GetLeftButton())
		SendMsg(CLICK, 0);

	if( GetMouseOver() || ( !sKey.m_bDown && !gpGui->GetMouse()->GetWheel() )  )
		pSlider->KeyEvent( Pos, sKey );
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

	pSlider->SetMaxValue( m_vStrings.size() );
	m_vStrings.push_back( sString.c_str() );

	int iHeight = 0;
	for( int i = pSlider->GetValue(); i <= pSlider->GetMaxValue(); i++ )
	{
		float fWidth = static_cast<float>( GetFont()->GetStringWidth( m_vStrings[ i ].c_str() ) );
		int iLines = static_cast<int>( ceilf( fWidth / ( GetWidth() - HELPERSLIDER_WIDTH ) ) );

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
	SetFont(gpGui->GetFont());

	pString = pState->GetColor( "String" );
	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
}

void CTextBox::ShowSlider( bool bShow )
{
	pSlider->Show = bShow;
}