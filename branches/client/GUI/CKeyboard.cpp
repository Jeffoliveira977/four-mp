#include "CGUI.h"

CKeyboard::CKeyboard(CGUI *Gui)
{
	pGui = Gui;
}

bool CKeyboard::HandleMessage( unsigned int uMsg, __w64 unsigned int wParam, _w64 long lParam )
{
	if( !pGui->IsVisible() || uMsg < WM_KEYFIRST || uMsg > WM_KEYLAST || pGui->GetMouse()->GetLeftButton() )
		return false;

	switch( uMsg )
	{
	case WM_KEYDOWN:
		SetKey( SKey( static_cast<char>( wParam ), true, lParam ) );
		break;
	case WM_KEYUP:
		SetKey( SKey( static_cast<char>( wParam ), false, lParam ) );
		break;
	}

	return pGui->KeyEvent( GetKey() );
}

void CKeyboard::SetKey( SKey sKey )
{
	m_sKey = sKey;
}

SKey CKeyboard::GetKey()
{
	SKey sRet = m_sKey;
	SetKey( SKey( 0, false ) );
	return sRet;
}