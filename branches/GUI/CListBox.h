#pragma once

#include "CGUI.h"

class CListBox : public CElement
{
	int m_iMouseOverIndex;
	int m_iMouseSelect;

	CHelperSlider * pSlider;

	std::vector<std::string> m_vRows;

	CColor * pInner, * pBorder, * pString, * pMouseOverString;
public:
	CListBox(CGUI *Gui, int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	void PutStr( std::string sString, int Index = -1 );
	std::string GetRow( int iIndex );

	void Clear();
	int GetSize();

	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );

	int GetSelected();
	void SetSelect(int Item = -1);
};