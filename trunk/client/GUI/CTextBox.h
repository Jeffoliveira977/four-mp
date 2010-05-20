#pragma once

#include "CGUI.h"

class CTextBox : public CElement
{
	int m_iStart;
	int m_iLimit;

	CScrollBar * pSlider;

	std::vector<std::string> m_vStrings;

	CColor * pString, * pInner, * pBorder;
public:
	CTextBox( CGUI *Gui, int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );
	~CTextBox();

	void Draw();
	void PreDraw();
	bool MouseMove( CMouse * pMouse, bool );
	bool KeyEvent( SKey sKey );

	void AddString( std::string sString );
	void RemoveStrings(int Row, int Count  = 1);
	void Clear();

	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );

	void SetMaxStrings(int limit = 128);
	int GetMaxStrings();
};