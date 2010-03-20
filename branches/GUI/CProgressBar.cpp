#include "CGUI.h"

CProgressBar::CProgressBar( int X, int Y, int Width, int Height, int value, const char * String, const char * String2, tAction Callback )
{
	SetElement( X, Y, Width, Height, String, String2, Callback );
	SetProgress( value );

	SetThemeElement( gpGui->GetThemeElement( "ProgressBar" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "Theme element invalid.", "ProgressBar", 0 );
	else
		SetElementState( "Norm" );
}

void CProgressBar::Draw()
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	gpGui->DrawOutlinedBox( Pos.GetX(), Pos.GetY(), GetWidth(), GetHeight(), pInner->GetD3DCOLOR(), pBorder->GetD3DCOLOR() );

	if( GetProgress() )
		pProgressBar->Draw( CPos( Pos.GetX() + 2, Pos.GetY() + 2 ), static_cast<int>( ( static_cast<float>( GetWidth() ) - 4 ) / 100 * GetProgress() ), GetHeight() - 4 );

	GetFont()->DrawString( Pos.GetX() + GetWidth() / 2, Pos.GetY() + GetHeight() / 2, FT_CENTER|FT_VCENTER, pString, m_sBuf );
}

int CProgressBar::GetProgress()
{
	return m_iProgress;
}

void CProgressBar::SetProgress( int iProgress )
{
	if( iProgress > 100 )
		iProgress = 100;
	else if( iProgress < 0 )
		iProgress = 0;

	m_iProgress = iProgress;

	std::stringstream sStream;
	sStream << GetProgress() << static_cast<char>( 37 );
	m_sBuf = sStream.str();

	m_iStrWidth = GetFont()->GetStringWidth( m_sBuf.c_str() ) / 2;
	m_iStrHeight = GetFont()->GetStringHeight() / 2;
}

void CProgressBar::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );
	SetFont(gpGui->GetFont());

	pInner = pState->GetColor( "Inner" );
	pBorder = pState->GetColor( "Border" );
	pString = pState->GetColor( "String" );

	pProgressBar = pState->GetTexture( "ProgressBar" );
}