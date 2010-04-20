#pragma once

#include "CGUI.h"

class CEditBox : public CElement
{
	int m_iStart, m_iIndex;
	int m_iCursorX;
	CTimer m_tCursorTimer;
	bool m_bCursorState;

	CColor * pString, * pCursor;
	CTexture *pEdit[3];

	int SizeEdge;
	bool CenterAlign;

public:
	CEditBox( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, tAction Callback = NULL );

	void Draw();

	void PreDraw();
	bool MouseMove( CMouse * pMouse, bool );
	bool KeyEvent( SKey sKey );

	int GetIndex();
	void SetIndex( int iIndex );

	int GetStart();
	void SetStart( int iStart );
	void SetAlignCenter(bool);

	void UpdateTheme( int iIndex );
};