#pragma once

#include "CGUI.h"

class CTextBox : public CElement
{
	int m_iStart;

	bool m_bUpArrow_MouseOver, m_bDownArrow_MouseOver, m_bSlider_MouseOver, m_bDragging;
	CTimer m_tUpArrow, m_tDownArrow;

	CHelperSlider * pSlider;

	std::vector<std::string> m_vStrings;

	CColor * pString, * pInner, * pBorder;
public:
	CTextBox(CGUI *Gui, int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void AddString( std::string sString );
	void Clear();

	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );
};