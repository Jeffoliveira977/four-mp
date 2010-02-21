#pragma once

#include "CGUI.h"

class CListBox : public CElement
{
	int m_iMouseOverIndex;

	CHelperSlider * pSlider;

	std::vector<std::string> m_vRows;

	CColor * pInner, * pBorder, * pString, * pMouseOverString;
public:
	CListBox( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void PutStr( std::string sString, int Index );
	std::string GetRow( int iIndex );

	void Clear();
	int GetSize();

	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );
};