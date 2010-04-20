#pragma once

#include "CGUI.h"

class CText : public CElement
{
	CColor * pString;
public:
	CText( CGUI *Gui, int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	bool MouseMove( CMouse * pMouse, bool );

	void UpdateTheme( int iIndex );
};