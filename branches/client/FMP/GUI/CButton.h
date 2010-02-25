#pragma once

#include "CGUI.h"

class CButton : public CElement
{
	CTimer m_tPressed;

	CTexture * pButton;
	CColor * pString;

	bool m_bVisibleBack;

public:
	CButton( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void SetBackVisible(bool v);
	bool GetBackVisible();

	void UpdateTheme( int iIndex );
};