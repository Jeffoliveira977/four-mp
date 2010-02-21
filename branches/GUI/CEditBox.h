#pragma once

#include "CGUI.h"

class CEditBox : public CElement
{
	int m_iStart, m_iIndex;
	int m_iCursorX;
	CTimer m_tCursorTimer;
	bool m_bCursorState;

	typedef std::string ( __cdecl * tAction )( const char * pszArgs, CElement * pElement );
	tAction m_pCallback;

	CColor * pInner, * pBorder, * pString, * pCursor;

public:
	CEditBox( int X, int Y, int Width, int Height, const char * String = NULL, const char * String2 = NULL, const char * Callback = NULL );

	void Draw();

	void PreDraw();
	void MouseMove( CMouse * pMouse );
	void KeyEvent( SKey sKey );

	int GetIndex();
	void SetIndex( int iIndex );

	int GetStart();
	void SetStart( int iStart );

	void SetAction( tAction pCallback );
	tAction GetAction();

	void UpdateTheme( int iIndex );
};