#include "CGUI.h"

CCheckBox::CCheckBox( CGUI *Gui, int X, int Y, int Width, int Height, bool Checked, const char * String, const char * String2, tAction Callback )
{
	SetElement( Gui, X, Y, Width, Height, String, String2, Callback );
	SetChecked( Checked );

	SetThemeElement( pGui->GetThemeElement( "CheckBox" ) );

	if( !GetThemeElement() )
		MessageBoxA( 0, "No color scheme element found.", "CheckBox", 0 );
	else
		SetElementState( "Norm" );
}

CCheckBox::~CCheckBox()
{
	SAFE_DELETE(pString);
	SAFE_DELETE(tCheck);
	SAFE_DELETE(tChecked);
}

bool CCheckBox::GetChecked()
{
	return m_bChecked;
}
void CCheckBox::SetChecked( bool bChecked )
{
	m_bChecked = bChecked;
}

void CCheckBox::Draw()
{
	CPos Pos = *GetRelPos();
	if(GetParent()) Pos = *GetParent()->GetAbsPos() + Pos;

	GetFont()->DrawString( Pos.GetX() + 20, Pos.GetY(), 0, pString, GetFormatted() );

	if( GetChecked() )
		tChecked->Draw(Pos, GetWidth(), GetHeight());
	else
		tCheck->Draw(Pos, GetWidth(), GetHeight());
}

void CCheckBox::PreDraw()
{
	GetString( true );
}

bool CCheckBox::MouseMove( CMouse * pMouse, bool over )
{
	CPos Pos = *GetParent()->GetAbsPos() + *GetRelPos();

	bool inArea = pMouse->InArea( Pos.GetX(), Pos.GetY(), GetFont()->GetStringWidth( GetFormatted().c_str() ) + GetWidth(), GetFont()->GetStringHeight() ) && over;
	SetElementState( SetMouseOver( inArea )?"MouseOver":"Norm" );
	return inArea;
}

bool CCheckBox::KeyEvent( SKey sKey )
{
	if( !sKey.m_vKey )
	{
		if( GetMouseOver() && pGui->GetMouse()->GetLeftButton( 0 ) )
		{
			m_bChecked = !m_bChecked;
			
			if( GetAction() )
				GetAction()( this, SELECT, m_bChecked );
		}
	}
	return 0;
}

void CCheckBox::UpdateTheme( int iIndex )
{
	SElementState * pState = GetElementState( iIndex );

	pString = pState->GetColor( "String" );
	tCheck = pState->GetTexture("CheckBox");
	tChecked = pState->GetTexture("CheckBoxChecked");

	if(GetWidth() == 0)
	{
		SetWidth(pState->GetInt("Width"));
		SetHeight(pState->GetInt("Height"));
	}
}