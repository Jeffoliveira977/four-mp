#pragma once

#include "CGUI.h"

class CCheckBox : public CElement
{
	bool m_bChecked;

	CColor * pInner, * pBorder, * pString, * pCross;

public:
	CCheckBox( int X, int Y, int Width, int Height, bool Checked = 0, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	bool GetChecked();
	void SetChecked( bool bChecked );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void UpdateTheme( int iIndex );
};