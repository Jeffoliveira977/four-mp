#pragma once

#include "CGUI.h"

struct ImageColumn
{
	CTexture * tTrue;
	CTexture * tFalse;
	CTexture * tTitle;

	int Width, Height;
};

class CListView : public CElement
{
	int Count;
	int * Widths;
	int * Poss;
	ImageColumn * imgColumn;

	int m_iMouseOverIndex;
	int m_iMouseSelect;

	CScrollBar * pSlider;

	std::vector<std::string> *m_vRows;
	std::vector<std::string> m_vRowsHelp;
	std::vector<std::string> mTitles;

	CColor * pInner, * pBorder, * pString, * pMouseOverString, * pSelInner, * pSelString;
	CColor * pTitle, * pbTitle, * pbInner, * pTitleString;
public:
	CListView( CGUI *Gui, int X, int Y, int *Width, int Height, int Columns, const char * String = NULL, const char *String2 = NULL, tAction Callback = NULL );

	void Draw();
	void PreDraw();
	bool MouseMove( CMouse * pMouse, bool );
	bool KeyEvent( SKey sKey );

	int GetSize(int Index = 0);

	void PutStr( std::string sString, int Column, int Row = -1, std::string sHelp = "" );
	std::string GetStr( int Row, int Column );

	void SetTitle( std::string sString, int Column = -1 );

	void Clear();
	void UpdateTheme( int iIndex );
	void ShowSlider( bool bShow );

	int GetSelected();
	void SetSelect(int Item = -1);

	std::string GetHelpString(int row);

	void SetColumnImage(int Index, char *True, char *False, char *Title, int W = 16, int H = 16);
};