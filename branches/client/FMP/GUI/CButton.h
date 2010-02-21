#pragma once

#include "CGUI.h"

class CButton : public CElement
{
	CTimer m_tPressed;

	CTexture * pButton;
	CColor * pString;

public:
	CButton( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void UpdateTheme( int iIndex );
};