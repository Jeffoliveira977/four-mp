#pragma once

#include "CGUI.h"

class CListView : public CElement
{
	int Count;
	int * Widths;
	int * Poss;

	int m_iMouseOverIndex;
	int m_iMouseSelect;

	CScrollBar * pSlider;

	std::vector<std::string> *m_vRows;
	std::vector<std::string> mTitles;

	CColor * pInner, * pBorder, * pString, * pMouseOverString;
	CColor * pTitle, * pbTitle, * pbInner, * pTitleString;
public:
	CListView( int X, int Y, int *Width, int Height, int Columns, const char * String = NULL, const char *String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	bool MouseMove( CMouse * pMouse, bool );
	bool KeyEvent( SKey sKey );

	int GetSize(int Index = 0);

	void PutStr( std::string sString, int Column, int Row = -1 );
	std::string GetStr( int Row, int Column );

	void SetTitle( std::string sString, int Column = -1 );

	void Clear();
	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );

	int GetSelected();
	void SetSelect(int Item = -1);
};